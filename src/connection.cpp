/*
 * connection.cpp
 *
 *  Created on: Sep 21, 2010
 *      Author: magnus
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <poll.h>
#include <pthread.h>
#include <sys/sendfile.h>
#include <iostream>
#include <poll.h>
#include <time.h>

#include "request.h"
#include "response.h"
#include "connection.h"
#include "connectionmanager.h"
#include "requestqueue.h"
#include "bytebuffer.h"

Connection::Connection(int socket,ConnectionManager* conectionMgr) {
	mSocket = socket;

	mReadBuffer = new ByteBuffer(4096);
	mWriteBuffer = new ByteBuffer(4096);

	mConnectionManager =  conectionMgr;
	mWantToRead = true;
	mRequest = NULL;
	mHasData = false;
	mWriteStatus = 0 ;
	mWritten = 0;

	mCreated = time(NULL);
	mLastRequest = mCreated ;
	SetCloseable(false);
}

Connection::~Connection()
{
	printf("Closing Connection\n");
	if (mReadBuffer)
	{
		delete mReadBuffer;
		mReadBuffer = 0;
	}

	if (mWriteBuffer)
	{
		delete mWriteBuffer;
		mWriteBuffer = 0;
	}

	if (mRequest)
		delete mRequest;

	if (mResponse)
		delete mResponse;
}

bool Connection::Read(size_t size)
{
	bool keepAlive=true;
	size_t toRead = size;
	bool done = false;
	if (toRead > mReadBuffer->GetSpaceLeft())
	{
		toRead = mReadBuffer->GetSpaceLeft();
	}

	char* tbuff = new char[toRead];
	int len = read(mSocket, tbuff , toRead);


	if (len > 0)
	{
		mReadBuffer->Add(tbuff,len);

		if (mRequest==NULL)
			mRequest = new Request(this);

		if (Request::ParseRequest(mRequest, mReadBuffer))
		{
			switch (mRequest->GetStatus())
			{

			case Http::HTTP_REQUEST_URI_TO_LONG:
			case Http::HTTP_REQUEST_TO_LARGE:
			case Http::HTTP_BAD_REQUEST:
			case Http::HTTP_REQUEST_VERSION_NOT_SUPPORTED:
			case Http::HTTP_OK:
			{
				// Transfer ownership of request to RequestQueue..
				done= true;
				break;
			}

			default:
				assert(false); // SHOULD NOT BE HERE
				break;

			}
		}
	}
	delete [] tbuff;

	return done;
}

int Connection::GetSocket() const
{
	return mSocket;
}

int Connection::Write(size_t size)
{
	size_t toWrite = size;
	int ret = 0 ;
	if (mWriteStatus==0)
	{
		if ( size > mWriteBuffer->GetUsage() )
			toWrite = mWriteBuffer->GetUsage();
	//	std::cout << "\nWrite" << size << " " << mWriteBuffer->GetUsage()  <<"\n";

		const char* buffer = mWriteBuffer->GetBuffer();
		int len = write(mSocket, buffer, toWrite);
		if (len>=0)
			mWritten+=len;

		if (mWritten==mWriteBuffer->GetUsage())
		{
			mWriteStatus=1;
			mWritten=0;
		}
	}

	if (mWriteStatus == 1)
	{
		if (mResponse->GetFile()!=-1)
		{
			if (toWrite > mResponse->GetContentLength() )
				toWrite = mResponse->GetContentLength();

			int len=0;
			off_t offset = mWritten;

			len=sendfile(mSocket,mResponse->GetFile(),&offset,toWrite);
			if (len>0)
				mWritten+=len;

			if (mWritten == mResponse->GetContentLength())
				mWriteStatus = 2;
		}
		else
			mWriteStatus = 2;
	}

	// All written...
	if (mWriteStatus == 2)
	{
		if ( (mResponse->GetHttpVersion()==Http::HTTP_VERSION_1_0) || mResponse->GetKeepAlive()==false )
			SetCloseable(true);
		ret = 1 ;
		mWriteBuffer->Clear();
		mWritten=0;
		mWriteStatus=0;
		delete mResponse;
		mResponse=NULL;
		mHasData = false ;
	}

	return ret;
}


bool Connection::HasData()
{
	return mHasData;
}

void Connection::SetHasData(bool b)
{
	mHasData = b ;
}

/**
 * Called from RequestQueueWorker thread to "deliver" the repsonse.
 * ConnectionQueueoworked thread SHOULD not touch this data until SetHasData() is called..
 * @param response
 */
void Connection::SetResponse(const Response* response)
{
	std::cout << "Connection::SetResponse:\n";
	assert(mResponse==NULL);
	mResponse = response;
	mResponse->ToBuffer(mWriteBuffer);
}

Request* Connection::GetRequest() const
{
	return mRequest;
}

void Connection::SetRequest(Request* request)
{
	mRequest = request;
}

void Connection::SetLastRequstTime(time_t lastTime)
{
	mLastRequest = lastTime;
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
	mCloseable = closeable;
}


