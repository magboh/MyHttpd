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
#include "requestqueue.h"
#include "ioworker.h"
#include "logger.h"

ConnectionManager::ConnectionManager(int maxConnections, RequestQueue* requestQueue)
{
	mMaxConnections=maxConnections;
	mNumConnections=0;
	mCurrentThread=0;
	mStats.nrTotalConnections=0;
	mRequestQueue=requestQueue;
}

ConnectionManager::~ConnectionManager()
{

}

void ConnectionManager::CreateConnection(int socket, const Site& site)
{
	mIoWorker->AddConnection(new Connection(socket, this, site));
}

void ConnectionManager::HandleConnection(Connection* con)
{
	mWorkerVector[mCurrentThread++%mWorkerVector.size()]->HandleConnection(con);
	++mStats.nrTotalConnections;
}

void ConnectionManager::PrintStats()
{
	std::cout<<"---- ConnectionManager ----\n";
	std::cout<<"Total Connections: "<<mStats.nrTotalConnections<<"\n";
}

void ConnectionManager::AddConnectionWorker(int nr)
{
	for (int i=0; i<nr; i++ )
	{
		ConnectionQueueWorker* cqw=new ConnectionQueueWorker(mRequestQueue);
		cqw->Start();
		mWorkerVector.push_back(cqw);
		AppLog(Logger::DEBUG,"Connection worker added");
	}
}
void ConnectionManager::AddIoWorker(int nr)
{
	for (int i=0; i<nr; i++ )
	{
		mIoWorker = new IoWorker(*this);
		mIoWorker->Start();
		AppLog(Logger::DEBUG,"IO worker added");
	}
}

void AddIoWorker(int nr = 1);

void ConnectionManager::ShutdownWorkers()
{
	AppLog(Logger::DEBUG,"ConnectionManager Shutdown workers");

	for (unsigned int i=0; i<mWorkerVector.size(); i++ )
	{
		mWorkerVector[i]->Stop();
	}
}

void ConnectionManager::WaitForWorkers()
{
	AppLog(Logger::DEBUG,"ConnectionManager Waiting for workers");
	for (unsigned int i=0; i<mWorkerVector.size(); i++ )
	{
		mWorkerVector[i]->Join();
		AppLog(Logger::DEBUG,"Connection worker shut down");
	}
}
