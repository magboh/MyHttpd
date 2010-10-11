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
	bool Read(size_t size);
	bool Write(size_t size);
	int GetSocket() const;
	bool WantToRead() const;
	bool HasData();
	void SetHasData(bool b);
	void SetResponse(const Response* response);
private:
	int mSocket;

	ByteBuffer* mReadBuffer;
	ByteBuffer* mWriteBuffer;

	ConnectionManager* mConnectionManager;
	Request* mRequest;
	bool mWantToRead;

	const Response* mResponse;
	bool mHasData;
	char mWriteStatus;
	size_t mWritten;
};

#endif /* CONNECTION_H_ */
