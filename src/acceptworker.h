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

#ifndef ACCEPTWORKER_H_
#define ACCEPTWORKER_H_

#include <vector>
#include "thread.h"

/**
 * The AcceptWorker class handles all accepts on all site's sockets.
 * First call AddSite() for all sites in system
 * Then call HandleIncomming(): This is run in its own thread
 */

class Site;
class ConnectionWorker;

class AcceptWorker: public Thread
{
public:
	AcceptWorker(std::vector <ConnectionWorker*>& conWorkerVector);
	virtual ~AcceptWorker();
	void AddSite(Site* site);
	void DeleteSite(Site* site);
private:
	AcceptWorker(const AcceptWorker &);  // No implementation
	AcceptWorker& operator=(const AcceptWorker& rhs);  // No implementation

	virtual void DoWork();
	ConnectionWorker *GetWorker();
	int mEpollSocket;
	std::vector <ConnectionWorker*>& mConWorkerVector;
};

#endif /* ACCEPTWORKER_H_ */
