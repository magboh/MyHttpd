/*
 * request.cpp
 *
 *  Created on: Sep 17, 2010
 *      Author: magnus
 */

#include <string>
#include <string.h>
#include <iostream>
#include <cassert>
#include "request.h"
#include "http.h"
#include "connection.h"
#include "bytebuffer.h"

Request::Request(Connection* connection) {
	// TODO Auto-generated constructor stub
	mHost="";
	mUri="";
	mType= HTTP_GET;
	mKeepAlive=true;
	mConnection = connection;
	mParseState = 0 ;
	mParsePos = 0;
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
bool Request::ParseRequest(Request* request,ByteBuffer* buffer)
{
	ParseReturn pret= REQUEST_UNFINISHED;

	char *ptr;
	char *line;
	int status=0;
	int parsePos=request->mParsePos;
	// ParseState == 0 is GET/POST line
	const char* data = buffer->GetBuffer();
	size_t size = buffer->GetUsage();
	if (size < 18)
		return false;
//	std::cout << "parsePos=" << parsePos << " size=" << size <<"\n";
	if (request->mParseState==0)
	{
		Http::Version version = Http::HTTP_VERSION_1_1;
		RequestType rt = Request::HTTP_GET;

		if (strncmp(data,"GET",3)==0)
		{
			rt = Request::HTTP_GET;
			parsePos+=3;
//			std::cout << "ParsePos=" << parsePos << "GET\n";
		}
		else if (strncmp(data,"POST",4)==0)
		{
			rt = Request::HTTP_POST;
			parsePos+=4;
			std::cout << "rparsePos=" << parsePos << "POST\n";
		}

		assert(data[parsePos]==' ');
		parsePos++;

		int i=parsePos;
		for (i;i<size;i++)
			if (data[i]==' ')
				break;

		if (i-parsePos > Request::MAX_URI_LENGTH)
		{
//			std::cout << "URI too LONG\n";
			request->SetStatus(Http::HTTP_REQUEST_URI_TO_LONG);
			return true;
		}

		std::string uri = std::string( data+parsePos, i-parsePos);
		parsePos+= i-parsePos;
//		std::cout << "parsePos=" << parsePos << " URI="<< uri <<"\n";


		if (size > parsePos+9)
		{
			assert(data[parsePos]==' ');
			parsePos++;
			if(strncmp("HTTP/1.1",data+parsePos,8)==0)
			{
				version = Http::HTTP_VERSION_1_1;
				parsePos+=8;
			}
			else if(strncmp("HTTP/1.0",data+parsePos,8)==0)
			{
				version = Http::HTTP_VERSION_1_0;
				parsePos+=8;
			}
			else
			{
	//			std::cout << "Bad version\n";
				request->SetStatus(Http::HTTP_REQUEST_VERSION_NOT_SUPPORTED);
				return true;
			}


		}
		else
			return false;

//		std::cout << "parsePos=" << parsePos << "version=" << Http::GetVersionString(version) << " \n";

		if (size > parsePos+2)
		{
			if (*(data+parsePos)=='\r')
				parsePos++;

			if (*(data+parsePos)=='\n')
			{
				parsePos++;

				request->mParseState = 1 ;
				request->mType = rt;
				request->SetHttpVersion(version);
				request->mUri = uri;
				request->mParsePos = parsePos;
				request->mKeepAlive = false;
			}
		}
	}

	if (request->mParseState == 1)
	{
		for (int i=request->mParsePos; i<=size-4; i++)
		{
			if (strncmp("\r\n\r\n",(data+i),4)==0)
			{
				request->mParsePos=0;
				request->SetStatus(Http::HTTP_OK);
				buffer->Remove(i+4);
//				std::cout << "request->mParsePos=" << request->mParsePos << " req=" << request->ToString() <<"\n";
				return true;
			}
		}
	}
	return false;

}

const std::string Request::ToString() const
{
	std::string str = (mType == HTTP_GET) ? "GET" : "POST";
	str+= " " + mUri +" ";
	str+= Http::GetVersionString( GetHttpVersion() );
	str+= " " + Http::GetStatusString(GetStatus());
	std::cout << "\nRequest::ToString=" << str << "\n";
	return str;
}

const std::string & Request::GetUri() const
{
	return mUri;
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


