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
	Request(Connection* connection, const Site& site);
	virtual ~Request();
	enum ParseReturn
	{
		REQUEST_OK, REQUEST_BAD, REQUEST_UNFINISHED, REQUEST_HTPP_VERSION_NOT_SUPPORTED, REQUEST_TO_LARGE, REQUEST_URI_TO_LONG,
	};
	static bool ParseRequest(Request* request, ByteBuffer* buffer);
	enum RequestType
	{
		HTTP_GET, HTTP_POST
	};

	const std::string ToString() const;
	const std::string & GetUri() const;
	RequestType GetHttpType() const;

	const Site& GetSite() const;

	Connection *GetConnection() const;
	void SetConnection(Connection *mConnection);
	bool GetKeepAlive() const;

private:
	Request(const Request &);
	enum
	{
		MAX_URI_LENGTH=2048
	};
	std::string mHost;
	std::string mUri;

	bool mKeepAlive;
	RequestType mType;
	Connection* mConnection;
	int mParseState;
	size_t mParsePos;
	std::map<std::string, std::string> mHeader;
	const Site& mSite;
};

#endif /* REQUEST_H_ */
