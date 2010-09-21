/*
 * virtualserver.cpp
 *
 *  Created on: Sep 15, 2010
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

#include "connectionmanager.h"
#include "virtualserver.h"
#include "request.h"

VirtualServer::VirtualServer() {
	// TODO Auto-generated constructor stub
	mSocket=-1;
	mConnectionManager = new ConnectionManager(10);
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


    if (listen(mSocket,10)==-1)
    {
    	perror("Listen failed:");
    }

    WaitForIncomming();
    return retval;

}

void VirtualServer::WaitForIncomming()
{
	struct sockaddr_in addr;
	mConnectionManager->StartHandleConnections();
	while(1)
	{
		socklen_t len = sizeof(addr);

		int clientSock = accept(mSocket, (struct sockaddr *) &addr, &len);

		if (clientSock!=-1)
		{
			std::cout << "VirtualServer::WaitForIncomming: socket " << clientSock << "\n";
			mConnectionManager->CreateConnection(clientSock);


		}
		else
		{
			perror("accept failed:");
		}

	};
}


