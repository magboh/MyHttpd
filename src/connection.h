/*
 * connection.h
 *
 *  Created on: Sep 21, 2010
 *      Author: magnus
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

class Response;
class Connection {
public:
	Connection(int socket);
	virtual ~Connection();
	/**
	 * Read reads what is on the socket..
	 */
	void Read();
	void Write(Response* response);
	void Write();
	int GetSocket() const;
private:
	int mSocket;

	unsigned char *mReadBuffer;
	unsigned int mReadBufferSize;

	unsigned char *mWriteBuffer;
	unsigned int mWriteBufferSize;
	unsigned mToWrite;
};

#endif /* CONNECTION_H_ */
