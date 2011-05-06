/***************************************************************************
 *      MyHTTPd
 *
 *      Tue, 15 Mar 2011 22:16:12 +0100
 *      Copyright 2011 Magnus Bohman
 *      magnus.bohman@gmail.com
 ***************************************************************************/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, US.
 */

#ifndef REQUESTQUEUE_H_
#define REQUESTQUEUE_H_

#include <queue>
#include <list>
class Request;
class RequestWorker;

/*!
 \brief Class responsible for queueing Requests for the RequestWorkers to work with

 The RequestQueue class allows ConnectionWorker-threads to queue Requests,
 that the RequestWorker-threads will handle. A ConnectionWorker uses GetNextRequest() to dequeue,
 a Request and start to work with it.
 If there are no Request in queue for a worker, then that worker-thread, will wait for new requests.
 When recieving a Request all waiting thread workers will be notified.

 All public methods are made thread-safe with mutex locks, except GetInstance().
 GetInstance() should first be called in single-threaded mode, to properly set up the Singleton.
 Then it is thread-safe for all to use.
  */

class RequestQueue
{
public:

	virtual ~RequestQueue();
	/**
	 * Adds a Request to the queue.
	 * This method is thread-safe
	 * @param request
	 */
	void AddRequest(const Request* request);

	/**
	 * Returns next Request to be handled from queue.
	 * Caller takes ownership of the Request so Caller MUST delete it.
	 * Caller waits if no Requests are available util request enter queue or Queue is closed
	 * Return NULL means Queue closing. Do not call again
	 * @return Next Request to handle or NULL if closing down Queue
	 */
	const Request* GetNextRequest();

	/**
	 * Adds a Request Worker Thread, to start work on the Request Queue
	 * @param nr
	 */
	void AddWorker(int nr=1);

	/**
	 * Let worker threads finish with the current queue. Then instead of waiting for new Requests,
	 * The threads will then stop and terminate.
	 */
	void Shutdown();

	/**
	 * Waits for all worker threads to terminate after a Shutdown.
	 * Takes care of cleaning up, and freeing memory.
	 * This method will block caller until ALL threads are done.
	 */
	void WaitForWorkers();

	/**
	 *  Returns Singleton instance of RequestQueue.
	 *  NOT thread-safe, do not call first time from multiple threads. Then it is ok.
	 * @return
	 */
	static RequestQueue & GetInstance();
private:
	RequestQueue();
	RequestQueue(const RequestQueue &); // No implementation
	RequestQueue& operator=(const RequestQueue& rhs); // No implementation

	/**
	 * The might request queue
	 */
	std::queue<const Request*> mReqQueue;

	/**
	 * the mutex
	 */
	pthread_mutex_t* mMutex;

	/**
	 * Conditional variable, used for worker threads to sleep until new Requests arrive
	 */
	pthread_cond_t* mCondThread;

	/**
	 * While true, worker threads sleeps if no Requests. Otherwis they will terminate.
	 */
	bool mKeepRunning;
	unsigned short mNrInQueue;
	struct stats_t
	{
		unsigned long mTotalNrInQueue;
		unsigned long mHighestInQueue;
	} mStats;

	/**
	 * List of RequestWorker threads, working the queue
	 */
	std::list<RequestWorker*> mWorkerList;
};

#endif /* REQUESTQUEUE_H_ */
