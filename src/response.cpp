/***************************************************************************
 *      MyHTTPd
 *
 *      Tue, 15 Mar 2011 22:16:12 +0100
 *      Copyright 2011 Magnus Bohman
 *      magnus.bohman@gmail.com
 ***************************************************************************/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, US.
 */

#include <sstream>
#include <string.h>

#include "request.h"
#include "response.h"
#include "bytebuffer.h"
#include "myhttpd.h"

#define EOL "\r\n"

Response *Response::CreateResponse(const Request *request)
{
	Response* response=new Response();
	response->SetHttpVersion(request->GetHttpVersion());
	response->mKeepAlive=request->GetKeepAlive();

	return response;
}

Response::~Response()
{
	if (mFile!=-1)
	{
		//		close(mFile);
		mFile=-1;
	}
}

Response::Response()
{
	mFile=-1;
	mContentLength=0;
}

int Response::GetFile() const
{
	return mFile;
}

void Response::SetFile(int fd)
{
	mFile=fd;
}

unsigned int Response::GetContentLength() const
{
	return mContentLength;
}

void Response::SetContentLength(unsigned int length)
{
	mContentLength=length;
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

	Http::Status status=GetStatus();
	ss<<Http::GetVersionString(GetHttpVersion())<<" "<<status<<" "<<Http::GetStatusString(status)<<EOL;
	ss<<"Server: MyHttpd"<< std::string(VersionString) << EOL;
	ss<<"Connection: ";
	if (mKeepAlive)
	{
		ss<<"keep-alive"<<EOL;
	}
	else
	{
		ss<<"close"<<EOL;
	}

	if (status==Http::HTTP_OK)
	{
		ss<<"Content-Length:"<<mContentLength<<EOL;
		ss<<"Content-Type: text/html"<<EOL;
		ss<<EOL;
	}
	else
	{
		str="<html><body><h1>"+Http::GetStatusString(status)+"</h1></body></html>";
		ss<<"Content-Type: text/html\r\n";
		ss<<EOL;
		ss<<str;
	}

	len=ss.str().size();

	if (len>buffer->GetSpaceLeft())
		len=buffer->GetSpaceLeft();
	//	std::cout << "ToBuffer= size=" << len <<"\n";
	buffer->Add(ss.str().c_str(), len);
	return len;
}

