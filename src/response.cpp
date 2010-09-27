/*
 * response.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#include "pthread.h"
#include <iostream>
#include "request.h"
#include "response.h"
#include "requestqueueworker.h"


Response *Response::CreateResponse(const Request *request)
{
	Response* response = new Response();
	response->SetHttpVersion( request->GetHttpVersion() );
	return response;
}

Response::~Response()
{
}

Response::Response()
{
	mFile = -1 ;
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


void Response::SetStatus(StatusType status)
{
	mStatus = status;
}

Response::StatusType Response::GetStatus() const
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
