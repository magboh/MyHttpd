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
	VirtualServer();
	virtual ~VirtualServer();

	bool Start();
	void Stop();
	void WaitForIncomming();
private:
private:
	int mSocket;
	ConnectionManager* mConnectionManager;
	bool mKeepRunning;

};

#endif /* VIRTUALSERVER_H_ */
