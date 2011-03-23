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
class Site;

class Connection
{
public:
	Connection(int socket, ConnectionManager* conectionMgr, const Site& site);
	virtual ~Connection();

	/**
	 *
	 * @param size amount of data to read from socket
	 * @return
	 */
	bool Read(size_t size);
	/**
	 *
	 * @param size
	 * @return 1 if all written 0 on more to write -1 if failure
	 */
	int Write(size_t size);
	int GetSocket() const;
	bool WantToRead() const;
	bool HasData();
	void SetHasData(bool b);
	void SetResponse(const Response* response);
	Request* GetRequest() const;
	void SetRequest(Request* request);

	void SetLastRequstTime(time_t lastTime);
	time_t GetLastRequstTime() const;

	bool IsCloseable() const;
	void SetCloseable(bool closeable);

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
	time_t mCreated;
	time_t mLastRequest;
	bool mCloseable;
	const Site& mSite;
};

#endif /* CONNECTION_H_ */
