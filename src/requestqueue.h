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
// All Requests added are considered to be OWNED by the request queue.
// Once added nothing must be changed in request

// ALL Additions / Removals must be THREAD_SAFE !! use mutexes

#include <queue>
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
	void PrintStats();
private:
	RequestQueue(const RequestQueue &) {};
	RequestQueue& operator=(const RequestQueue& rhs) { if (this==&rhs) return *this; return *this;};

	std::queue <const Request*> mReqQueue;
	pthread_mutex_t* mMutex;
	pthread_cond_t* mCondThread;
	bool mKeepRunning;
	unsigned short mNrInQueue;
	struct stats_t
	{
		unsigned long mTotalNrInQueue;
		unsigned long mHighestInQueue;
	} mStats;
};

#endif /* REQUESTQUEUE_H_ */
