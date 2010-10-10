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

#include "connection.h"
#include "connectionmanager.h"
#include "connectionqueueworker.h"

ConnectionQueueWorker::	ConnectionQueueWorker(ConnectionManager* conMgr , std::list <Connection*>* list, Type type)
{
	mType = type;
	mThread = new pthread_t;
	mKeepRunning = true;
	mConnectionManager = conMgr;
	mList= list;
}

ConnectionQueueWorker::~ConnectionQueueWorker()
{
	delete mThread;
	mThread = NULL;
}

void* ConnectionQueueWorker::ThreadCallBack(void* arg)
{
	ConnectionQueueWorker* worker = (ConnectionQueueWorker*) arg;

	switch (worker->mType)
	{
	case ConnectionQueueWorker::Reader:
		worker->HandleRead();
		break;
	case ConnectionQueueWorker::Writer:
		worker->HandleWrite();
		break;
	}

	return (NULL);
}

bool ConnectionQueueWorker::Start()
{
	bool ok = (pthread_create(mThread,NULL, ConnectionQueueWorker::ThreadCallBack,(void*)this)==0);
	return ok ;
}


void ConnectionQueueWorker::HandleRead()
{
	// Max per iteration of data to send.. Should be ca 100kb.. this
	// This should be TrafficShaped to be throughput per second
	int readThrougput = 512000;

	while (1)
	{
		std::list<Connection*>::iterator it;
		int count = mList->size();

		for(it = mList->begin() ; count >0 &&   it != mList->end() ; it++)
		{
			Connection* con = *it;
			con->Read(readThrougput / count);
			usleep(100);
		}

	}

}

void ConnectionQueueWorker::HandleWrite()
{

	// Max per iteration of data to send.. Should be ca 100kb.. this
		// This should be TrafficShaped to be throughput per second
		int writeThrougput = 512000;

		while (1)
		{
			std::list<Connection*>::iterator it;
			int count = mList->size();

			for(it = mList->begin() ; it != mList->end() ; it++)
			{
				Connection* con = *it;
				if (con->HasData())
					con->Write(writeThrougput / count);

				usleep(100);
			}

		}


}

