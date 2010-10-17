/*
 * connectionqueueworker.cpp
 *
 *  Created on: Oct 7, 2010
 *      Author: magnus
 */

#include <pthread.h>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <list>
#include <cassert>

#include "connection.h"
#include "connectionmanager.h"
#include "connectionqueueworker.h"

ConnectionQueueWorker::	ConnectionQueueWorker(ConnectionManager* conMgr)
{
	mThread = new pthread_t;
	mKeepRunning = true;
	mConnectionManager = conMgr;

	mMutex = new pthread_mutex_t;
	pthread_mutex_init(mMutex, NULL);

	mKeepRunning = false;
}

ConnectionQueueWorker::~ConnectionQueueWorker()
{
	delete mThread;
	mThread = NULL;
}

void ConnectionQueueWorker::RemoveConnection(Connection *con)
{
	assert(con);
	close( con->GetSocket() );
	delete con;

}

void* ConnectionQueueWorker::ThreadCallBack(void* arg)
{
	ConnectionQueueWorker* worker = (ConnectionQueueWorker*) arg;
	worker->Work();
	return (NULL);
}

bool ConnectionQueueWorker::Start()
{
	mKeepRunning = true;
	bool ok = (pthread_create(mThread,NULL, ConnectionQueueWorker::ThreadCallBack,(void*)this)==0);
	return ok ;
}


void ConnectionQueueWorker::Work()
{
	// Max per iteration of data to send.. Should be ca 100kb.. this
	// This should be TrafficShaped to be throughput per second
	int readThrougput = 512000;
	// Max per iteration of data to send.. Should be ca 100kb.. this
		// This should be TrafficShaped to be throughput per second
	int writeThrougput = 512000;
	int count=0;
	std::list<Connection*>::iterator it;
	Connection* con;
	assert(mKeepRunning);
	while (mKeepRunning || count > 0 )
	{
		count = mList.size();
		for(it = mList.begin() ; count >0 &&   it != mList.end() ; it++)
		{
			con = *it;
			if(mKeepRunning)
				con->Read(readThrougput / count);

			if (con->HasData())
			{
				if (!con->Write(writeThrougput / count))
				{
					RemoveConnection(con);
					con=NULL;
					it = mList.erase(it);
				}
			}

		}
		usleep(10);
	}

}

void ConnectionQueueWorker::AddConnection(Connection* con)
{
	pthread_mutex_lock(mMutex);
	mList.push_back(con);
	pthread_mutex_unlock(mMutex);
}

bool ConnectionQueueWorker::Stop()
{
	mKeepRunning = false;
	return true;
}
