/*
 * connection.cpp
 *
 *  Created on: Sep 21, 2010
 *      Author: magnus
 */

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <poll.h>
#include <pthread.h>
#include <sys/sendfile.h>

#include "request.h"
#include "response.h"
#include "connection.h"
#include "requestqueue.h"

Connection::Connection(int socket) {
	mSocket = socket;

	mReadBufferSize= 4096 ;
	mReadBuffer= new unsigned char[mReadBufferSize];

	mWriteBufferSize= 4096 ;
	mWriteBuffer= new unsigned char[mWriteBufferSize];
	mToWrite=0;
}

Connection::~Connection()
{
	if (!mReadBuffer)
	{
		delete [] mReadBuffer;
		mReadBuffer = 0;
	}

	if (!mWriteBuffer)
	{
		delete [] mWriteBuffer;
		mWriteBuffer = 0;
	}

}

void Connection::Read()
{
	int len;
	size_t readBytes=0;
	while( (len = read(mSocket,mReadBuffer+readBytes,mReadBufferSize-readBytes) ) > 0)
	{
		readBytes+= len ;
		if (strcmp("\n\r\n\r",(char*)mReadBuffer-4)) {
			break;
		}
	}
	//std::cout << "mBuffer=" << mBuffer << "\n";
	// Read 0 bytes, means socket is closes on other side
	if (readBytes>0)
	{
		Request* req = Request::ParseRequest(mReadBuffer,readBytes,this);
		if (req)
		{
			sleep(2);
			RequestQueue::GetInstance()->AddRequest(req);
		}

	}
	else
	{
		perror("read error:");
	}
}

int Connection::GetSocket() const
{
	return mSocket;
}



void Connection::Write(Response* response)
{

	mToWrite =  response->ToBuffer(mWriteBuffer,mWriteBufferSize);
	Write();
	mToWrite = response->GetContentLength();

	sendfile(mSocket,response->GetFile(),0,mToWrite);

}

void Connection::Write()
{
	int len;
	size_t writtenBytes=0;

	while( (len = write(mSocket,mWriteBuffer+writtenBytes,mToWrite-writtenBytes) ) > 0  )
	{
		writtenBytes+= len ;
	}

}
