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

#ifndef REQUEST_H_
#define REQUEST_H_

#include <string>
#include <map>

#include "http.h"

class Connection;
class ByteBuffer;
class Site;

class Request:public Http
{
public:
	Request(Connection* connection, const Site* site);
	virtual ~Request();
	enum ParseReturn
	{
		REQUEST_OK, REQUEST_BAD, REQUEST_UNFINISHED, REQUEST_HTPP_VERSION_NOT_SUPPORTED, REQUEST_TO_LARGE, REQUEST_URI_TO_LONG
	};
	static bool ParseRequest(Request* request, ByteBuffer* buffer);
	enum RequestType
	{
		HTTP_UNDEF=0,HTTP_GET, HTTP_POST, HTTP_HEAD
	};

	const std::string ToString() const;
	const std::string & GetUri() const;
	RequestType GetHttpType() const;

	const Site* GetSite() const;

	Connection *GetConnection() const;
	void SetConnection(Connection *mConnection);
private:
	Request(const Request &);
	Request& operator=(const Request& rhs);  // No implementation

	enum
	{
		MAX_URI_LENGTH=2048
	};
	const Site* mSite;

	std::string mHost;
	std::string mUri;

	RequestType mType;
	Connection* mConnection;
	int mParseState;
	std::map<std::string, std::string> mHeader;

	static size_t ParseRequestLine(Request* request, const char* data, size_t size);
	static bool ParseRequestHeaders(Request* request, const char* data, size_t size,size_t& position);
};

#endif /* REQUEST_H_ */
