/*
 * virtualserver.h
 *
 *  Created on: Sep 15, 2010
 *      Author: magnus
 */

#ifndef VIRTUALSERVER_H_
#define VIRTUALSERVER_H_

class ConnectionManager;

class ConnectionQueueWorker;
class RequestQueueWorker;
class RequestQueue;

class VirtualServer {
public:
	VirtualServer();
	virtual ~VirtualServer();

	bool Start();
	void Stop();
	void WaitForIncomming();
	void PrintStats();
private:
	int mSocket;
	ConnectionManager* mConnectionManager;
	bool mKeepRunning;
	void SetupSocket();
	void SetupSubsystem();
	void ShutdownSubsystem();
	void Shutdown();

	int mNumConnections;
	int mMaxConnections;

	int mNrConnectionWorkers;
	int mNrRequestWorkers;
	int mCurrentThread;
	ConnectionQueueWorker** mConnectionWorker;
	RequestQueueWorker** mRequestWorker;
	RequestQueue* mRequestQueue;

	struct stats_t {
		unsigned long mNrAcceptErrors;
	} mStats;


};

#endif /* VIRTUALSERVER_H_ */
