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
	int readThrougput = 4096;
	// Max per iteration of data to send.. Should be ca 100kb.. this
		// This should be TrafficShaped to be throughput per second
	int writeThrougput = 4096;
	int count=0;
	std::list<Connection*>::iterator it;
	std::list<Connection*>::iterator end;
	Connection* con;
	const int timeout = 30 ;
	while (mKeepRunning || count > 0 )
	{
		time_t now = time(NULL);

		pthread_mutex_lock(mMutex);
		count = mList.size();
		it = mList.begin();
		end = mList.end();
		pthread_mutex_unlock(mMutex);


		for(; (count >0) && (it!=end) ; it++)
		{
			con = *it;
			if ( con->Read(readThrougput ) )
			{
				mRequestQueue->AddRequest(con->GetRequest());
				con->SetRequest(NULL);
				con->SetLastRequstTime(now);
			}

			if (con->HasData())
			{
				con->SetLastRequstTime(now);
				int ret = con->Write(writeThrougput);
				if (ret<0)
				{
					//std::cout << "Connection write error. Close\n";
					RemoveConnection(con);
					con=NULL;
					it = mList.erase(it);
				}
				else if (ret==1)
				{

					if (con->IsCloseable())
					{
						//std::cout << "Connection closing.. Done its work\n";
						RemoveConnection(con);
						con=NULL;
						it = mList.erase(it);
					} else
					{
					}

				}
			}

			if (con !=0 && con->GetLastRequstTime() + timeout < now)
			{
				std::cout << "Connection timeout\n";
				RemoveConnection(con);
				con=NULL;
				it = mList.erase(it);
			}

			if(!mKeepRunning && con->GetRequest()!= NULL)
			{
				RemoveConnection(con);
				con=NULL;
				it = mList.erase(it);
			}

		}
		usleep(10);
	}
	std::cout << "Connection timeout\n";
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
}
