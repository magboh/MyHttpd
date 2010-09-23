/*
 * connectionmanager.cpp
 *
 *  Created on: Sep 20, 2010
 *      Author: magnus
 */
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <iostream>
#include <poll.h>
#include <pthread.h>

#include "connection.h"
#include "connectionmanager.h"

ConnectionManager::ConnectionManager(int maxConnections)
{

	mMaxConnections = maxConnections;
	mNumConnections = 0;
	mConnections = new Connection *[mMaxConnections];
	mFds = new pollfd[mMaxConnections];
	for (int i=0;i<mMaxConnections;i++)
	{
		mConnections[i]=NULL;
		mFds[i].events = POLLIN;
		mFds[i].fd = 0 ;
	}
}

ConnectionManager::~ConnectionManager()
{
	// TODO Auto-generated destructor stub
}


void ConnectionManager::CreateConnection(int socket)
{
	int index=0;

	for (index=0 ; index<mMaxConnections ; index++)
	{
		if (mConnections[index]==NULL)
		{
			mConnections[index] = new Connection(socket);
			mFds[index].fd = socket;
			std::cout <<"ConnectionManager::CreateConnection: socket" << socket << "at index=" << index << "\n";
			break;
		}
	}
}

void ConnectionManager::DeleteConnection(int socket)
{
	int index=0;

	for (index=0 ; index<mMaxConnections ; index++)
	{
		if (mConnections[index]!=NULL && mConnections[index]->GetSocket() == socket)
		{
			delete mConnections[index] ;
			mFds[index].fd = 0;
			std::cout <<"ConnectionManager::DeleteConnection: socket" << socket << "at index=" << index << "\n";
			break;
		}
	}
}



/**
 * Run from ThreadCb
 */
void ConnectionManager::HandleConnections()
{
	nfds_t nfds = mMaxConnections ;
	while(1)
	{
		int ret = poll(mFds,nfds,1000);

		if (ret<0)
		{
			perror("poll failed");
		}
		std::cout << "\n ret=" << ret << "\n";
		if (ret>0)
		{
			for (int index=0;index<mMaxConnections;index++)
			{
				if (mFds[index].revents == POLLIN)
				{
					mConnections[index]->Read();
				}

			}
		}
	}



}

void ConnectionManager::StartHandleConnections()
{
	static pthread_t mThread;
	if (pthread_create(&mThread,NULL, ConnectionManager::ThreadCallBack,(void*)this)!=0)
	{
		perror("pthread_create");
	}
}


void* ConnectionManager::ThreadCallBack(void* arg)
{
	ConnectionManager* connectionManager = (ConnectionManager*) arg;
	connectionManager->HandleConnections();
}
