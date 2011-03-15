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

	mStats.mHighestInQueue=0;
	mStats.mTotalNrInQueue=0;
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
 * OWnership of Request is transfered to caller.
 * @return The request is returned or NULL if we are in closing down mode
 */
const Request* RequestQueue::GetNextRequest()
{
	assert(mMutex);

	const Request *request = NULL;

	pthread_mutex_lock(mMutex);

	while (1)
	{
		if (mReqQueue.empty())
		{
			if (mKeepRunning)
				pthread_cond_wait(mCondThread,mMutex);
			else
				break; // closing down..
		}
		else
		{
			request = mReqQueue.front();
			mReqQueue.pop();
			mNrInQueue--;
			break;
		}
	}

	pthread_mutex_unlock(mMutex);
	return request;
}

void RequestQueue::AddRequest(const Request* request)
{
	pthread_mutex_lock(mMutex);
	pthread_cond_broadcast(mCondThread);
	mReqQueue.push(request);
	mStats.mTotalNrInQueue++;
	if (++mNrInQueue>mStats.mHighestInQueue)
		mStats.mHighestInQueue=mNrInQueue;

	pthread_mutex_unlock(mMutex);
}


void RequestQueue::Shutdown()
{
	pthread_mutex_lock(mMutex);
/* Awake the threads, and have them get a NULL reequest*/
	mKeepRunning = false;
	pthread_cond_broadcast(mCondThread);
	pthread_mutex_unlock(mMutex);
}

void RequestQueue::PrintStats()
{
	std:: cout << "---- Request Queue ----\n";
	std:: cout << "Total number of requests: " <<  mStats.mTotalNrInQueue <<  "\n";
	std:: cout << "Highest number of requests in queue: " <<  mStats.mHighestInQueue <<  "\n";
}


