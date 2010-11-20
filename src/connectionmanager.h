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
	ConnectionManager(int maxConnections, int nrWorkers,ConnectionQueueWorker** workers);
	virtual ~ConnectionManager();
	void CreateConnection(int socket);
	void PrintStats();
private:
	static void* ThreadCallBack(void* arg);

	int mNumConnections;
	int mMaxConnections;

	ConnectionQueueWorker** mConnectionWorker;

	int mNrWorkers;
	int mCurrentThread;
	ConnectionQueueWorker** mWorker;

	struct stats_t {
		unsigned int nrTotalConnections;
	} mStats;
};

#endif /* CONNECTIONMANAGER_H_ */
