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
#include <sstream>

#include "connection.h"
#include "connectionmanager.h"
#include "connectionworker.h"
#include "requestqueue.h"

#include "logger.h"
#include "mutex.h"

ConnectionWorker::ConnectionWorker(RequestQueue& requestQueue, ConnectionManager& connectionManager) :
	mRequestQueue(requestQueue), mConnectionManager(connectionManager)
{
	mKeepRunning=true;
	mMutex=new Mutex();
	mEpollSocket=epoll_create(1000);
}

ConnectionWorker::~ConnectionWorker()
{
	delete mMutex;
	mMutex=0;
}

void ConnectionWorker::RemoveConnection(Connection *con)
{
	AppLog(Logger::DEBUG,"ConnectionQueueWorker::RemoveConnection");
	delete con;
}

void ConnectionWorker::DoWork()
{
	// Max per iteration of data to send.. Should be ca 100kb.. this
	// This should be TrafficShaped to be throughput per second
	int readThrougput=4096;
	// Max per iteration of data to send.. Should be ca 100kb.. this
	// This should be TrafficShaped to be throughput per second
	int writeThrougput=4096;

	while (mKeepRunning)
	{
		mMutex->Lock();
		mList.merge(mAddList);
		mMutex->UnLock();

		std::list<Connection*>::iterator it=mList.begin();
		usleep(20);
		while (it!=mList.end())
		{
			Connection* con=*it;

			enum State
			{
				REMOVE, WAIT_FOR_IO, NO_ACTION
			};
			State state=NO_ACTION;
			Connection::Status_t status=con->Read(readThrougput);
			switch (status)
			{
			case Connection::STATUS_OK:
			{
				if (con->Parse())
				{
					mRequestQueue.AddRequest(con->GetRequest());
					con->SetDataPending(true);
					con->SetRequest(NULL);
					state=WAIT_FOR_IO;
				}
				break;
			}
			case Connection::STATUS_AGAIN:
			case Connection::STATUS_INTERUPT:
			{
				state=WAIT_FOR_IO;
				break;
			}
			default:
			{
				state=REMOVE;
			}
			}
			if (con->HasData())
			{
				con->SetDataPending(false);
				Connection::Status_t status=con->Write(writeThrougput);
				switch (status)
				{
				case Connection::STATUS_DONE:
				{
					state=WAIT_FOR_IO;
					if (con->IsCloseable())
						state=REMOVE;
					break;
				}
				case Connection::STATUS_OK:
				case Connection::STATUS_AGAIN:
				case Connection::STATUS_INTERUPT:
				{
					state=NO_ACTION;
					break;
				}
				case Connection::STATUS_ERROR:
				{
					state=REMOVE;
					break;
				}
				}
			}

			if (con->HasDataPending())
			{
				state=NO_ACTION;
			}

			// Determine what to do with current con, depending on state

			switch (state)
			{
			case REMOVE:
				it=mList.erase(it);
				RemoveConnection(con);
				break;
			case WAIT_FOR_IO:
				it=mList.erase(it);
				mConnectionManager.AddConnection(con);
				break;
			case NO_ACTION:
				it++;
				break;
			}
		}
	}
	AppLog(Logger::DEBUG,"ConnectionQueueWorker leaving");
}

void ConnectionWorker::HandleConnection(Connection* con)
{
	AppLog(Logger::DEBUG,"ConnectionQueueWorker::HandleConnection");
	mMutex->Lock();
	mAddList.push_back(con);
	mMutex->UnLock();
}

void ConnectionWorker::Stop()
{
	AppLog(Logger::DEBUG,"ConnectionQueueWorker::Stop");
	mKeepRunning=false;
}
