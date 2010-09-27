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

	StatusType getStatus() const;
    void setStatus(StatusType mStatus);

    Http::Version getVersion() const;
    void setVersion(Http::Version mVersion);

    void setContentLength(unsigned int mContentLength);
    unsigned int getContentLength() const;
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
