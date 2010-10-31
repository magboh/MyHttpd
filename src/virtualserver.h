/*
 * virtualserver.h
 *
 *  Created on: Sep 15, 2010
 *      Author: magnus
 */

#ifndef VIRTUALSERVER_H_
#define VIRTUALSERVER_H_

class ConnectionManager;

class VirtualServer {
public:
	VirtualServer(ConnectionManager* connectionManager);
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
	void ShutdownSubsystem();
	void Shutdown();
	int mNumConnections;
	int mMaxConnections;

	int mCurrentThread;


	struct stats_t {
		unsigned long mNrAcceptErrors;
	} mStats;


};

#endif /* VIRTUALSERVER_H_ */
