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


#include "request.h"
#include "connection.h"
#include "requestqueue.h"

Connection::Connection(int socket) {
	mSocket = socket;
	mBufferSize= 1000 ;
	mBuffer= new unsigned char[mBufferSize];
}

Connection::~Connection()
{
	if (!mBuffer)
	{
		delete [] mBuffer;
		mBuffer = 0;
	}
}

void Connection::Read()
{
	int len;
	size_t readBytes=0;
	while( (len = read(mSocket,mBuffer+readBytes,mBufferSize-readBytes) ) > 0)
	{
		readBytes+= len ;
		if (strcmp("\n\r\n\r",(char*)mBuffer-4)) {
			break;
		}
	}
	//std::cout << "mBuffer=" << mBuffer << "\n";
	// Read 0 bytes, means socket is closes on other side
	if (readBytes>0)
	{
		Request* req = Request::ParseRequest(mBuffer,readBytes);
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

