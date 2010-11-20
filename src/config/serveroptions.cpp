/*
 * serveroptions.cpp
 *
 *  Created on: Oct 28, 2010
 *      Author: magnus
 */

#include "serveroptions.h"

ServerOptions::ServerOptions()
{
	// TODO Auto-generated constructor stub

}

ServerOptions::~ServerOptions()
{
	// TODO Auto-generated destructor stub
}

int ServerOptions::GetConnectionQueueSize() const
{
    return mConnectionQueueSize;
}

int ServerOptions::GetNoIOWorkers() const
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
    mConnectionQueueSize = connectionQueueSize;
}

void ServerOptions::SetNoIOWorkers(int noIOWorkers)
{
	mNoIOWorkers = noIOWorkers;
}

void ServerOptions::SetNoRequstWorkers(int noRequstWorkers)
{
    mNoRequstWorkers = noRequstWorkers;
}

void ServerOptions::SetRequestBufferSize(int requestBufferSize)
{
    mRequestBufferSize = requestBufferSize;
}

void ServerOptions::SetResponseBufferSize(int responseBufferSize)
{
    mResponseBufferSize = responseBufferSize;
}


