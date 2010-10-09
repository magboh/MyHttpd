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
	enum Type {Reader=0,Writer};
	ConnectionQueueWorker(ConnectionManager* conMgr , std::list <Connection*>* list, Type type);
	virtual ~ConnectionQueueWorker();
	bool Start();
private:
	void Work();
	static void* ThreadCallBack(void* arg);
	void HandleRead();
	void HandleWrite();

	bool mKeepRunning;
	pthread_t* mThread;
	Type mType;
	ConnectionManager* mConnectionManager;
	std::list <Connection*>* mList;
};

#endif /* CONNECTIONQUEUEWORKER_H_ */
