/*
 * requestqueue.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#include <pthread.h>
#include <iostream>
#include <cassert>

#include "request.h"
#include "requestqueue.h"
#include "requestqueueworker.h"

RequestQueue::RequestQueue()
{
	std::cout << "RequestQueue::RequestQueue\n";
	mMutex = new pthread_mutex_t;
	*mMutex = PTHREAD_MUTEX_INITIALIZER ;
}

RequestQueue::~RequestQueue()
{
	// TODO Auto-generated destructor stub
}

const Request* RequestQueue::GetNextRequest()
{
	std::cout << "\nRequestQueue::GetNextRequest\n";
	if (!mMutex)
	{
		std::cout << "ASSÅ\n\n";
	}

	const Request *request = NULL;

	pthread_mutex_lock(mMutex);
	if (!mReqQueue.empty())
	{
		request = mReqQueue.front();
		mReqQueue.pop();
	}
	pthread_mutex_unlock(mMutex);
	return request;
}

void RequestQueue::AddRequest(const Request* request)
{
	std::cout <<"RequestQueue::AddRequest()\n";
	assert(pthread_mutex_lock(mMutex)==0);
	mReqQueue.push(request);
	assert(pthread_mutex_unlock(mMutex)==0);
}

bool RequestQueue::CreateQueueWorker()
{
	std::cout << "RequestQueue::CreateQueueWorker\n";
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
