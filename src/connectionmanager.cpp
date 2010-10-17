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

ConnectionManager::ConnectionManager(int maxConnections, int nrWorkers,ConnectionQueueWorker** workers)
{
	mNrWorkers = nrWorkers ;

	mMaxConnections = maxConnections;
	mNumConnections = 0;
	mCurrentThread = 0 ;
	mWorker = workers;
	mCurrentThread = 0 ;
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
