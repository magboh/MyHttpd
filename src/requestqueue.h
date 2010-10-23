/*
 * requestqueue.h
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#ifndef REQUESTQUEUE_H_
#define REQUESTQUEUE_H_
// The request queue will hold ALL request for all sites
// Each request is handled in order by x number of requestqueueworker
// All Requests added are condidered to be OWNED by the request queue.
// Once added nothig must be changed in request

// ALL Additions / Removals must be THREAD_SAFE !! use mutexes

#include <queue>
#include <vector>

class Request;
class RequestQueueWorker;

class RequestQueue
{
public:

	RequestQueue();
	virtual ~RequestQueue();
	void AddRequest(const Request* request);
	const Request* GetNextRequest();
	void Shutdown();
private:
	RequestQueue(const RequestQueue &) {};
	RequestQueue& operator=(const RequestQueue& p) {return *this;};

	std::queue <const Request*> mReqQueue;
	pthread_mutex_t* mMutex;
	pthread_cond_t* mCondThread;
	bool mKeepRunning;
};

#endif /* REQUESTQUEUE_H_ */
