/*
 * requestqueue.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#include <pthread.h>
#include <cassert>
#include <iostream>
#include <errno.h>
#include <stdio.h>

#include "request.h"
#include "requestqueue.h"


RequestQueue::RequestQueue()
{
	mMutex = new pthread_mutex_t;
	pthread_mutex_init(mMutex, NULL);

	mCondThread = new pthread_cond_t;
	pthread_cond_init (mCondThread, NULL);
	mKeepRunning= true;
}

RequestQueue::~RequestQueue()
{
	delete mMutex;
	delete mCondThread;
	mMutex = NULL;
	mCondThread = NULL;
}

/***
 * Used by RequestWorker to obtain a request to work with.
 * @return The request is returned or NULL if we are in closing down mode
 */
const Request* RequestQueue::GetNextRequest()
{
	assert(mMutex);

	const Request *request = NULL;

	assert(pthread_mutex_lock(mMutex)==0);

	while (1)
	{
		if (mReqQueue.empty())
		{
			if (mKeepRunning)
				assert(pthread_cond_wait(mCondThread,mMutex)==0);
			else
				break; // closing down..
		}
		else
		{
			request = mReqQueue.front();
			mReqQueue.pop();
			break;
		}
	}

	assert(pthread_mutex_unlock(mMutex)==0);
	return request;
}

void RequestQueue::AddRequest(const Request* request)
{
	assert(pthread_mutex_lock(mMutex)==0);
	pthread_cond_broadcast(mCondThread);
	mReqQueue.push(request);
	assert(pthread_mutex_unlock(mMutex)==0);
}


void RequestQueue::Shutdown()
{
	assert(pthread_mutex_lock(mMutex)==0);
/* Awake the threads, and have them get a NULL reequest*/
	mKeepRunning = false;
	pthread_cond_broadcast(mCondThread);
	assert(pthread_mutex_unlock(mMutex)==0);
}



