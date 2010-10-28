/*
 * serveroptions.h
 *
 *  Created on: Oct 28, 2010
 *      Author: magnus
 */

#ifndef SERVEROPTIONS_H_
#define SERVEROPTIONS_H_

class ServerOptions
{
public:
	ServerOptions();
	virtual ~ServerOptions();
    int GetConnectionQueueSize() const;
    int GetNoIOWorkers() const;
    int GetNoRequstWorkers() const;
    int GetRequestBufferSize() const;
    int GetResponseBufferSize() const;
    void SetConnectionQueueSize(int connectionQueueSize);
    void SetNoIOWorkers(int noIOWorkers);
    void SetNoRequstWorkers(int noRequstWorkers);
    void SetRequestBufferSize(int requestBufferSize);
    void SetResponseBufferSize(int responseBufferSize);

private:
	int mNoRequstWorkers;
	int mNoIOWorkers;
	int mResponseBufferSize;
	int mRequestBufferSize;
	int mConnectionQueueSize;
};

#endif /* SERVEROPTIONS_H_ */
