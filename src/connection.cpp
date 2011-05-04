/***************************************************************************
 *      MyHTTPd
 *
 *      Tue, 15 Mar 2011 22:16:12 +0100
 *      Copyright 2011 Magnus Bohman
 *      magnus.bohman@gmail.com
 ***************************************************************************/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, US.
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <poll.h>
#include <pthread.h>
#include <sys/sendfile.h>
#include <poll.h>
#include <time.h>
#include <errno.h>
#include <sstream>
#include <iostream>

#include "site.h"
#include "request.h"
#include "response.h"
#include "connection.h"
#include "bytebuffer.h"
#include "logger.h"

Connection::Connection(int socket, const Site* site) :
	mSocket(socket), mSite(site)
{
	mSocket=socket;

	mReadBuffer=new ByteBuffer(4096);
	mWriteBuffer=new ByteBuffer(4096);

	mWantToRead=true;
	mRequest=NULL;
	mHasData=false;
	mWriteStatus=0;
	mWritten=0;
	mResponse=NULL;
	mCreated=time(NULL);
	mLastRequest=mCreated;
	SetCloseable(false);
}

Connection::~Connection()
{
	if (mReadBuffer)
	{
		delete mReadBuffer;
		mReadBuffer=0;
	}

	if (mWriteBuffer)
	{
		delete mWriteBuffer;
		mWriteBuffer=0;
	}

	if (mRequest)
	{
		delete mRequest;
		mRequest =0;
	}

	if (mResponse)
	{
		delete mResponse;
		mResponse=0;
	}

	if(mSocket!=-1)
	{
		close(mSocket);
		mSocket=-1;
	}
}

Connection::Status_t Connection::Read(size_t size)
{
	size_t toRead=size;
	Status_t status=STATUS_OK;

	if (toRead>mReadBuffer->GetSpaceLeft())
	{
		toRead=mReadBuffer->GetSpaceLeft();
	}

	ssize_t len=read(mSocket,mReadBuffer->GetCurrentBufferPtr(),toRead);
	int err=errno;

	if (len>0 )
	{
		mReadBuffer->Add(len);
		if (IsAppLog(Logger::DEBUG))
		{
			std::stringstream ss;
			ss<<"Bytes Read:" << len ;
			AppLog(Logger::DEBUG,ss);
		}
	}
	else if (len==0)
	{
		status=STATUS_CLOSE;
	}
	else
	{
		status=ErrnoToStatus(err);
	}
	return status;
}

int Connection::GetSocket() const
{
	return mSocket;
}

Connection::Status_t Connection::Write(size_t size)
{
	size_t toWrite=size;
	Status_t status=STATUS_OK;

	if (mWriteStatus==0)
	{

		if (mWritten+size>mWriteBuffer->GetUsage())
			toWrite=mWriteBuffer->GetUsage()-mWritten;

		const char* buffer=mWriteBuffer->GetBuffer();
		ssize_t len=write(mSocket,buffer+mWritten,toWrite);
		int err=errno;

		if (len>=0)
		{
			mWritten+=len;

			if (mWritten==mWriteBuffer->GetUsage())
			{
				mWriteStatus=1;
				mWritten=0;
				status=STATUS_DONE;
			}
			else
			{
				return status;
			}
		}
		else
		{
			perror("sending header");
			status=ErrnoToStatus(err);
		}
	}

	if (mWriteStatus==1&&mResponse->GetFile()!=-1)
	{
		toWrite = size;
		if (mWritten+size>mResponse->GetContentLength())
		{
			toWrite=mResponse->GetContentLength()-mWritten;
		}

		ssize_t len=0;
		off_t offset=mWritten;

		len=sendfile(mSocket,mResponse->GetFile(),&offset,toWrite);
		int err=errno;

		if (len>0)
		{
			mWritten+=len;
			status = STATUS_OK;
			if (mWritten==mResponse->GetContentLength())
			{
				mWriteStatus=2;
				status=STATUS_DONE;
			}
			else
			{
				return status;
			}
		}
		else
		{
			status=ErrnoToStatus(err);
		}
	}

	// All written...
	if (mWriteStatus==2||status==STATUS_ERROR||status==STATUS_DONE)
	{
		if ((mResponse->GetHttpVersion()==Http::HTTP_VERSION_1_0)||mResponse->GetKeepAlive()==false)
		{
			SetCloseable(true);
		}
		mWriteBuffer->Clear();
		mWritten=0;
		mWriteStatus=0;
		delete mResponse;
		mResponse=NULL;
		mHasData=false;
	}

	return status;
}

bool Connection::HasData()
{
	return mHasData;
}

void Connection::SetHasData(bool b)
{
	mHasData=b;
}

/**
 * Called from RequestWorker thread to "deliver" the repsonse.
 * ConnectionQueueoworked thread SHOULD not touch this data until SetHasData() is called..
 * @param response
 */
void Connection::SetResponse(const Response* response)
{
	assert(mResponse==NULL);
	mResponse=response;
	mResponse->ToBuffer(mWriteBuffer);
}

Request* Connection::GetRequest() const
{
	return mRequest;
}

void Connection::SetRequest(Request* request)
{
	mRequest=request;
}

void Connection::SetLastRequstTime(time_t lastTime)
{
	mLastRequest=lastTime;
}

time_t Connection::GetLastRequstTime() const
{
	return mLastRequest;
}

bool Connection::IsCloseable() const
{
	return mCloseable;
}

void Connection::SetCloseable(bool closeable)
{
	mCloseable=closeable;
}

bool Connection::Parse()
{
	if (mRequest==NULL)
	{
		mRequest=new Request(this,mSite);
	}
	return Request::ParseRequest(mRequest,mReadBuffer);
}

Connection::Status_t Connection::ErrnoToStatus(int error)
{
	Status_t status=STATUS_ERROR;
	switch (error)
	{
	case EAGAIN:
		// No more data to read now
		// remove connection from worker. Add again to IoWorker
		status=STATUS_AGAIN;
		break;
	case EINTR: // Not sure what do do here.. Probably retry read
		status=STATUS_INTERUPT;
		break;
	default:
		// All other cases remove connection. And hope for the best..
		status=STATUS_ERROR;
		break;
	}
	return status;
}
