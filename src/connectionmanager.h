/*
 * connectionmanager.h
 *
 *  Created on: Sep 20, 2010
 *      Author: magnus
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_


#include <list>

class Connection;
struct pollfd;

class ConnectionManager {
public:
	ConnectionManager(int maxConnections);
	virtual ~ConnectionManager();
	void CreateConnection(int socket);
	void StartHandleConnections();
	Connection* GetWriteConnection();
	Connection* GetReadConnection();
private:
	static void* ThreadCallBack(void* arg);
	void HandleConnections();

	int mNumConnections;
	int mMaxConnections;
	Connection** mConnections;
	pollfd* mFds;
	pthread_mutex_t* mMutex;
	pthread_mutex_t* mReadMutex;
	pthread_mutex_t* mWriteMutex;

	pthread_cond_t* mCondReadThread;
	pthread_cond_t* mCondWriteThread;

	std::list <Connection*>** 	mReadList;
	std::list <Connection*>** 	mWriteList;
	int mNrWorkers;
	int mCurrentThread;
};

#endif /* CONNECTIONMANAGER_H_ */
