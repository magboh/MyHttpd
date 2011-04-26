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

#include <string>
#include <string.h>
#include <iostream>

#include <cassert>
#include "request.h"
#include "connection.h"
#include "bytebuffer.h"
#include "site.h"
#include "logger.h"

Request::Request(Connection* connection, const Site& site) :
	mSite(site)
{
	// TODO Auto-generated constructor stub
	mHost="";
	mUri="";
	mType=HTTP_UNDEF;
	mConnection=connection;
	mParseState=0;
}

Request::~Request()
{
	// TODO Auto-generated destructor stub
}

const std::string Request::ToString() const
{
	std::string str=(mType==HTTP_GET) ? "GET" : "HEAD";
	str+=" "+mUri+" ";
	str+=Http::GetVersionString(GetHttpVersion());
	str+=" "+Http::GetStatusString(GetStatus())+"\n";

	for (std::map<std::string, std::string>::const_iterator it=mHeader.begin();it!=mHeader.end();++it)
	{
		str+="'"+it->first+"':'"+it->second+"'\n";
	}
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
	mConnection=mConnection;
}

const Site& Request::GetSite() const
{
	return mSite;
}

/**
 *
 * @param data
 * @param size
 * @return
 */
bool Request::ParseRequest(Request* request, ByteBuffer* buffer)
{
	const char* data=buffer->GetBuffer();
	size_t size=buffer->GetUsage();
	bool retVal=false;
	size_t position=0;
	if (request->mParseState==0)
	{
		position=ParseRequestLine(request,data,size);
		if (position>0)
			request->mParseState=1;
	}
	if (request->GetStatus()==Http::HTTP_OK&&request->mParseState==1)
	{
		bool ok=ParseRequestHeaders(request,data,size,position);
		if (ok)
		{
			buffer->Clear();

			if (request->GetHttpVersion()==Http::HTTP_VERSION_1_1)
			{
				request->SetKeepAlive(true);

				if (request->mHeader.find("Host")==request->mHeader.end())
				{
					request->SetStatus(Http::HTTP_BAD_REQUEST);
				}
			}
			else
				request->SetKeepAlive(false);

			std::map<std::string, std::string>::iterator it=request->mHeader.find("Connection");

			if (it!=request->mHeader.end())
			{
				std::string & header=it->second;
				if (header.compare("keep-alive")==0)
					request->SetKeepAlive(true);
				else if (header.compare("close")==0)
					request->SetKeepAlive(false);

			}

			AppLog(Logger::DEBUG,"Parsed request:\n" + request->ToString() );
			retVal=true;

		}
		else
			request->mParseState=0;
	}
	return retVal;
}

size_t Request::ParseRequestLine(Request* request, const char* data, size_t size)
{
	int state=0;
	char lookfor=' ';
	size_t oldpos=0;
	size_t position=0;
	Status status=HTTP_OK;
	request->SetStatus(status);
	for (size_t pos=0;pos<size;pos++)
	{
		if (data[pos]==lookfor)
		{
			if (state==0&&pos>0) // parse METHOD
			{
				std::string method(data,pos);
				if (pos==3&&method.compare(0,3,"GET")==0)
				{
					request->mType=Request::HTTP_GET;
				}
				else if (pos==4&&method.compare(0,4,"HEAD")==0)
				{
					request->mType=Request::HTTP_HEAD;
				}
				else
				{
					status=HTTP_NOT_IMPLEMENTED;
				}

				state=1;
				oldpos=pos;
			}
			else if (state==1) // parse URI
			{
				std::string uri(data+oldpos+1,pos-oldpos-1);
				request->mUri=uri;

				if (uri.length()>MAX_URI_LENGTH)
				{
					status=Http::HTTP_REQUEST_URI_TO_LONG;
				}

				lookfor='\r';
				state=2;
				oldpos=pos;
			}
			else if (state==2) // parse version
			{
				std::string version(data+oldpos+1,pos-oldpos-1);

				if (version.compare(0,8,"HTTP/1.1")==0)
				{
					request->SetHttpVersion(HTTP_VERSION_1_1);
				}
				else if (version.compare(0,8,"HTTP/1.0")==0)
				{
					request->SetHttpVersion(HTTP_VERSION_1_0);
				}
				else
				{
					status=Http::HTTP_REQUEST_VERSION_NOT_SUPPORTED;
				}

				lookfor='\n';
				state=3;
			}
			else if (state==3) // Done parsing request line
			{
				request->SetStatus(status);
				position=pos+1;
				break;
			}
		}

	}
	return position;
}

bool Request::ParseRequestHeaders(Request* request, const char* data, size_t size, size_t& position)
{
	if (size>position)
	{
		// Was request only a request line, ie. no headers?
		if (data[position]=='\r'&&data[position+1]=='\n')
		{
			position++;
			return true;
		}
	}

	char lookfor=':';
	int state=0;
	size_t oldpos=position-1;
	;
	std::string header;
	for (size_t pos=position;pos<size;pos++)
	{
		if (data[pos]==lookfor)
		{
			if (state==0)
			{
				header=std::string(data+oldpos+1,pos-oldpos-1);
				lookfor='\r';
				state=1;
				pos++;

				while (pos<size&&(data[pos]==' '||data[pos]=='\t'))
					pos++;

				oldpos=pos;
			}
			else if (state==1)
			{
				std::string value(data+oldpos,pos-oldpos);
				request->mHeader[header]=value;
				state=2;
				lookfor='\n';
			}
			else if (state==2)
			{
				if (size>pos+1)
				{
					if (data[pos+1]=='\r'&&data[pos+2]=='\n')
					{
						position=pos+2;
						return true;
					}
					else
					{
						state=0;
						lookfor=':';
						oldpos=pos;
					}
				}
				else
				{
					position=pos;
					return false;
				}
			}
		}
	}
	return false;
}
