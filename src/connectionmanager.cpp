/***************************************************************************
 *      MyHTTPd
 *
 *      Tue, 15 Mar 2011 22:16:12 +0100
 *      Copyright 2011 Magnus Bohman
 *      magnus.bohman@gmail.com
 ***************************************************************************/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, US.
 */

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
	//std::cout << "Created new connection\n";
	Connection* con= new Connection(socket,this,site);
	mWorker[mCurrentThread++ % mNrWorkers]->AddConnection(con);
	++mStats.nrTotalConnections;
}

void ConnectionManager::PrintStats()
{
	std:: cout << "---- ConnectionManager ----\n";
	std:: cout << "Total Connections: " <<  mStats.nrTotalConnections <<  "\n";
}


