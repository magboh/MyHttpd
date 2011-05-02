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

class RequestQueue;
class Connection;
class ConnectionManager;
class Mutex;
class ConnectionWorker:public Thread
{
public:
	ConnectionWorker(ConnectionManager& connectionManager);
	/**
	 * Called from ConnectionManager when there is work read/write/close to be done on this connection
	 * Method is thread-safe, and takes ownership of the Connection object con.
	 * @param con
	 */
	void HandleConnection(Connection* con);
	virtual ~ConnectionWorker();
private:

	virtual void DoWork();

	void RemoveConnection(Connection* con);

	enum State
	{
		REMOVE, WAIT_FOR_IO, NO_ACTION
	};

	State Read(Connection* con);
	State Write(Connection* con);
	/**
	 *
	 */
	ConnectionManager& mConnectionManager;
	/**
	 * used as the epoll() socket
	 */
	int mEpollSocket;

	/**
	 * List of Connections this worker handles
	 */
	std::list<Connection*> mList;

	/**
	 * List of Connections newly added. Not just added to mList
	 */
	std::list<Connection*> mAddList;

	Mutex* mMutex;
};

#endif /* CONNECTIONWORKER_H_ */
