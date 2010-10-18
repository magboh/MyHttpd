/*
 * requestqueueworker.h
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#ifndef REQUESTQUEUEWORKER_H_
#define REQUESTQUEUEWORKER_H_

#include "thread.h"

class RequestQueue;

// Worker is responsible to DELETE the request gotten from queue

class RequestQueueWorker : public Thread
{
public:
	RequestQueueWorker(RequestQueue* requestQueue);
	virtual ~RequestQueueWorker();
private:
	void DoWork();
	void HandleRequest(const Request* request);
	RequestQueue* mRequestQueue;
};

#endif /* REQUESTQUEUEWORKER_H_ */
