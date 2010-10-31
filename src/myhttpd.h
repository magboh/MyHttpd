/*
 * myhttpd.h
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#ifndef MYHTTPD_H_
#define MYHTTPD_H_

class VirtualServer;
class RequestQueue;
class ConnectionQueueWorker;
class RequestQueueWorker;

class MyHttpd {
public:
	MyHttpd();
	virtual ~MyHttpd();
	int Start();
	void Stop();
	void SigINTHandler(int signal);

	void AllowSignals();
	void BlockSignals();

	void LoadConfig();

	static MyHttpd* myhttpd;
private:

	VirtualServer *mServer;
	RequestQueue* mRequestQueue;

	int mNumConnections;
	int mMaxConnections;

	int mNrConnectionWorkers;
	int mNrRequestWorkers;

	ConnectionQueueWorker** mConnectionWorker;
	RequestQueueWorker** mRequestWorker;

	void StartRequestQueue();
	void StartConnectionWorkers();
	void StartRequestWorkers();
	void StopRequestQueue();
	void StopConnectionWorkers();

	void WaitForConnectionWorkers();
	void WaitForRequestWorkers();

	void StartVirtualServers();

};

#endif /* MYHTTPD_H_ */
