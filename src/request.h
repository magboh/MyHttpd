/*
 * request.h
 *
 *  Created on: Sep 17, 2010
 *      Author: magnus
 */

#ifndef REQUEST_H_
#define REQUEST_H_
#include <string>

enum RequestType {HTTP_GET,HTTP_POST}
class Request {
public:
	Request();
	virtual ~Request();

	static Request* ParseRequest(unsigned char* data, size_t size);

private:
	std::string mHost;
	std::string mUri;
	std::string mVersion;
	bool mKeepAlive;
	RequestType type;
};a

#endif /* REQUEST_H_ */
