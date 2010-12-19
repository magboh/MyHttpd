/*
 * acceptworker.h
 *
 *  Created on: Dec 16, 2010
 *      Author: magnus
 */

#ifndef ACCEPTWORKER_H_
#define ACCEPTWORKER_H_

#include <map>
#include "thread.h"

/**
 * The AcceptWorker class handles all accepts on all site's sockets.
 * First call AddSite() for all sites in system
 * Then call HandleIncomming(): This is run in its own thread
 */

class Site;
class ConnectionManager;

class AcceptWorker: public Thread
{
public:
	AcceptWorker(ConnectionManager* connectionManager);
	virtual ~AcceptWorker();
	void AddSite(Site* site);
	void HandleIncomming();
private:
	virtual void DoWork();

	std::map<int, Site*> mSiteMap;
	int mEpollSocket;
	ConnectionManager* mConnectionManager;
};

#endif /* ACCEPTWORKER_H_ */
