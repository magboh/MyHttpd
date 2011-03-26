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
	mType=HTTP_GET;
	mKeepAlive=true;
	mConnection=connection;
	mParseState=0;
	mParsePos=0;
}

Request::~Request()
{
	// TODO Auto-generated destructor stub
}

/**
 *
 * @param data
 * @param size
 * @return
 */
bool Request::ParseRequest(Request* request, ByteBuffer* buffer)
{
	unsigned int parsePos=request->mParsePos;
	// ParseState == 0 is GET/POST line
	const char* data=buffer->GetBuffer();
	size_t size=buffer->GetUsage();
	if (size<18)
		return false;
	//	std::cout << "parsePos=" << parsePos << " size=" << size <<"\n";
	if (request->mParseState==0)
	{
		Http::Version version=Http::HTTP_VERSION_1_1;
		RequestType rt=Request::HTTP_GET;

		if (strncmp(data,"GET",3)==0)
		{
			rt=Request::HTTP_GET;
			parsePos+=3;
			//			std::cout << "ParsePos=" << parsePos << "GET\n";
		}
		else if (strncmp(data,"POST",4)==0)
		{
			rt=Request::HTTP_POST;
			parsePos+=4;
		}

		assert(data[parsePos]==' ');
		parsePos++;

		unsigned int i=parsePos;

		for (;i<size;i++)
		{
			if (data[i]==' ')
				break;
		}

		if (i-parsePos>Request::MAX_URI_LENGTH)
		{
			request->SetStatus(Http::HTTP_REQUEST_URI_TO_LONG);
			return true;
		}

		std::string uri=std::string(data+parsePos,i-parsePos);
		parsePos+=i-parsePos;

		if (size>parsePos+9)
		{
			assert(data[parsePos]==' ');
			parsePos++;
			if (strncmp("HTTP/1.1",data+parsePos,8)==0)
			{
				version=Http::HTTP_VERSION_1_1;
				parsePos+=8;
			}
			else if (strncmp("HTTP/1.0",data+parsePos,8)==0)
			{
				version=Http::HTTP_VERSION_1_0;
				parsePos+=8;
			}
			else
			{
				request->SetStatus(Http::HTTP_REQUEST_VERSION_NOT_SUPPORTED);
				return true;
			}

		}
		else
			return false;

		if (size>parsePos+2)
		{
			if (*(data+parsePos)=='\r')
				parsePos++;

			if (*(data+parsePos)=='\n')
			{
				parsePos++;

				request->mParseState=1;
				request->mType=rt;
				request->SetHttpVersion(version);
				request->mUri=uri;
				request->mParsePos=parsePos;
			}
		}
	}

	if (request->mParseState==1)
	{
		int headStart=parsePos;
		int headEnd=parsePos;
		for (unsigned int i=parsePos;i<=size-4;i++)
		{

			if (strncmp("\r\n",(data+i),2)==0)
			{
				request->mHeader[std::string(data+headStart,headEnd-headStart-1)]=std::string(data+headEnd+1,i-headEnd-1);
				headStart=i+2;
				headEnd=headStart;
			}

			if (strncmp("\r\n\r\n",(data+i),4)==0)
			{
				request->mParsePos=0;
				request->SetStatus(Http::HTTP_OK);
				buffer->Remove(i+4);
				request->mKeepAlive=false;
				if (request->mHeader["Connection"].compare("keep-alive")==0)
					request->mKeepAlive=true;
				else if (request->mHeader["Connection"].compare("close")==0)
					request->mKeepAlive=false;

				AppLog(Logger::DEBUG,"Parsed request:" + request->ToString() );

				return true;
			}

			if (data[i]==' '&&headEnd==headStart)
				headEnd=i;
		}
	}
	return false;

}

const std::string Request::ToString() const
{
	std::string str=(mType==HTTP_GET) ? "GET" : "POST";
	str+=" "+mUri+" ";
	str+=Http::GetVersionString(GetHttpVersion());
	str+=" "+Http::GetStatusString(GetStatus())+"\n";

	std::map<std::string, std::string>::const_iterator i=mHeader.begin();

	for (;i!=mHeader.end();++i)
	{
		str+=":"+i->first+":"+i->second+":::\n";
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

bool Request::GetKeepAlive() const
{
	return mKeepAlive;
}

const Site& Request::GetSite() const
{
	return mSite;
}
