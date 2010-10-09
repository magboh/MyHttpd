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

int Connection::Read(size_t size)
{
	int len;
	size_t toRead = size;

	if (toRead > mReadBuffer->GetSpaceLeft())
	{
		toRead = mReadBuffer->GetSpaceLeft();
	}

	unsigned char* tbuff = new unsigned char[toRead];

	len = read(mSocket, tbuff , toRead);


	if (len > 0)
	{
		mReadBuffer->Add(tbuff,len);

		if (mRequest==NULL)
			mRequest = new Request(this);

		Request::ParseReturn parseReturn = Request::ParseRequest(mRequest,mReadBuffer);

		switch (parseReturn)
		{
			case Request::REQUEST_OK:
			{
				RequestQueue::GetInstance()->AddRequest(mRequest);
				break;
			}
			case Request::REQUEST_HTPP_VERSION_NOT_SUPPORTED:
			{
				assert(false); // TODO implement sending of HTTP Version Not Supported
				break;
			}
			case Request::REQUEST_BAD:
			{
				assert(false); // TODO implement sending of 400 Bad Request
				break;
			}

			case Request::REQUEST_TO_LARGE:
			{
				assert(false); // TODO implement sending of 413 Request Entity Too Large
				break;
			}
			case Request::REQUEST_URI_TO_LONG:
			{
				assert(false); // TODO implement sending of 	414 Request-URI Too Long
				break;
			}

			case Request::REQUEST_UNFINISHED: /* This means we got ot read more data*/
			{

				break;
			}
		}


	}

	if (strcmp("\n\r\n\r",(char*)mReadBuffer-4))
	{
		Request* req = Request::ParseRequest(mReadBuffer,this);
		if (req)
		{
			RequestQueue::GetInstance()->AddRequest(req);
		}
	}

	delete [] tbuff;
	return len;
}

int Connection::GetSocket() const
{
	return mSocket;
}



void Connection::Write(const Response* response)
{

	int mToWrite =  response->ToBuffer(mWriteBuffer);
	Write(mToWrite);

	if (response->GetFile()!=-1)
	{
		mToWrite = response->GetContentLength();
	//	std::cout << "\nSenfile toWrite:" << mToWrite;
		int len=0;
		int chunk=4096;
		off_t offset = 0;
		while( (len=sendfile(mSocket,response->GetFile(),&offset,chunk))>0)
		{
//			std::cout << "\nSenfile written:" << offset << "/" << mToWrite;

		}
	}

	if (!response->GetKeepAlive())
	{
	}
}

void Connection::Write(size_t size)
{
	size_t writtenBytes=0;
	size_t toWrite = size;
	if ( size > mWriteBuffer->GetSize() )
		toWrite = mWriteBuffer->GetSize();

	unsigned char* buffer = mWriteBuffer->GetBuffer();
	write(mSocket, buffer, toWrite);
}
