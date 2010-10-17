/*
 * connectionqueueworker.h
 *
 *  Created on: Oct 7, 2010
 *      Author: magnus
 */

#ifndef CONNECTIONQUEUEWORKER_H_
#define CONNECTIONQUEUEWORKER_H_
#include <list>
class RequestQueue;
class Connection;
class ConnectionQueueWorker
{
public:
	ConnectionQueueWorker(RequestQueue *requestWorker);

	void AddConnection(Connection* con);
	virtual ~ConnectionQueueWorker();
	bool Start();
	void Stop();
private:
	void Work();

	void RemoveConnection(Connection* con);
	static void* ThreadCallBack(void* arg);
	pthread_t* mThread;
	std::list <Connection*> mList;
	pthread_mutex_t* mMutex;
	bool mKeepRunning;
	RequestQueue* mRequestQueue;
};

#endif /* CONNECTIONQUEUEWORKER_H_ */
