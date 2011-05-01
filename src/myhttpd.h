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

#define VersionString "0.1"

class RequestQueue;
class ConfigReader;
class AcceptWorker;
class ConnectionManager;

struct RunOptions
{
	std::string configFile;
	bool debugLog;
};

class MyHttpd
{
public:
	MyHttpd();
	virtual ~MyHttpd();

	bool ParseArgs(int argc, char** argv);
	int Start(const RunOptions& options);
	void Stop();
	void SigINTHandler(int signal);

	bool LoadConfig(ConfigReader& cr, const std::string& fileName);
	static MyHttpd* myhttpd;
private:
	RequestQueue* mRequestQueue;
	AcceptWorker* mAcceptWorker;
	ConnectionManager* mConnectionManager;
	bool mKeepRunning;
	int mNumConnections;
	int mMaxConnections;

	int mNrConnectionWorkers;
	int mNrRequestWorkers;

	std::vector<Site*> mSites;
	void StartRequestQueue();
	void StartConnectionWorkers();
	void StartRequestWorkers();
	void StopRequestQueue();
	void StopConnectionWorkers();
	void WaitForConnectionWorkers();
	void WaitForRequestWorkers();

	void StartSites(const ConfigReader& cr);
	void StopSites();

	void AllowSignals();
	void BlockSignals();

	bool LoadConfig(ConfigReader* cr, const std::string& fileName);
};

#endif /* MYHTTPD_H_ */
