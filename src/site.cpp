/*
 * site.cpp
 *
 *  Created on: Oct 26, 2010
 *      Author: magnus
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <iostream>
#include <errno.h>


#include "site.h"
#include "connectionmanager.h"
#include "config/siteoptions.h"

Site::Site(const SiteOptions* siteOptions)
{
	mPort = siteOptions->GetPort();
	mAddress = siteOptions->GetIp4Address();

	mDefaultFile = siteOptions->GetDefaultFile();
	mDocumentRoot = siteOptions->GetDocumentRoot();
	mSocket = -1 ;
	mListenQueue = 400;
}

Site::~Site()
{
	// TODO Auto-generated destructor stub
}

bool Site::Setup()
{
	SetupSocket();
	return true;
}

void Site::HandleIncomming()
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	int clientSock = accept(mSocket, (struct sockaddr *) &addr, &len);
	if (clientSock > 0)
	{
		int flags = fcntl(clientSock, F_GETFL, 0);
		fcntl(clientSock, F_SETFL, flags | O_NONBLOCK);
		mConnectionManager->CreateConnection(clientSock,this);
	}
}

int Site::SetupSocket()
{
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
	my_addr.sin_addr.s_addr = htonl(mAddress);
	my_addr.sin_port = htons(mPort);

	int on = 1;
	setsockopt( mSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

	if (bind(mSocket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind failed");
	}

	if (listen(mSocket,mListenQueue)==-1)
	{
		perror("Listen failed:");
	}

	int flags = fcntl(mSocket, F_GETFL, 0);
	fcntl(mSocket, F_SETFL, flags | O_NONBLOCK);

	return 0;
}

const std::string & Site::GetDocumentRoot() const
{
	return mDocumentRoot;
}
