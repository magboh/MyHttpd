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
class ByteBuffer;
class Site;

class Connection
{
public:
	enum Status_t {STATUS_OK,STATUS_ERROR,STATUS_AGAIN,STATUS_INTERUPT,STATUS_DONE,STATUS_CLOSE};
	Connection(int socket, const Site* site);
	virtual ~Connection();

	/**
	 * Read at most 'size' bytes from Connections's socket
	 * @param size amount of data to read
	 * @return
	 */
	Status_t Read(size_t size);

	/**
	 * Try to write at most 'size' bytes to socket.
	 * @param size
	 * @return 1 if all written 0 on more to write -1 if failure
	 */
	Status_t Write(size_t size);

	/**
	 * Parses data read from socket.
	 * @return true if Request ready to be processed. GetRequest() return != NULL
	 */
	bool Parse();

	/**
	 * Gets the socket for this connection
	 * @return valid socked, or -1 if closed
	 */
	int GetSocket() const;

	/**
	 *
	 * @return
	 */
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

	/**
	 * Connection has sent a Request. Waiting for answer
	 * @return
	 */
	bool HasDataPending() {
		return mPendingData;
	}

	void SetDataPending(bool pending)
	{
		mPendingData=pending;
	}

private:
	Status_t ErrnoToStatus(int error);

	int mSocket;

	ByteBuffer* mReadBuffer;
	ByteBuffer* mWriteBuffer;

	/**
	 * Needed while parsing Request
	 */
	Request* mRequest;
	const Response* mResponse;
	bool mHasData;
	char mWriteStatus;

	/**
	 * Nr bytes written to socket, for the current Response.
	 */
	size_t mWritten;

	/**
	 * time of connection create
	 */
	time_t mCreated;

	/**
	* time when a request was last recieved
	*/
	time_t mLastRequest;
	bool mCloseable;
	bool mPendingData;
	const Site* mSite;
};

#endif /* CONNECTION_H_ */
