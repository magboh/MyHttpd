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
class Response : public Http
{
public:
	static Response* CreateResponse(const Request* request);
	virtual ~Response();

    void SetContentLength(unsigned int length);
    unsigned int GetContentLength() const;

    int GetFile() const;
    void SetFile(int fd);

    bool GetKeepAlive() const;

    int ToBuffer(ByteBuffer* buffer) const;
private:
	Response();
	unsigned int mContentLength;
	int mFile;
	Connection* mConnection;
	bool mKeepAlive;

};

#endif /* RESPONSE_H_ */
