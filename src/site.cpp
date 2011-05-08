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

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <errno.h>

#include "site.h"
#include "config/siteoptions.h"
#include "logger.h"

Site::Site(const SiteOptions* siteOptions)
{
	mPort=siteOptions->GetPort();
	mAddress=siteOptions->GetIp4Address();

	mDefaultFile=siteOptions->GetDefaultFile();
	mDocumentRoot=siteOptions->GetDocumentRoot();
	mSocket=-1;
	mListenQueue=400;
}

Site::~Site()
{
}

bool Site::Setup()
{
	SetupSocket();
	return true;
}

int Site::SetupSocket()
{
	int domain=AF_INET;
	int protocol=0;
	int type=SOCK_STREAM;

	mSocket=socket(domain, type, protocol);

	if (mSocket==-1)
	{
		perror("socket fail:");
	}

	struct sockaddr_in my_addr;

	memset(&my_addr, 0, sizeof(struct sockaddr_in));
	my_addr.sin_family=AF_INET;
	my_addr.sin_addr.s_addr=htonl(mAddress);
	my_addr.sin_port=htons(mPort);

	int on=1;
	setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	if (bind(mSocket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in))==-1)
	{
		perror("bind failed");
	}

	if (listen(mSocket, mListenQueue)==-1)
	{
		perror("Listen failed:");
	}

	int flags=fcntl(mSocket, F_GETFL, 0);
	fcntl(mSocket, F_SETFL, flags|O_NONBLOCK);

	return 0;
}

const std::string & Site::GetDocumentRoot() const
{
	return mDocumentRoot;
}

void Site::Stop()
{
	AppLog(Logger::INFO,"Stopping site:" + mDocumentRoot);
	close(mSocket);
	mSocket=-1;
}
