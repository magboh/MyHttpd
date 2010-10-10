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
}

Connection::~Connection()
{
	if (mReadBuffer)
	{
		delete [] mReadBuffer;
		mReadBuffer = 0;
	}

	if (mWriteBuffer)
	{
		delete [] mWriteBuffer;
		mWriteBuffer = 0;
	}

	if (mRequest)
		delete mRequest;

//	printf("\nClosing Connection socket=%d\n",mSocket);
}

bool Connection::Read(size_t size)
{
	int len;
	size_t toRead = size;

	if (toRead > mReadBuffer->GetSpaceLeft())
	{
		toRead = mReadBuffer->GetSpaceLeft();
	}

	char* tbuff = new char[toRead];
	len = read(mSocket, tbuff , toRead);


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
				RequestQueue::GetInstance()->AddRequest(mRequest);
				mRequest = NULL;
				break;
			}

			default:
				assert(false); // SHOULD NOT BE HERE
				break;

			}
		}
	}
	delete [] tbuff;

	return len;
}

int Connection::GetSocket() const
{
	return mSocket;
}

void Connection::Write(size_t size)
{
	size_t toWrite = size;
//	std::cout << "\nWrite" << size << "bytes\n";
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
	}

	// All written...
	if (mWriteStatus == 2)
	{
//		if ( !mResponse->GetKeepAlive() )
		mResponse = NULL;
		mHasData = false;
	}


}


bool Connection::HasData()
{
	return mHasData;
}

void Connection::SetHasData(bool b)
{
	mHasData = b ;
}

void Connection::SetResponse(const Response* response)
{
//	std::cout << "Connection::SetResponse:\n";
	mWriteBuffer->Clear();
	mResponse = response;
	mResponse->ToBuffer(mWriteBuffer);
}
