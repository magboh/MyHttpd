/*
 * ioworker.cpp
 *
 *  Created on: Mar 20, 2011
 *      Author: magnus
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
#define MAX_EVENTS 100
#define START_NR_SOCKETS 1000

	struct epoll_event events[MAX_EVENTS];

	for (;;)
	{
		int nfds=epoll_wait(mPollSocket,events,MAX_EVENTS,-1);
		if (nfds==-1)
		{
			perror("IoWorker::DoWork() epoll_pwait");
		}
		AppLog(Logger::DEBUG,"IoWorker::DoWork");
		for (int n=0;n<nfds;++n)
		{

			Connection* con=static_cast<Connection*> (events[n].data.ptr);
			// Tell Connection Manager to handle IO for this connection

			if (events[n].events&EPOLLIN)
				mConnectionManager.HandleConnection(con);
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

