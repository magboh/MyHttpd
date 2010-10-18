/*
 * connectionqueueworker.h
 *
 *  Created on: Oct 7, 2010
 *      Author: magnus
 */

#ifndef CONNECTIONQUEUEWORKER_H_
#define CONNECTIONQUEUEWORKER_H_

#include <list>

#include "thread.h"

class RequestQueue;
class Connection;

class ConnectionQueueWorker : public Thread
{
public:
	ConnectionQueueWorker(RequestQueue *requestWorker);

	void AddConnection(Connection* con);
	virtual ~ConnectionQueueWorker();
	void Stop();
private:
	virtual void DoWork();

	void RemoveConnection(Connection* con);

	std::list <Connection*> mList;
	pthread_mutex_t* mMutex;
	bool mKeepRunning;
	RequestQueue* mRequestQueue;
};

#endif /* CONNECTIONQUEUEWORKER_H_ */
