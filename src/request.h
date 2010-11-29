/*
 * request.h
 *
 *  Created on: Sep 17, 2010
 *      Author: magnus
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include <string>
#include <map>

#include "http.h"

class Connection;
class ByteBuffer;
class Site;

class Request : public Http {
public:
	Request(Connection* connection,const Site* site);
	virtual ~Request();
	enum ParseReturn {
		REQUEST_OK,
		REQUEST_BAD,
		REQUEST_UNFINISHED,
		REQUEST_HTPP_VERSION_NOT_SUPPORTED,
		REQUEST_TO_LARGE,
		REQUEST_URI_TO_LONG,
		};
	static bool ParseRequest(Request* request,ByteBuffer* buffer);
	enum RequestType {HTTP_GET,HTTP_POST};

	const std::string ToString() const;
	const std::string & GetUri() const;
	RequestType GetHttpType() const;

	const Site* GetSite() const;


	Connection *GetConnection() const;
    void SetConnection(Connection *mConnection);
    bool GetKeepAlive() const;

private:
	enum {MAX_URI_LENGTH=2048};
	std::string mHost;
	std::string mUri;


	bool mKeepAlive;
	RequestType mType;
	Connection* mConnection;
	int mParseState;
	size_t mParsePos;
	std::map<std::string , std::string> mHeader;
	const Site* mSite;
};

#endif /* REQUEST_H_ */
