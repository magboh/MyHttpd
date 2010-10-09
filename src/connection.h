/*
 * connection.h
 *
 *  Created on: Sep 21, 2010
 *      Author: magnus
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

class Response;
class Request;
class ConnectionManager;
class ByteBuffer;
class Connection {
public:
	Connection(int socket,ConnectionManager* conectionMgr);
	virtual ~Connection();

	/**
	 *
	 * @param size amount of data to read from socket
	 * @return
	 */
	int Read(size_t size);
	void Write(const Response* response);
	void Write(size_t size);
	int GetSocket() const;
	bool WantToRead() const;
private:
	int mSocket;

	ByteBuffer* mReadBuffer;
	ByteBuffer* mWriteBuffer;

	ConnectionManager* mConnectionManager;
	Request* mRequest;
	bool mWantToRead;
};

#endif /* CONNECTION_H_ */
