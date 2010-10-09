/*
 * request.cpp
 *
 *  Created on: Sep 17, 2010
 *      Author: magnus
 */

#include <string>
#include <string.h>
#include <iostream>

#include "request.h"
#include "http.h"
#include "connection.h"
#include "bytebuffer.h"

Request::Request(Connection* connection) {
	// TODO Auto-generated constructor stub
	mHost="";
	mUri="";
	mVersion = Http::HTTP_VERSION_1_1;
	mType= HTTP_GET;
	mKeepAlive=true;
	mConnection = connection;
	mParseState = 0 ;
}

Request::~Request() {
	// TODO Auto-generated destructor stub
}

/**
 *
 * @param data
 * @param size
 * @return
 */
Request::ParseReturn Request::ParseRequest(Request* request,ByteBuffer* buffer)
{
	ParseReturn = REQUEST_UNFINISHED;

	char *ptr;
	char *line;
	int status=0;
	unsigned char* data = buffer->GetBuffer();

			if(strcmp(cmd,"HTTP/1.1\r")==0)
			{
				request->mVersion = Http::HTTP_VERSION_1_1;
				status++;
			}
			else if(strcmp(cmd,"HTTP/1.0\r")==0)
			{
				request->mVersion = Http::HTTP_VERSION_1_0;
				status++;
			}
		}
	}


	return ;
}

const std::string Request::ToString() const
{
	std::string str = (mType == HTTP_GET) ? "GET" : "POST";
	str+= " " + mUri +" ";
	str+= Http::GetVersionString(mVersion);
	std::cout << "\nRequest::ToString=" << str << "\n";
	return str;
}

const std::string & Request::GetUri() const
{
	return mUri;
}

Http::Version Request::GetHttpVersion() const
{
	return mVersion;
}

Request::RequestType Request::GetHttpType() const
{
	return mType;
}

Connection *Request::GetConnection() const
{
    return mConnection;
}

void Request::SetConnection(Connection *mConnection)
{
    mConnection = mConnection;
}



bool Request::GetKeepAlive() const
{
	return mKeepAlive;
}
