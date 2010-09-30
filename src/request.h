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

class Request {
public:
	Request(Connection* connection);
	virtual ~Request();

	static Request* ParseRequest(unsigned char* data, size_t size, Connection* connection);
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
};

#endif /* REQUEST_H_ */
