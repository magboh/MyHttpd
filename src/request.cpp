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

Request::Request(Connection* connection) {
	// TODO Auto-generated constructor stub
	mHost="";
	mUri="";
	mVersion = Http::HTTP_VERSION_1_1;
	mType= HTTP_GET;
	mKeepAlive=false;
	mConnection = connection;
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
Request* Request::ParseRequest(unsigned char* data, size_t size,Connection* connection)
{
	Request* request = new Request(connection);
	char *ptr;
	char *line;
	int l=0;
	int status=0;
	line=strtok_r((char*)data,"\n",&ptr);

	if (line != NULL)
	{
		char *lptr;
		char *cmd;
		// GET / POST
		cmd = strtok_r((char*)line," ",&lptr);
		if (cmd)
		{
			if (strcmp(cmd,"GET")==0)
			{
				request->mType = Request::HTTP_GET;
				status++;
			}
			else if (strcmp(cmd,"POST")==0)
			{
				request->mType = Request::HTTP_POST;
				status++;
			}
		}

		//URI
		cmd = strtok_r(NULL," ",&lptr);

		if (cmd && strlen(cmd)<=MAX_URI_LENGTH)
		{
			request->mUri = cmd;
			status++;
		}


		// HTTP Version
		cmd = strtok_r(NULL," ",&lptr);

		if (cmd)
		{
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

	while( (line=strtok_r(NULL,"\n",&ptr)) )
	{
	}

	// IF not enough info retrieved
	if (status < 3 )
	{
		delete request;
		request = NULL;
	}
	return request;
}

const std::string Request::ToString() const
{
	std::string str = (mType == HTTP_GET) ? "GET" : "POST";
	str+= " " + mUri +" ";
	str+= (mVersion == Http::HTTP_VERSION_1_1) ? "HTTP/1.1" : "HTTP/1.0";
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


