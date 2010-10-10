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
class Connection;
class ByteBuffer;
class Response
{
public:
	static Response* CreateResponse(const Request* request);
	virtual ~Response();

	Http::Status GetStatus() const;
    void SetStatus(Http::Status mStatus);

    Http::Version GetHttpVersion() const;
    void SetHttpVersion(Http::Version mVersion);

    void SetContentLength(unsigned int length);
    unsigned int GetContentLength() const;

    int GetFile() const;
    void SetFile(int fd);

    bool GetKeepAlive() const;

    int ToBuffer(ByteBuffer* buffer) const;
private:
	Response();
	Http::Status mStatus ;
	unsigned int mContentLength;
	Http::Version mVersion;
	int mFile;
	Connection* mConnection;
	bool mKeepAlive;

};

#endif /* RESPONSE_H_ */
