/*
 * request.h
 *
 *  Created on: Sep 17, 2010
 *      Author: magnus
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include <string>
#include "http.h"

class Connection;
class ByteBuffer;

class Request {
public:
	Request(Connection* connection);
	virtual ~Request();
	enum ParseReturn {
		REQUEST_OK,
		REQUEST_BAD,
		REQUEST_UNFINISHED,
		REQUEST_HTPP_VERSION_NOT_SUPPORTED,
		REQUEST_TO_LARGE,
		REQUEST_URI_TO_LONG,
		};
	static Request::ParseReturn ParseRequest(Request* request,ByteBuffer* buffer);
	enum RequestType {HTTP_GET,HTTP_POST};

	const std::string ToString() const;
	const std::string & GetUri() const;
	Http::Version GetHttpVersion() const;
	RequestType GetHttpType() const;

	Connection *GetConnection() const;
    void SetConnection(Connection *mConnection);
    bool GetKeepAlive() const;

private:
	enum {MAX_URI_LENGTH=2048};
	std::string mHost;
	std::string mUri;
	Http::Version mVersion;

	bool mKeepAlive;
	RequestType mType;
	Connection* mConnection;
	int mParseState;
};

#endif /* REQUEST_H_ */
