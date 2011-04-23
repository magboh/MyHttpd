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
#include "connectionworker.h"

#include "ioworker.h"
#include "logger.h"

ConnectionManager::ConnectionManager(int maxConnections) :
	mNumConnections(0), mMaxConnections(maxConnections), mCurrentThread(0)
{
	mStats.nrTotalConnections=0;
}

ConnectionManager::~ConnectionManager()
{

}

void ConnectionManager::CreateConnection(int socket, const Site& site)
{
	++mStats.nrTotalConnections;
	mIoWorker->AddConnection(new Connection(socket,site,mCurrentThread++%mWorkerVector.size()));
}

void ConnectionManager::AddConnection(Connection* con)
{
	AppLog(Logger::DEBUG,"ConnectionManager AddConnection()");
	mIoWorker->ModConnection(con);
}

void ConnectionManager::HandleConnection(Connection* con)
{
	AppLog(Logger::DEBUG,"ConnectionManager HandleConnection()");
	mWorkerVector[con->GetThreadNr()]->HandleConnection(con);
}

void ConnectionManager::AddConnectionWorker(RequestQueue& requestQueue, int nr)
{
	for (int i=0;i<nr;i++)
	{
		ConnectionWorker* cqw=new ConnectionWorker(requestQueue,*this);
		if (cqw->Start())
		{
			mWorkerVector.push_back(cqw);
			AppLog(Logger::DEBUG,"Connection worker added");
		}
		else
		{
			delete cqw;
			AppLog(Logger::CRIT,"Failed to create Connection worker");
		}
	}
}

void ConnectionManager::AddIoWorker(int nr)
{
	for (int i=0;i<nr;i++)
	{
		mIoWorker=new IoWorker(*this);
		if (mIoWorker->Start())
		{
			AppLog(Logger::DEBUG,"IO worker added");
		}
		else
		{
			delete mIoWorker;
			mIoWorker=0;
			AppLog(Logger::CRIT,"Failed to create Io worker");
		}
	}
}

void ConnectionManager::ShutdownWorkers()
{
	AppLog(Logger::DEBUG,"ConnectionManager Shutdown workers");

	for (size_t i=0;i<mWorkerVector.size();i++)
	{
		mWorkerVector[i]->Stop();
	}

	mIoWorker->Stop();
}

void ConnectionManager::WaitForWorkers()
{
	AppLog(Logger::DEBUG,"ConnectionManager Waiting for workers");
	for (size_t i=0;i<mWorkerVector.size();i++)
	{
		mWorkerVector[i]->Join();
		delete mWorkerVector[i];
		AppLog(Logger::DEBUG,"Connection worker shut down");
	}

	delete mIoWorker;
}
