/*
 * virtualserver.h
 *
 *  Created on: Sep 15, 2010
 *      Author: magnus
 */

#ifndef VIRTUALSERVER_H_
#define VIRTUALSERVER_H_

class VirtualServer {
public:
	VirtualServer();
	virtual ~VirtualServer();

	bool Start();
private:
	void WaitForIncomming();
	void HandleIncomming(int socket);
private:
	int mSocket;

};

#endif /* VIRTUALSERVER_H_ */
