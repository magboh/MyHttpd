/*
 * requestqueueworker.h
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#ifndef REQUESTQUEUEWORKER_H_
#define REQUESTQUEUEWORKER_H_

class RequestQueue;

// Worker is responsible to DELETE the request gotten from queue

class RequestQueueWorker
{
public:
	RequestQueueWorker(RequestQueue* requestQueue);
	virtual ~RequestQueueWorker();
	bool Start();
private:
	void HandleRequests();
	static void* ThreadCallBack(void* arg);
	void HandleRequest(const Request* request);

	bool mKeepRunning;
	RequestQueue* mRequestQueue;
	pthread_t* mThread;
};

#endif /* REQUESTQUEUEWORKER_H_ */
