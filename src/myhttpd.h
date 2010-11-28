/*
 * myhttpd.h
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#ifndef MYHTTPD_H_
#define MYHTTPD_H_
#include <vector>
#include "site.h"
class RequestQueue;
class ConnectionQueueWorker;
class RequestQueueWorker;
class ConfigReader;

class MyHttpd {
public:
	MyHttpd();
	virtual ~MyHttpd();
	int Start();
	void Stop();
	void SigINTHandler(int signal);

	void AllowSignals();
	void BlockSignals();

	bool LoadConfig(ConfigReader* cr);

	static MyHttpd* myhttpd;
private:
	RequestQueue* mRequestQueue;
	bool mKeepRunning;
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

	void StartSites(const ConfigReader* cr);
	std::vector <Site> mSites;
};

#endif /* MYHTTPD_H_ */
