/*
 * connectionmanager.h
 *
 *  Created on: Sep 20, 2010
 *      Author: magnus
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_


class Connection;
struct pollfd;

class ConnectionManager {
public:
	ConnectionManager(int maxConnections);
	virtual ~ConnectionManager();
	void CreateConnection(int socket);
	void DeleteConnection(int socket);
	void StartHandleConnections();
private:
	static void* ThreadCallBack(void* arg);
	void HandleConnections();

	int mNumConnections;
	int mMaxConnections;
	Connection** mConnections;
	pollfd* mFds;
};

#endif /* CONNECTIONMANAGER_H_ */
