/*
 * connection.h
 *
 *  Created on: Sep 21, 2010
 *      Author: magnus
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

class Connection {
public:
	Connection(int socket);
	virtual ~Connection();
	/**
	 * Read reads what is on the socket..
	 */
	void Read();
	int GetSocket() const;
private:
	int mSocket;
	unsigned char *mBuffer;
	unsigned int mBufferSize;
};

#endif /* CONNECTION_H_ */
