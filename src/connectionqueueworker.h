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

	struct {
		Connection* con;
		epoll_event event;
	} EpollData_t;

	virtual void DoWork();

	void RemoveConnection(Connection* con);
	void PollAdd(int socket);
	void PollDel(int socket);
	std::list <Connection*> mList;
	std::map <int, Connection*> mConMap;

	pthread_mutex_t* mMutex;
	bool mKeepRunning;
	RequestQueue* mRequestQueue;
	int mEpollSocket;
	std::list <EpollData_t> mEpollData;
};

#endif /* CONNECTIONQUEUEWORKER_H_ */
