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

#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <list>
#include <cassert>

#include "connection.h"
#include "connectionmanager.h"
#include "connectionqueueworker.h"
#include "requestqueue.h"

#include "logger.h"

ConnectionQueueWorker::ConnectionQueueWorker(RequestQueue& requestQueue, ConnectionManager& connectionManager) :
	mRequestQueue(requestQueue), mConnectionManager(connectionManager)
{
	mKeepRunning=true;
	mMutex=new pthread_mutex_t;
	pthread_mutex_init(mMutex,NULL);

	mEpollSocket=epoll_create(1000);
}

ConnectionQueueWorker::~ConnectionQueueWorker()
{
	pthread_mutex_destroy(mMutex);
	delete mMutex;
	mMutex=0;
}

void ConnectionQueueWorker::RemoveConnection(Connection *con)
{
	AppLog(Logger::DEBUG,"ConnectionQueueWorker::RemoveConnection");
	delete con;
}

void ConnectionQueueWorker::DoWork()
{
	// Max per iteration of data to send.. Should be ca 100kb.. this
	// This should be TrafficShaped to be throughput per second
	int readThrougput=4096;
	// Max per iteration of data to send.. Should be ca 100kb.. this
	// This should be TrafficShaped to be throughput per second
	int writeThrougput=4096;

	std::list<Connection*>::iterator it;

	Connection* con;
	while (mKeepRunning)
	{
		pthread_mutex_lock(mMutex);
		mList.merge(mAddList);
		pthread_mutex_unlock(mMutex);

		it=mList.begin();
		usleep(20);
		while (it!=mList.end())
		{
			enum State
			{
				REMOVE, WAIT, CONTINUE
			};
			State state=CONTINUE;

			con=*it;

			if (1)
			{
				Connection::ReadStatus_t readStatus=con->Read(readThrougput);

				if (readStatus==Connection::READ_STATUS_OK)
				{
					if (con->Parse())
					{
						mRequestQueue.AddRequest(con->GetRequest());
						con->SetDataPending(true);
						con->SetRequest(NULL);
						state=WAIT;
					}
				}
				else if (readStatus==Connection::READ_STATUS_DONE)
				{
					state=WAIT;
				}
				else
					state=REMOVE;
			}

			if (con->HasData())
			{
				int ret=con->Write(writeThrougput);
				state=CONTINUE;

				if (ret==1)
				{
					state=WAIT;
					con->SetDataPending(false);

					if (con->IsCloseable())
						state=REMOVE;
				}
			}
			else if (con->HasDataPending())
			{
				state=CONTINUE;
			}
			switch (state)
			{
			case REMOVE:
				it=mList.erase(it);
				RemoveConnection(con);
				break;
			case WAIT:
				it=mList.erase(it);
				mConnectionManager.AddConnection(con);
				break;
			case CONTINUE:
				it++;
				break;
			}
		}
	}
	AppLog(Logger::DEBUG,"ConnectionQueueWorker leaving");
}

void ConnectionQueueWorker::HandleConnection(Connection* con)
{
	AppLog(Logger::DEBUG,"ConnectionQueueWorker::HandleConnection");
	pthread_mutex_lock(mMutex);
	mAddList.push_back(con);
	pthread_mutex_unlock(mMutex);
}

void ConnectionQueueWorker::Stop()
{
	AppLog(Logger::DEBUG,"ConnectionQueueWorker::Stop");
	mKeepRunning=false;
}
