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
