/*
 * connectionqueueworker.h
 *
 *  Created on: Oct 7, 2010
 *      Author: magnus
 */

#ifndef CONNECTIONQUEUEWORKER_H_
#define CONNECTIONQUEUEWORKER_H_
#include <list>
class Connection;
class ConnectionManager;

class ConnectionQueueWorker
{
public:
	ConnectionQueueWorker(ConnectionManager* conMgr);

	void AddConnection(Connection* con);
	virtual ~ConnectionQueueWorker();
	bool Start();
	bool Stop();
private:
	void Work();

	void RemoveConnection(Connection* con);
	static void* ThreadCallBack(void* arg);
	pthread_t* mThread;
	ConnectionManager* mConnectionManager;
	std::list <Connection*> mList;
	pthread_mutex_t* mMutex;
	bool mKeepRunning;
};

#endif /* CONNECTIONQUEUEWORKER_H_ */
