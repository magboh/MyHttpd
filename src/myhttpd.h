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

#ifndef MYHTTPD_H_
#define MYHTTPD_H_
#include <vector>
#include "site.h"

#define Version 0.1
#define VersionString "0.1"
class RequestQueue;
class ConfigReader;
class AcceptWorker;

class MyHttpd
{
public:
	MyHttpd();
	virtual ~MyHttpd();
	int Start();
	void Stop();
	void SigINTHandler(int signal);

	void AllowSignals();
	void BlockSignals();

	bool LoadConfig(ConfigReader* cr);

	static MyHttpd* myhttpd;
private:
	RequestQueue* mRequestQueue;
	bool mKeepRunning;
	int mNumConnections;
	int mMaxConnections;

	int mNrConnectionWorkers;
	int mNrRequestWorkers;

	AcceptWorker* mAcceptWorker;
	ConnectionManager* mConnectionManager;

	void StartSites(const ConfigReader* cr);
	void StopSites();
	std::vector<Site*> mSites;
};

#endif /* MYHTTPD_H_ */
