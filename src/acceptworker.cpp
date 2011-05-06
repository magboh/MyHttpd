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
#include "sys/epoll.h"
#include <netinet/in.h>

#include "acceptworker.h"
#include "connectionworker.h"
#include "site.h"
#include "logger.h"

AcceptWorker::AcceptWorker(std::vector <ConnectionWorker*>& conWorkerVector) :
	mConWorkerVector(conWorkerVector)
{
}

AcceptWorker::~AcceptWorker()
{
	close(mEpollSocket);
}

void AcceptWorker::DoWork()
{
	const int MAX_EVENTS=100;
	const int EPOLL_WAIT=500;

	struct epoll_event events[MAX_EVENTS];

	struct sockaddr_in addr;
	socklen_t len=sizeof(addr);

	if ((mEpollSocket=epoll_create(100))==-1)
	{
		AppLog(Logger::ERROR,"AcceptWorker leaving on epoll_create() error");
		return;
	}

	while(isRunning())
	{
		int nfds=epoll_wait(mEpollSocket,events,MAX_EVENTS,EPOLL_WAIT);
		if (nfds==-1)
		{
			perror("AcceptWorker::DoWork() epoll_wait:");
			AppLog(Logger::ERROR,"epoll_wait() failed");
		}

		ConnectionWorker *cw=GetWorker();

		for (int n=0;n<nfds;++n)
		{
			Site* site=static_cast<Site*> (events[n].data.ptr);
			int sock=site->GetSocket();
			if ((events[n].events&(EPOLLIN|EPOLLPRI))==events[n].events)
			{

				int clientSock=accept(sock,(struct sockaddr *) &addr,&len);

				if (clientSock!=-1)
				{
					int flags=fcntl(clientSock,F_GETFL,0);
					fcntl(clientSock,F_SETFL,flags|O_NONBLOCK);
					cw->CreateConnection(clientSock,site);
				}
				else
					perror("Accept():");
			}
			else
			{
				AppLog(Logger::INFO,"Closing site socket");
				epoll_ctl(mEpollSocket,EPOLL_CTL_DEL,sock,0);
			}

		}
	}
	AppLog(Logger::DEBUG,"Leaving AcceptWorker::DoWork");
}
void AcceptWorker::AddSite(Site* site)
{
	struct epoll_event ev;
	ev.data.ptr=(void*) site;
	ev.events=EPOLLIN;

	if (epoll_ctl(mEpollSocket,EPOLL_CTL_ADD,site->GetSocket(),&ev))
	{
		perror("AcceptWorker::AddSite()");
	}
}

void AcceptWorker::DeleteSite(Site* site)
{
	if (epoll_ctl(mEpollSocket,EPOLL_CTL_DEL,site->GetSocket(),NULL))
	{
		perror("AcceptWorker::AddSite()");
	}
}

ConnectionWorker *AcceptWorker::GetWorker()
{
	size_t workerMax= mConWorkerVector.size();
	ConnectionWorker *cw=mConWorkerVector[0];
	size_t small=cw->GetQueueSize();

	for (size_t i=1;i<workerMax;i++)
	{
		if (mConWorkerVector[i]->GetQueueSize() < small )
		{
			small=mConWorkerVector[i]->GetQueueSize();
			cw=mConWorkerVector[i];
		}
	}
	return cw;
}
