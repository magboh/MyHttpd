/*
 * virtualserver.cpp
 *
 *  Created on: Sep 15, 2010
 *      Author: magnus
 */

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <iostream>
#include <errno.h>
#include <time.h>
#include "connectionqueueworker.h"
#include "connectionmanager.h"
#include "virtualserver.h"
#include "request.h"
#include "requestqueue.h"
#include "requestqueueworker.h"

VirtualServer::VirtualServer() {
	// TODO Auto-generated constructor stub
	mSocket=-1;
	mKeepRunning = true;

	mMaxConnections = 500 ;
	mNrConnectionWorkers = 2;
	mNrRequestWorkers = 4;

	mStats.mNrAcceptErrors=0;
}

VirtualServer::~VirtualServer() {
	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		delete mConnectionWorker[i] ;
	}

	for(int i=0; i<mNrRequestWorkers;i++)
	{
		delete mRequestWorker[i];
	}

	delete [] mConnectionWorker;
	delete [] mRequestWorker;

}

bool VirtualServer::Start()
{
	SetupSubsystem();

	/*Open up to others*/
	SetupSocket();

    return true;

}
int a=0;
void VirtualServer::WaitForIncomming()
{
	struct sockaddr_in addr;
	time_t statsTime = time(NULL);
	while(mKeepRunning)
	{
		socklen_t len = sizeof(addr);

		int clientSock = accept(mSocket, (struct sockaddr *) &addr, &len);
		int error = errno;
		if (clientSock > 0)
		{
			/* set to non blocking*/
			//fcntl(clientSock,O_NONBLOCK);

			int flags = fcntl(clientSock, F_GETFL, 0);
			fcntl(clientSock, F_SETFL, flags | O_NONBLOCK);
			mConnectionManager->CreateConnection(clientSock);
		}
		else
		{
			if (error == EWOULDBLOCK || error == EAGAIN)
			{
				usleep(10);
			}
			else {
				mStats.mNrAcceptErrors++;
			}
		}

		if (++a>100)
		{
			a=0;
			time_t newTime = time(NULL);

			if (newTime>statsTime+5)
			{
				statsTime=newTime;
				PrintStats();
			}
		}

	}

}


void VirtualServer::Stop()
{
	mKeepRunning = false;
	ShutdownSubsystem();

	PrintStats();
}

void VirtualServer::Shutdown()
{
	close(mSocket);
	ShutdownSubsystem();
}


void VirtualServer::SetupSocket()
{
	mKeepRunning = true;

	int domain= AF_INET;
	int protocol = 0 ;
	int type = SOCK_STREAM;

	mSocket = socket(domain,type,protocol);

	if (mSocket == -1 )
	{
		perror("socket fail:");
	}

	struct sockaddr_in my_addr;

	memset(&my_addr, 0, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(8080);

	int on = 1;
	setsockopt( mSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

	if (bind(mSocket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind failed");
	}

	if (listen(mSocket,60)==-1)
	{
		perror("Listen failed:");
	}

	int flags = fcntl(mSocket, F_GETFL, 0);
	fcntl(mSocket, F_SETFL, flags | O_NONBLOCK);


}

void VirtualServer::SetupSubsystem()
{

	mRequestQueue = new RequestQueue();
	mRequestWorker = new RequestQueueWorker*[mNrRequestWorkers];
	for(int i=0; i<mNrRequestWorkers;i++)
	{
		mRequestWorker[i]= new RequestQueueWorker(mRequestQueue);
		mRequestWorker[i]->Start();
	}



	typedef ConnectionQueueWorker cqwp;
	mConnectionWorker = new ConnectionQueueWorker*[mNrConnectionWorkers];

	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		mConnectionWorker[i]= new ConnectionQueueWorker(mRequestQueue);
		mConnectionWorker[i]->Start();
	}

	mConnectionManager = new ConnectionManager(mMaxConnections,mNrConnectionWorkers,mConnectionWorker);
}

void VirtualServer::ShutdownSubsystem()
{

	// Turn of reading new request from connections
	std::cout << "Blocking Connection Workers\n";

	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		mConnectionWorker[i]->Stop();
	}

	std::cout << "Shutting down Request Queue\n";
	mRequestQueue->Shutdown();

	for(int i=0; i<mNrRequestWorkers;i++)
	{
		if(mRequestWorker[i]->Join())
		{
			std::cout << "Request Worker shut down\n";
		}
	}

	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		if(mConnectionWorker[i]->Join())
		{
			std::cout << "Connection Worked shut down\n";
		}
	}

}


void VirtualServer::PrintStats()
{
	mRequestQueue->PrintStats();
	mConnectionManager->PrintStats();

	std::cout << "---- Virtual Server  ----\n";
	std::cout << "Total accept() errors:" << mStats.mNrAcceptErrors << "\n";
}


