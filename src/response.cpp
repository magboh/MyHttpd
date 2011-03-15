/*
 * response.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#include <sstream>
#include <string.h>

#include "request.h"
#include "response.h"
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
//		close(mFile);
		mFile = -1;
	}
}

Response::Response()
{
	mFile = -1 ;
	mContentLength = 0 ;
}


int Response::GetFile() const
{
	return mFile;
}


void Response::SetFile(int fd)
{
	mFile = fd;
}


unsigned int Response::GetContentLength() const
{
	return mContentLength;
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

	Http::Status status = GetStatus();
	ss << Http::GetVersionString(GetHttpVersion()) << " " << status << " " << Http::GetStatusString(status) <<"\r\n";

	if (mKeepAlive)
	{
		ss << "Connection: keep-alive\r\n";
	}
	else
	{
		ss << "Connection: close\r\n";
	}


	if (status == Http::HTTP_OK)
	{
		ss << "Content-Length:" << mContentLength << "\r\n";
		ss << "Content-Type: text/html\r\n";
		ss << "\r\n";
	}
	else
	{
		str=  "<html><body><h1>" + Http::GetStatusString(status) + "</h1></body></html>";
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


