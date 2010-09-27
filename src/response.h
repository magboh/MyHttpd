/*
 * response.h
 *
 *  Created on: Sep 26, 2010
 *      Author: magnus
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include "http.h"

class Request;

class Response
{
public:
	enum StatusType {HTTP_OK=200,HTTP_NO_ACCESS=403, HTTP_NOT_FOUND=404, HTTP_INTERNAL_SERVER_ERROR=500};
	static Response* CreateResponse(const Request* request);
	virtual ~Response();

	StatusType GetStatus() const;
    void SetStatus(StatusType mStatus);

    Http::Version GetHttpVersion() const;
    void SetHttpVersion(Http::Version mVersion);

    void SetContentLength(unsigned int length);
    unsigned int GetContentLength() const;
    const unsigned char* ToBuffer();

    int GetFile();
    void SetFile(int fd);
private:
	Response();
	StatusType mStatus ;
	unsigned int mContentLength;
	Http::Version mVersion;
	int mFile;
};

#endif /* RESPONSE_H_ */
