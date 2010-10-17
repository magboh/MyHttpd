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
#include "connectionqueueworker.h"
#include "requestqueue.h"

ConnectionQueueWorker::	ConnectionQueueWorker(RequestQueue* reqQueue)
{

	mKeepRunning = true;
	mRequestQueue = reqQueue;
	mMutex = new pthread_mutex_t;
	pthread_mutex_init(mMutex, NULL);

	mKeepRunning = true;
}

ConnectionQueueWorker::~ConnectionQueueWorker()
{
}

void ConnectionQueueWorker::RemoveConnection(Connection *con)
{
	assert(con);
	close( con->GetSocket() );
	delete con;

}

void ConnectionQueueWorker::DoWork()
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

	while (mKeepRunning || count > 0 )
	{
		count = mList.size();
		for(it = mList.begin() ; count >0 &&   it != mList.end() ; it++)
		{
			con = *it;
			if(mKeepRunning)
			{
				if ( con->Read(readThrougput / count) )
				{
					mRequestQueue->AddRequest(con->GetRequest());
					con->SetRequest(NULL);
				}

			}
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

void ConnectionQueueWorker::Stop()
{
	mKeepRunning = false;
	return true;
}
