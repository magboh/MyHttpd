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

#include "serveroptions.h"

ServerOptions::ServerOptions() :
	mNoRequstWorkers(1),mNoIOWorkers(1),  mResponseBufferSize(4096), mRequestBufferSize(4096), mConnectionQueueSize(400)
{
}

ServerOptions::~ServerOptions()
{
}

int ServerOptions::GetConnectionQueueSize() const
{
	return mConnectionQueueSize;
}

int ServerOptions::GetNoConnectionWorkers() const
{
	return mNoIOWorkers;
}

int ServerOptions::GetNoRequstWorkers() const
{
	return mNoRequstWorkers;
}

int ServerOptions::GetRequestBufferSize() const
{
	return mRequestBufferSize;
}

int ServerOptions::GetResponseBufferSize() const
{
	return mResponseBufferSize;
}

void ServerOptions::SetConnectionQueueSize(int connectionQueueSize)
{
	mConnectionQueueSize=connectionQueueSize;
}

void ServerOptions::SetNoConnectionWorkers(int noIOWorkers)
{
	mNoIOWorkers=noIOWorkers;
}

void ServerOptions::SetNoRequstWorkers(int noRequstWorkers)
{
	mNoRequstWorkers=noRequstWorkers;
}

void ServerOptions::SetRequestBufferSize(int requestBufferSize)
{
	mRequestBufferSize=requestBufferSize;
}

void ServerOptions::SetResponseBufferSize(int responseBufferSize)
{
	mResponseBufferSize=responseBufferSize;
}

