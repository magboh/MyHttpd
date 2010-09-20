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

private:
	std::string mHost;
	std::string mUri;
	std::string mVersion;
	bool mKeepAlive;
	RequestType mType;
};

#endif /* REQUEST_H_ */
