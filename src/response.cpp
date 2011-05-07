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

#include "response.h"
#include "bytebuffer.h"
#include "myhttpd.h"
#include "logger.h"

#define EOL "\r\n"

std::string ToGMTStr(time_t t);

Response::Response(Http::Version version, bool keepAlive) :
	mContentLength(0), mContentType("text/html"), mFile(-1)
{
	SetHttpVersion(version);
	SetKeepAlive(keepAlive);
}

Response::~Response()
{
	if (mFile!=-1)
	{
		//		close(mFile);
		mFile=-1;
	}
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
std::string ToGMTStr(time_t t)
{
	char buff[30];
	tm tmm;
	gmtime_r(&t,&tmm);
	strftime(buff,30,"%a, %d %b %Y %T GMT",&tmm);
	return std::string(buff);
}

int Response::ToBuffer(ByteBuffer* buffer) const
{
	std::stringstream ss;
	std::string str="";
	size_t len=0;

	Http::Status status=GetStatus();
	ss<<Http::GetVersionString(GetHttpVersion())<<" "<<status<<" "<<Http::GetStatusString(status)<<EOL;
	ss<<"Server: " << ServerHeader <<EOL;

	// Do some HTTP 1.1 specifics
	if (GetHttpVersion()==HTTP_VERSION_1_1)
	{
		ss<<"Connection: " << ((GetKeepAlive()) ? "keep-alive\r\n" : "close\r\n");
	}

	time_t t;
	time(&t);
	ss<<"Date: "<< ToGMTStr(t) << EOL;

	if (status==Http::HTTP_OK)
	{
			ss<<"Last-Modified: " << ToGMTStr(mTime)<<EOL;
			ss<<"Content-Length: "<<mContentLength<<EOL;
			ss<<"Content-Type: " << mContentType << "\r\n\r\n";
	}
	else
	{
		ss<<"Content-Type: text/html\r\n\r\n";
		str="<html><body><h1>"+Http::GetStatusString(status)+"</h1></body></html>";
		ss<<str;
	}

	str = ss.str();
	len=str.size();

	if (len>buffer->GetSpaceLeft())
		len=buffer->GetSpaceLeft();

	buffer->Add(str.c_str(),len);

	if (IsAppLog(Logger::DEBUG))
		AppLog(Logger::DEBUG,ss);

	return len;
}


void Response::SetContentType(const std::string& type)
{
	mContentType = type;
}

const std::string& Response::GetContentType()
{
	return mContentType;
}

void Response::SetLastModTime(time_t t)
{
	mTime=t;
}
