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
private:
	void WaitForIncomming();
private:
	int mSocket;
	ConnectionManager* mConnectionManager;
};

#endif /* VIRTUALSERVER_H_ */
