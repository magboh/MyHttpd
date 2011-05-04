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
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <list>
#include <cassert>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include "connection.h"
#include "connectionworker.h"
#include "requestqueue.h"

#include "logger.h"

ConnectionWorker::ConnectionWorker() :
	mQueSize(0)
{
}

ConnectionWorker::~ConnectionWorker()
{
}

void ConnectionWorker::RemoveConnection(Connection *con)
{
	if (IsAppLog(Logger::DEBUG))
		AppLog(Logger::DEBUG,"ConnectionQueueWorker::RemoveConnection");
	epoll_ctl(mEpollSocket,EPOLL_CTL_DEL,con->GetSocket(),0);
	delete con;
}

void ConnectionWorker::DoWork()
{
	mEpollSocket=epoll_create(1000);

	if (mEpollSocket==-1)
	{
		AppLog(Logger::ERROR,"ConnectionWorker leaving on epoll_create() error");
		return;
	}

	std::list<Connection*>::iterator it=mList.begin();

	int nr=0;
	while (isRunning())
	{
		char loopCounter=0;

		UpdateConnectionIo();
		nr++;
		if (nr>10000)
		{
			std::stringstream ss;
			ss<<"list.size="<<mList.size()<<" mqueuesize="<<mQueSize;
			nr=0;
			AppLog(Logger::INFO,ss.str());
		}
		it=mList.begin();

		while ((it!=mList.end())&&(++loopCounter<2)) // Not more than 10 laps, before checking for more connections
		{
			Connection* con=*it;
			State state=NO_ACTION;
			state=Read(con);

			if (con->HasData())
			{
				state=Write(con);
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
				con=0;
				--mQueSize;
				break;
			case WAIT_FOR_IO:
				it=mList.erase(it);
				WaitIo(con);
				con=0;
				--mQueSize;
				break;
			case NO_ACTION:
				it++;
				break;
			}
		}
	}
	AppLog(Logger::DEBUG,"ConnectionQueueWorker leaving");

	close(mEpollSocket);
	mEpollSocket=-1;

}

ConnectionWorker::State ConnectionWorker::Read(Connection* con)
{
	State state=NO_ACTION;
	if (IsAppLog(Logger::DEBUG))
		AppLog(Logger::DEBUG,"ConnectionWorker::Read");
	Connection::Status_t status=con->Read(1024);
	switch (status)
	{
	case Connection::STATUS_OK:
	{
		if (con->Parse())
		{
			RequestQueue::GetInstance().AddRequest(con->GetRequest());
			con->SetDataPending(true);
			con->SetRequest(NULL);
			state=WAIT_FOR_IO;
		} // state already set to NO_ACTION Above

		break;
	}
	case Connection::STATUS_AGAIN:
	case Connection::STATUS_INTERUPT:
	{
		state=WAIT_FOR_IO;
		break;
	}

	case Connection::STATUS_CLOSE:
	default:
	{
		state=REMOVE;
	}
	}
	return state;
}

ConnectionWorker::State ConnectionWorker::Write(Connection* con)
{
	State state=NO_ACTION;

	con->SetDataPending(false);
	Connection::Status_t status=con->Write(1024);
	if (IsAppLog(Logger::DEBUG))
		AppLog(Logger::DEBUG,"ConnectionWorker::Write");
	switch (status)
	{
	case Connection::STATUS_DONE:
	{
		if (con->IsCloseable())
			state=REMOVE;
		else
			state=WAIT_FOR_IO;
		break;
	}
	case Connection::STATUS_OK:
	case Connection::STATUS_AGAIN:
	case Connection::STATUS_INTERUPT:
	{
		state=NO_ACTION;
		break;
	}
	case Connection::STATUS_CLOSE:
	case Connection::STATUS_ERROR:
	{
		state=REMOVE;
		break;
	}
	}

	return state;
}

void ConnectionWorker::UpdateConnectionIo()
{
	const int MAX_EVENTS=100;
	const int EPOLL_WAIT=0; /* NO one there, well thread got more things to do*/
	const int EPOLL_WAIT_EMPTY=500; /* We have no other to worry about. Wait a while*/
	struct epoll_event events[MAX_EVENTS];

	int nfds=epoll_wait(mEpollSocket,events,MAX_EVENTS,mList.empty() ? EPOLL_WAIT_EMPTY : EPOLL_WAIT);

	if (IsAppLog(Logger::DEBUG))
		AppLog(Logger::DEBUG,"ConnectionWorker::UpdateConnectionIo");
	for (int n=0;n<nfds;n++)
	{
		Connection* con=static_cast<Connection*> (events[n].data.ptr);
		// Tell Connection Manager to handle IO for this connection
		if ((events[n].events&(EPOLLERR|EPOLLHUP|EPOLLRDHUP)))
		{
			RemoveConnection(con);
		}
		else if ((events[n].events&EPOLLIN)==events[n].events)
		{
			/*			if (find(mList.begin(),mList.end(),con)!=mList.end())
			 {
			 AppLog(Logger::CRIT,"ERROR ERROR");
			 assert(false);
			 }
			 else
			 */
			mQueSize++;
			mList.push_back(con);
		}
	}
}

void ConnectionWorker::AddConnection(Connection* con)
{
	if (IsAppLog(Logger::DEBUG))
		AppLog(Logger::DEBUG,"ConnectionWorker::AddConnection");
	struct epoll_event ev;

	ev.events=EPOLLIN|EPOLLONESHOT|EPOLLET;
	ev.data.ptr=(void*) con;

	if (epoll_ctl(mEpollSocket,EPOLL_CTL_ADD,con->GetSocket(),&ev))
	{
		perror("ConnectionWorker::AddConnection");
		AppLog(Logger::CRIT,"epoll_ctl failed");
	}
}

void ConnectionWorker::WaitIo(Connection* con)
{
	if (IsAppLog(Logger::DEBUG))
		AppLog(Logger::DEBUG,"ConnectionWorker::WaitIo");
	struct epoll_event ev;
	ev.events=EPOLLIN|EPOLLONESHOT|EPOLLET;
	ev.data.ptr=(void*) con;

	if (epoll_ctl(mEpollSocket,EPOLL_CTL_MOD,con->GetSocket(),&ev))
	{
		perror("ConnectionWorker::ModConnection");
		AppLog(Logger::CRIT,"epoll_ctl failed");
	}
}

void ConnectionWorker::CreateConnection(int socket, const Site* site)
{
	if (IsAppLog(Logger::DEBUG))
		AppLog(Logger::DEBUG,"ConnectionWorker::CreateConnection");
	AddConnection(new Connection(socket,site));
}

size_t ConnectionWorker::GetQueueSize()
{
	return mQueSize;
}
