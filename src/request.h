/*
 * request.h
 *
 *  Created on: Sep 17, 2010
 *      Author: magnus
 */

#ifndef REQUEST_H_
#define REQUEST_H_
#include <string>

class Request {
public:
	Request();
	virtual ~Request();

	static Request* ParseRequest(unsigned char* data, size_t size);
	enum RequestType {HTTP_GET,HTTP_POST};
	enum RequestVersion {HTTP_VERSION_1_0,HTTP_VERSION_1_1};
	const std::string & ToString() const;
	const std::string & GetUri() const;
	RequestVersion GetHttpVersion() const;
	RequestType GetHttpType() const;

private:
	enum {MAX_URI_LENGTH=2048};
	std::string mHost;
	std::string mUri;
	RequestVersion mVersion;

	bool mKeepAlive;
	RequestType mType;
};

#endif /* REQUEST_H_ */
