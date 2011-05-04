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
	 *
	 * @param size amount of data to read from socket
	 * @return
	 */
	Status_t Read(size_t size);
	/**
	 *
	 * @param size
	 * @return 1 if all written 0 on more to write -1 if failure
	 */
	Status_t Write(size_t size);

	/**
	 * Parses data stored in mReadBuffer. Creates mRequests is needed
	 * @return true if request ready to be processed
	 */
	bool Parse();
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

	Request* mRequest;
	bool mWantToRead;
	const Response* mResponse;
	bool mHasData;
	char mWriteStatus;
	size_t mWritten;
	time_t mCreated;
	time_t mLastRequest;
	bool mCloseable;
	bool mPendingData;
	const Site* mSite;
	unsigned char mTreadNr;
};

#endif /* CONNECTION_H_ */
