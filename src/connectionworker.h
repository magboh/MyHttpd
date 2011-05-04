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

#ifndef CONNECTIONWORKER_H_
#define CONNECTIONWORKER_H_

#include <list>
#include "thread.h"

class Site;
class Connection;

class ConnectionWorker:public Thread
{
public:
	ConnectionWorker();
	/**
	 *  Creates and handles Read/Writes of the socket.
	 * @param socket of new connection
	 * @param site, the site connected to
	 */
	void CreateConnection(int socket, const Site* site);
	virtual ~ConnectionWorker();
	/**
	 * Get the number of connections Worker currently working on. May not be 100% accurate
	 * Thread-safe
	 * @return
	 */
	size_t GetQueueSize();
private:

	virtual void DoWork();

	void AddConnection(Connection* con);
	void RemoveConnection(Connection* con);

	void UpdateConnectionIo();
	void WaitIo(Connection* con);
	enum State
	{
		REMOVE, WAIT_FOR_IO, NO_ACTION
	};

	State Read(Connection* con);
	State Write(Connection* con);
	/**
	 * used as the epoll() socket
	 */
	int mEpollSocket;

	/**
	 * List of Connections this worker handles
	 */
	std::list<Connection*> mList;
	/**
	 * Nr of connections in mList.
	 */
	size_t mQueSize;
};

#endif /* CONNECTIONWORKER_H_ */
