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
#include "connectionqueueworker.h"
#include "requestqueue.h"

#include "logger.h"

ConnectionQueueWorker::ConnectionQueueWorker(RequestQueue* reqQueue)
{
	mKeepRunning=true;
	mRequestQueue=reqQueue;
	mMutex=new pthread_mutex_t;
	pthread_mutex_init(mMutex, NULL);

	mKeepRunning=true;
}

ConnectionQueueWorker::~ConnectionQueueWorker()
{
	pthread_mutex_destroy(mMutex);
	mMutex=0;
}

void ConnectionQueueWorker::RemoveConnection(Connection *con)
{
	AppLog(Logger::DEBUG,"ConnectionQueueWorker::RemoveConnection");
	close(con->GetSocket());
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
	int count=0;
	std::list<Connection*>::iterator it;
	std::list<Connection*>::iterator end;
	Connection* con;
	const int timeout=30;
	while (mKeepRunning)
	{
		time_t now=time(NULL);

		pthread_mutex_lock(mMutex);
		count=mList.size();
		it=mList.begin();
		end=mList.end();
		pthread_mutex_unlock(mMutex);

		for (; (count>0)&&(it!=end); it++ )
		{
			con=*it;
			if (con->Read(readThrougput))
			{
				mRequestQueue->AddRequest(con->GetRequest());
				con->SetRequest(NULL);
				con->SetLastRequstTime(now);
			}

			if (con->HasData())
			{
				con->SetLastRequstTime(now);
				int ret=con->Write(writeThrougput);
				if (ret<0)
				{
					RemoveConnection(con);
					con=NULL;
					it=mList.erase(it);
				}
				else if (ret==1)
				{

					if (con->IsCloseable())
					{
						RemoveConnection(con);
						con=NULL;
						it=mList.erase(it);
					}
					else
					{
					}

				}
			}

			if (con!=0&&con->GetLastRequstTime()+timeout<now)
			{
				AppLog(Logger::DEBUG,"Connection time out");
				RemoveConnection(con);
				con=NULL;
				it=mList.erase(it);
			}

			if (!mKeepRunning&&con->GetRequest()!=NULL)
			{
				RemoveConnection(con);
				con=NULL;
				it=mList.erase(it);
			}

		}
		usleep(10);
	}
	AppLog(Logger::DEBUG,"ConnectionQueueWorker leaving");
}

void ConnectionQueueWorker::AddConnection(Connection* con)
{
	AppLog(Logger::DEBUG,"ConnectionQueueWorker::AddConnection");
	pthread_mutex_lock(mMutex);
	mList.push_back(con);
	pthread_mutex_unlock(mMutex);
}

void ConnectionQueueWorker::Stop()
{
	AppLog(Logger::DEBUG,"ConnectionQueueWorker::Stop");
	mKeepRunning=false;
}
