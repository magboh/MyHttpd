/*
 * requestqueueworker.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#include "pthread.h"
#include <iostream>
#include "request.h"
#include "requestqueue.h"
#include "requestqueueworker.h"

RequestQueueWorker::RequestQueueWorker(RequestQueue* requestQueue)
{
	mThread = new pthread_t;
	mKeepRunning = true;
	mRequestQueue = requestQueue;
}

RequestQueueWorker::~RequestQueueWorker()
{
	// TODO Auto-generated destructor stub
}

/* Worker is responsible to DELETE the request gotten from queue*/
void RequestQueueWorker::HandleRequests()
{
	const Request* request;
	sleep(6);
	while(mKeepRunning)
	{
		request = mRequestQueue->GetNextRequest();

		if (request != NULL)
		{
			HandleRequest(request);
			delete request ;
		}
	}

}

void* RequestQueueWorker::ThreadCallBack(void* arg)
{
	((RequestQueueWorker*)arg)->HandleRequests();
	return (NULL);
}

bool RequestQueueWorker::Start()
{
	bool ok = (pthread_create(mThread,NULL, RequestQueueWorker::ThreadCallBack,(void*)this)==0);
	return ok ;
}

void RequestQueueWorker::HandleRequest(const Request* request)
{

	std::cout << "RequestQueueWorker::HandleRequest() request:" << request->ToString() << "\n";
}
