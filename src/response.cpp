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
#include "bytebuffer.h"

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
	mKeepAlive = true;
}


int Response::GetFile() const
{
	return mFile;
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

bool Response::GetKeepAlive() const
{
	return mKeepAlive;
}

int Response::ToBuffer(ByteBuffer* buffer) const
{
	std::stringstream ss;
	std::string str="";
	size_t len=0;

	ss << Http::GetVersionString(mVersion) << " " << mStatus << " " << Http::GetStatusString(mStatus) <<"\r\n";

//	if (!mKeepAlive)
	//{
		ss << "Connection:Close\r\n";
	//}


	if (mStatus == Http::HTTP_OK)
	{
		ss << "Content-Length:" << mContentLength << "\r\n";
		ss << "Content-Type: text/html\r\n";
		ss << "\r\n";
	}
	else
	{
		str=  "<html><body><h1>" + Http::GetStatusString(mStatus) + "</h1></body></html>";
	//	mContentLength = str.length();
//		ss << "Content-Length:" << mContentLength << "\r\n";
		ss << "Content-Type: text/html\r\n";
		ss << "\r\n";
		ss << str;
	}

	len = ss.str().size();

	if (len>buffer->GetSpaceLeft())
		len=buffer->GetSpaceLeft();
//	std::cout << "ToBuffer= size=" << len <<"\n";
	buffer->Add( ss.str().c_str() , len);
	return len;
}


