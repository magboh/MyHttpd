/*
 * response.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#include "pthread.h"
#include <iostream>
#include <sstream>
#include "string.h"

#include "request.h"
#include "response.h"
#include "requestqueueworker.h"

Response *Response::CreateResponse(const Request *request)
{
	Response* response = new Response();
	response->SetHttpVersion( request->GetHttpVersion() );
	response->mKeepAlive = request->GetKeepAlive();
	return response;
}

Response::~Response()
{
	if (mFile!=-1)
	{
		close(mFile);
		mFile = -1;
	}
}

Response::Response()
{
	mFile = -1 ;
	mContentLength = 0 ;
}


int Response::GetFile()
{
	return mFile;
}

const unsigned char *Response::ToBuffer()
{
}

void Response::SetFile(int fd)
{
	mFile = fd;
}


void Response::SetStatus(Http::Status status)
{
	mStatus = status;
}

Http::Status Response::GetStatus() const
{
	return mStatus;
}

void Response::SetHttpVersion(Http::Version version)
{
	mVersion = version;
}

unsigned int Response::GetContentLength() const
{
	return mContentLength;
}

Http::Version Response::GetHttpVersion() const
{
	return mVersion;
}

void Response::SetContentLength(unsigned int length)
{
	mContentLength = length;
}

int Response::ToBuffer(unsigned char* buffer, int length)
{
	std::stringstream ss;
	std::string str="";
	int len=0;

	if (mStatus != Http::HTTP_OK)
	{
		str=  "<html><body><h1>" + Http::GetStatusString(mStatus) + "</h1></body></html>";
	}

	ss << Http::GetVersionString(mVersion) << " " << mStatus << " " << Http::GetStatusString(mStatus) <<" \r\n";

	if (mStatus != Http::HTTP_OK)
	{
		mContentLength = str.length();
	}
/*	if ( mFile =! -1)
	{*/
		ss << "Content-Length:" << mContentLength << " \r\n";
		ss << "Content-Type: text/html \r\n";
	//}

	if (!mKeepAlive)
	{
		ss << "Connection:Close";
	}

	ss << "\r\n";

	if (mStatus != Http::HTTP_OK)
	{
		ss << str;
	}

//	ss <<"APA GNU LEJON";
	len = ss.str().size();

	if (len>length)
		len=length;

	memcpy(buffer,ss.str().c_str(),len);
//	std::cout <<" \n" << ss.str() <<"\n";
//	std::cout <<"buff \n" << buffer <<"\n";

	return len;
}

