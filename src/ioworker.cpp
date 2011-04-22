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

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include "ioworker.h"
#include "logger.h"
#include "connectionmanager.h"
#include "site.h"
#include "connection.h"

IoWorker::IoWorker(ConnectionManager& connectionManager) :
	mConnectionManager(connectionManager)
{
	mPollSocket=epoll_create(1000);
}

IoWorker::~IoWorker()
{
	// TODO Auto-generated destructor stub
}

void IoWorker::DoWork()
{
	const int MAX_EVENTS=100;
	const int EPOLL_WAIT=-1;
	struct epoll_event events[MAX_EVENTS];

	while (isRunning())
	{
		int nfds=epoll_wait(mPollSocket,events,MAX_EVENTS,EPOLL_WAIT);
		if (nfds==-1)
		{
			perror("IoWorker::DoWork() epoll_pwait");
		}
		for (int n=0;n<nfds;++n)
		{
			Connection* con=static_cast<Connection*> (events[n].data.ptr);
			// Tell Connection Manager to handle IO for this connection

			if ((events[n].events&(EPOLLERR|EPOLLHUP|EPOLLRDHUP)))
			{
				epoll_ctl(mPollSocket,EPOLL_CTL_DEL,con->GetSocket(),0);
				delete con;
			}
			else if ((events[n].events&EPOLLIN)==events[n].events)
			{
				mConnectionManager.HandleConnection(con);
			}
		}
	}
	close(mPollSocket);
	mPollSocket=-1;
}

void IoWorker::AddConnection(Connection* con)
{
	AppLog(Logger::DEBUG,"IoWorker::AddConnection");
	struct epoll_event ev;
	ev.events=EPOLLIN|EPOLLONESHOT|EPOLLET;
	ev.data.ptr=(void*) con;

	if (epoll_ctl(mPollSocket,EPOLL_CTL_ADD,con->GetSocket(),&ev))
	{
		perror("IoWorker::AddConnection");
		AppLog(Logger::CRIT,"epoll_ctl failed");
	}
}

void IoWorker::ModConnection(Connection* con)
{
	AppLog(Logger::DEBUG,"IoWorker::ModConnection");
	struct epoll_event ev;
	ev.events=EPOLLIN|EPOLLONESHOT|EPOLLET;
	ev.data.ptr=(void*) con;

	if (epoll_ctl(mPollSocket,EPOLL_CTL_MOD,con->GetSocket(),&ev))
	{
		perror("IoWorker::ModConnection");
		AppLog(Logger::CRIT,"epoll_ctl failed");
	}
}
