/*
 * acceptworker.cpp
 *
 *  Created on: Dec 16, 2010
 *      Author: magnus
 */


#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include "sys/epoll.h"
#include <netinet/in.h>

#include "acceptworker.h"
#include "connectionmanager.h"
#include "site.h"

AcceptWorker::AcceptWorker(ConnectionManager* connectionManager)
{
	mConnectionManager = connectionManager;
}

AcceptWorker::~AcceptWorker()
{
	// TODO Auto-generated destructor stub
}


void AcceptWorker::DoWork()
{
	#define MAX_EVENTS 10

	struct epoll_event ev[mSiteMap.size()], events[MAX_EVENTS];

	int clientSock, nfds;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
    mEpollSocket = epoll_create(10);

    if (mEpollSocket == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }
    int i=0;
    for (std::map<int , Site*>::iterator it = mSiteMap.begin() ; it != mSiteMap.end() ; it++ , i++ )
    {
    	ev[i].events = EPOLLIN;
    	int socket = (*it).first;
    	ev[i].data.fd = socket;
    	if (epoll_ctl(mEpollSocket, EPOLL_CTL_ADD, socket, &ev[i]) == -1) {
    		perror("epoll_ctl: listen_sock");
    		exit(EXIT_FAILURE);
    	}
    }

    for (;;) {
        nfds = epoll_wait(mEpollSocket, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_pwait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n)
        {
        	int sock = events[n].data.fd;
        	Site* site = mSiteMap[sock];
        	clientSock = accept(sock, (struct sockaddr *) &addr, &len);

        	if (clientSock != -1)
        	{

        		int flags = fcntl(clientSock, F_GETFL, 0);
        		fcntl(clientSock, F_SETFL, flags | O_NONBLOCK);
        		mConnectionManager->CreateConnection(clientSock,site);
            }

        }
    }

}

void AcceptWorker::AddSite(Site* site)
{
	mSiteMap[site->GetSocket()] = site;
}

void AcceptWorker::HandleIncomming()
{
	this->Start();
}
