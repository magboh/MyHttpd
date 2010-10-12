/*
 * connectionmanager.h
 *
 *  Created on: Sep 20, 2010
 *      Author: magnus
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_


#include <list>

class ConnectionQueueWorker;
class ConnectionManager
{
public:
	ConnectionManager(int maxConnections);
	virtual ~ConnectionManager();
	void CreateConnection(int socket);
	void StartHandleConnections();
private:
	static void* ThreadCallBack(void* arg);
	void HandleConnections();

	int mNumConnections;
	int mMaxConnections;

	int mNrWorkers;
	int mCurrentThread;
	ConnectionQueueWorker** mWorker;
};

#endif /* CONNECTIONMANAGER_H_ */
