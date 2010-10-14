/*
 * connectionmanager.cpp
 *
 *  Created on: Sep 20, 2010
 *      Author: magnus
 */
//#include <sys/types.h>          /* See NOTES */
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <stdio.h>

#include <cassert>
#include <iostream>
#include <list>

#include "connection.h"
#include "connectionmanager.h"
#include "connectionqueueworker.h"

ConnectionManager::ConnectionManager(int maxConnections)
{
	mNrWorkers = 2 ;

	mMaxConnections = maxConnections;
	mNumConnections = 0;
	mCurrentThread = 0 ;

	typedef ConnectionQueueWorker cqwp;
	mWorker = new ConnectionQueueWorker*[2];

	for(int i=0; i<mNrWorkers;i++)
	{
		mWorker[i]= new ConnectionQueueWorker(this);
		mWorker[i]->Start();
	}


}

ConnectionManager::~ConnectionManager()
{

	for(int i=0; i<mNrWorkers;i++)
	{
		delete mWorker[i];
	}

	delete mWorker;
}


void ConnectionManager::CreateConnection(int socket)
{

	Connection* con= new Connection(socket,this);
	mWorker[mCurrentThread++ % mNrWorkers]->AddConnection(con);
}

void ConnectionManager::Stop()
{
	for(int i=0; i<mNrWorkers;i++)
	{
		mWorker[i]->Stop();
	}

}
