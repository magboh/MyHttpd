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

#include "site.h"
#include "request.h"
#include "response.h"
#include "connection.h"
#include "connectionmanager.h"
#include "requestqueue.h"
#include "bytebuffer.h"

Connection::Connection(int socket, ConnectionManager* conectionMgr, const Site& site,unsigned char threadNr) :
	mSocket(socket), mConnectionManager(conectionMgr),mSite(site), mTreadNr(threadNr)
{
	mSocket=socket;

	mReadBuffer=new ByteBuffer(4096);
	mWriteBuffer=new ByteBuffer(4096);

	mConnectionManager=conectionMgr;
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
	//	printf("Closing Connection\n");
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
		delete mRequest;

	if (mResponse)
		delete mResponse;
}

Connection::ReadStatus_t Connection::Read(size_t size)
{
	size_t toRead=size;
	ReadStatus_t status = READ_STATUS_OK;

	if (toRead>mReadBuffer->GetSpaceLeft())
	{
		toRead=mReadBuffer->GetSpaceLeft();
	}

	char* tbuff=new char[toRead];
	ssize_t len=read(mSocket,tbuff,toRead);
	int err=errno;
	if (len>0)
	{
		mReadBuffer->Add(tbuff,len);
	}
	else
	{
		switch (err)
		{
		case EAGAIN:
			// No more data to read now
			// remove connection from worker. Add again to IoWorker
			status = READ_STATUS_DONE;
			break;

		case EINTR: // Not sure what do do here.. Probably retry read
			status = READ_STATUS_AGAIN;
			break;

		default:
			// All other cases remove connection. And hope for the best..
			status = READ_STATUS_ERROR;
			break;
		}
	}

	delete[] tbuff;

	return status;
}

int Connection::GetSocket() const
{
	return mSocket;
}

int Connection::Write(size_t size)
{
	size_t toWrite=size;
	int ret=0;
	if (mWriteStatus==0)
	{
		if (size>mWriteBuffer->GetUsage())
			toWrite=mWriteBuffer->GetUsage();

		const char* buffer=mWriteBuffer->GetBuffer();
		int len=write(mSocket,buffer,toWrite);
		if (len>=0)
			mWritten+=len;

		if (mWritten==mWriteBuffer->GetUsage())
		{
			mWriteStatus=1;
			mWritten=0;
		}
	}

	if (mWriteStatus==1)
	{
		if (mResponse->GetFile()!=-1)
		{
			if (toWrite>mResponse->GetContentLength())
				toWrite=mResponse->GetContentLength();

			int len=0;
			off_t offset=mWritten;

			len=sendfile(mSocket,mResponse->GetFile(),&offset,toWrite);
			if (len>0)
				mWritten+=len;

			if (mWritten==mResponse->GetContentLength())
				mWriteStatus=2;
		}
		else
			mWriteStatus=2;
	}

	// All written...
	if (mWriteStatus==2)
	{
		if ((mResponse->GetHttpVersion()==Http::HTTP_VERSION_1_0)||mResponse->GetKeepAlive()==false)
			SetCloseable(true);
		ret=1;
		mWriteBuffer->Clear();
		mWritten=0;
		mWriteStatus=0;
		delete mResponse;
		mResponse=NULL;
		mHasData=false;
	}

	return ret;
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
 * Called from RequestQueueWorker thread to "deliver" the repsonse.
 * ConnectionQueueoworked thread SHOULD not touch this data until SetHasData() is called..
 * @param response
 */
void Connection::SetResponse(const Response* response)
{
	//std::cout << "Connection::SetResponse:\n";
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
