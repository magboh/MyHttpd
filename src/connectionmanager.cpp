/*
 * connectionmanager.cpp
 *
 *  Created on: Sep 20, 2010
 *      Author: magnus
 */
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <poll.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <list>

#include "connection.h"
#include "connectionmanager.h"
#include "requestqueue.h"
#include "connectionqueueworker.h"

ConnectionManager::ConnectionManager(int maxConnections)
{
	mNrWorkers = 1 ;


	mMutex = new pthread_mutex_t;
	pthread_mutex_init(mMutex, NULL);

	mReadMutex = new pthread_mutex_t;
	pthread_mutex_init(mReadMutex, NULL);

	mWriteMutex = new pthread_mutex_t;
	pthread_mutex_init(mWriteMutex, NULL);

	mCondReadThread = new pthread_cond_t;
	pthread_cond_init (mCondReadThread , NULL);

	mCondWriteThread = new pthread_cond_t;
	pthread_cond_init (mCondWriteThread , NULL);

	mMaxConnections = maxConnections;
	mNumConnections = 0;
	mCurrentThread = 0 ;

	typedef  std::list <Connection*> conque;

	mReadList = new conque*[mNrWorkers];
	mWriteList= new conque*[mNrWorkers];

	for(int i=0; i<mNrWorkers;i++)
	{
		mReadList[i] = new conque;
		mWriteList[i] = new conque;

		ConnectionQueueWorker* worker1 = new ConnectionQueueWorker(this, mReadList[i],  ConnectionQueueWorker::Reader);
		ConnectionQueueWorker* worker2 = new ConnectionQueueWorker(this, mWriteList[i], ConnectionQueueWorker::Writer);

		worker1->Start();
		worker2->Start();

	}


}

ConnectionManager::~ConnectionManager()
{
	// TODO Auto-generated destructor stub
}


void ConnectionManager::CreateConnection(int socket)
{

	Connection* con= new Connection(socket,this);
	mReadList[mCurrentThread % mNrWorkers]->push_back(con);
	mWriteList[mCurrentThread++ % mNrWorkers]->push_back(con);
}


void ConnectionManager::HandleConnections()
{
/*	nfds_t nfds = mMaxConnections ;
	while(1)
	{
		int ret = poll(mFds,nfds,100);

		if (ret<0)
		{
			perror("poll failed");
		}
		if (ret>0)
		{
			int handled=0;
			for (int index=0 ; (index<mMaxConnections) && (handled < ret) ; index++, handled++)
			{

				pollfd* fds = &mFds[index];
				Connection* con = mConnections[index];
				if (con == NULL || fds == NULL)
					continue;
				if (fds->revents  & (POLLERR | POLLNVAL | POLLHUP))
				{
*/					// Some kind of error occured, or connection closed
//					printf("Connection closing from Poll() socket=%d",fds->fd);

/*					continue;
				}

				if (fds->revents & POLLIN)
				{
					if (con->Read()<=0)
					{
						continue;
					}
				}
*/
			/*	if (fds->revents & POLLOUT)
				{
					con->Write();
				}
*/
/*			}
		}
	}*/
}

void ConnectionManager::StartHandleConnections()
{
	static pthread_t mThread;
	if (pthread_create(&mThread,NULL, ConnectionManager::ThreadCallBack,(void*)this)!=0)
	{
		perror("pthread_create");
	}

}


void* ConnectionManager::ThreadCallBack(void* arg)
{
	ConnectionManager* connectionManager = (ConnectionManager*) arg;
	connectionManager->HandleConnections();
	return NULL;
}
