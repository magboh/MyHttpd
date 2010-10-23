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
	mStats.nrTotalConnections = 0 ;
}

ConnectionManager::~ConnectionManager()
{

}


void ConnectionManager::CreateConnection(int socket)
{
	std::cout << "Created new connection\n";
	Connection* con= new Connection(socket,this);
	mWorker[mCurrentThread++ % mNrWorkers]->AddConnection(con);
	++mStats.nrTotalConnections;
}

void ConnectionManager::PrintStats()
{
	std:: cout << "---- ConnectionManager ----\n";
	std:: cout << "Total Connections: " <<  mStats.nrTotalConnections <<  "\n";
}


