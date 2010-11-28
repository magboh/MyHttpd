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
#include "site.h"

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


void ConnectionManager::CreateConnection(int socket,const Site *site)
{
	std::cout << "Created new connection\n";
	Connection* con= new Connection(socket,this,site);
	mWorker[mCurrentThread++ % mNrWorkers]->AddConnection(con);
	++mStats.nrTotalConnections;
}

void ConnectionManager::PrintStats()
{
	std:: cout << "---- ConnectionManager ----\n";
	std:: cout << "Total Connections: " <<  mStats.nrTotalConnections <<  "\n";
}


