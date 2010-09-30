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
#include "requestqueueworker.h"

RequestQueue::RequestQueue()
{
	mMutex = new pthread_mutex_t;
	mCondThread = new pthread_cond_t;
	pthread_mutex_init(mMutex, NULL);
	pthread_cond_init (mCondThread, NULL);


}

RequestQueue::~RequestQueue()
{
	// TODO Auto-generated destructor stub
}

const Request* RequestQueue::GetNextRequest()
{
	assert(mMutex);

	const Request *request = NULL;

	assert(pthread_mutex_lock(mMutex)==0);

	if (mReqQueue.empty())
	{
		assert(pthread_cond_wait(mCondThread,mMutex)==0);
	}
	else
	{
		request = mReqQueue.front();
		mReqQueue.pop();
		std::cout << "threadid=" << pthread_self()<<  " handling " << request->ToString() << "\n";
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

bool RequestQueue::CreateQueueWorker()
{
	RequestQueueWorker* rqw = new RequestQueueWorker(this);

	if (rqw->Start()) // worker started
	{
		//mWorkerVector. push(rqw);
		mWorkerVector.push_back(rqw);
	}
	else
	{
		delete rqw;
		rqw = NULL;
	}

	return true;
}

static RequestQueue sInstance;
RequestQueue* RequestQueue::GetInstance()
{
	return &sInstance;
}
