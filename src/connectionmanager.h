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

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_


#include <vector>

class ConnectionQueueWorker;
class Site;
class RequestQueue;
class IoWorker;
class Connection;

class ConnectionManager
{
public:
	ConnectionManager(int maxConnections,RequestQueue& requestQueue);
	virtual ~ConnectionManager();
	void CreateConnection(int socket, const Site& site);
	void HandleConnection(Connection* con);
	void AddConnection(Connection* con);
	void PrintStats();
	/**
	 * Add a Worker to handle Connections.
	 */
	void AddConnectionWorker(int nr = 1);


	/**
	 * Add a IO Worker to handle Connection socket stats.
	 */
	void AddIoWorker(int nr = 1);

	/**
	 * Tell all added Workers to stop.
	 */
	void ShutdownWorkers();
	/**
	 * Wait for all Workers to stop.
	 * Caller thread blocked until all workers done.
	 */
	void WaitForWorkers();
private:

	int mNumConnections;
	int mMaxConnections;

	int mCurrentThread;
	RequestQueue& mRequestQueue;
	struct stats_t {
		unsigned int nrTotalConnections;
	} mStats;

	std::vector <ConnectionQueueWorker*> mWorkerVector;
	IoWorker* mIoWorker;
};

#endif /* CONNECTIONMANAGER_H_ */
