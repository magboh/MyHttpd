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
#include <poll.h>

#include "connectionmanager.h"
#include "virtualserver.h"
#include "request.h"
#include "requestqueue.h"

VirtualServer::VirtualServer() {
	// TODO Auto-generated constructor stub
	mSocket=-1;
	mConnectionManager = new ConnectionManager(500);
}

VirtualServer::~VirtualServer() {
	// TODO Auto-generated destructor stub
}

bool VirtualServer::Start()
{
	int domain= AF_INET;
	int protocol = 0 ;
	int type = SOCK_STREAM;
	bool retval = true;
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

    RequestQueue::GetInstance();

	RequestQueue::GetInstance()->CreateQueueWorker();
	RequestQueue::GetInstance()->CreateQueueWorker();
	RequestQueue::GetInstance()->CreateQueueWorker();

	WaitForIncomming();
    return retval;

}

static int a=0;
void VirtualServer::WaitForIncomming()
{
	struct sockaddr_in addr;

	while(1)
	{
		socklen_t len = sizeof(addr);

		int clientSock = accept(mSocket, (struct sockaddr *) &addr, &len);

		if (clientSock > 0)
		{
			if (++a > 1000000)
			{
				return;
			}

			/* set to non blocking*/
			//fcntl(clientSock,O_NONBLOCK);

			int flags = fcntl(clientSock, F_GETFL, 0);
			fcntl(clientSock, F_SETFL, flags | O_NONBLOCK);
			mConnectionManager->CreateConnection(clientSock);
		}
		else
		{
			perror("accept failed:");
		}

	};
}


