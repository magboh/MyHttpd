/*
 * ioworker.h
 *
 *  Created on: Mar 20, 2011
 *      Author: magnus
 */

#ifndef IOWORKER_H_
#define IOWORKER_H_

#include "thread.h"
class Connection;
class ConnectionManager;

/*!
 * The IoWorker class builds up Fd sets for epoll()
 *
 */
class IoWorker : public Thread
{
public:
	IoWorker(ConnectionManager& mConnectionManager);
	virtual ~IoWorker();
	virtual void DoWork();
	void AddConnection(Connection* con);
private:
	int mPollSocket;
	ConnectionManager& mConnectionManager;
};

#endif /* IOWORKER_H_ */
