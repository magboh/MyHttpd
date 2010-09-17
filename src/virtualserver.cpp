/*
 * virtualserver.cpp
 *
 *  Created on: Sep 15, 2010
 *      Author: magnus
 */

#include "virtualserver.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <iostream>
VirtualServer::VirtualServer() {
	// TODO Auto-generated constructor stub
	mSocket=-1;
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

	while(1)
	{
		socklen_t len = sizeof(addr);

		int clientSock = accept(mSocket, (struct sockaddr *) &addr, &len);

		if (clientSock!=-1)
		{
			HandleIncomming(clientSock);
		}
		else
		{
			perror("accept failed:");
		}

	};

}


void VirtualServer::HandleIncomming(int socket)
{
	assert(socket!=-1);
	const size_t size = 16384;
	static unsigned char buf[size];

	memset(buf,0x00,size);

	/*Read*/
	int len = 0 ;
	int readBytes=0;

	while( (len = read(socket,&buf+readBytes,size-readBytes) ) > 0)
	{
		readBytes+= len ;
		std::cout << "\nread:" << buf ;

		if (strcmp("\n\r\n\r",(char*)buf-4)) {
			break;
		}
	}
	std::cout << "\nTotal bytes:" << readBytes <<"\n";
	close(socket);
	// Read 0 bytes, means socket is closes on other side
	if (len==0)
	{
		close(socket);
	}
	else if (len<0)
	{
		perror("read error:");
	}



}

