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

#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "myhttpd.h"
#include "requestqueue.h"
#include "requestqueueworker.h"
#include "connectionmanager.h"
#include "connectionqueueworker.h"
#include "config/configreader.h"

#include "logger.h"

MyHttpd* MyHttpd::myhttpd = NULL;

void handlerInt(int s);

void handlerInt(int s)
{
	MyHttpd::myhttpd->SigINTHandler(s);
}

MyHttpd::MyHttpd()
{
	// TODO Auto-generated constructor stub
	myhttpd = this;
}

MyHttpd::~MyHttpd()
{

	for (int i = 0; i < mNrRequestWorkers; i++)
	{
		delete mRequestWorker[i];
		delete[] mRequestWorker;
	}
}

int MyHttpd::Start()
{
	ConfigReader cr;

	if (!LoadConfig(&cr))
	{
		AppLog(Logger::ERROR,"MyHttpd exiting due to problem loading configuration");
		return 0;
	}

	BlockSignals();

	StartRequestQueue();
	mConnectionManager =  new ConnectionManager(400, mRequestQueue);
	StartRequestWorkers();
	StartConnectionWorkers();

	AllowSignals();
	signal(SIGINT, handlerInt);
	StartSites(&cr);

	mKeepRunning = true;
	while (mKeepRunning)
	{
		for (size_t i = 0; i < mSites.size(); i++)
		{
			mSites[i].HandleIncomming();
		}
		usleep(10);
	}

	AppLog(Logger::INFO,"MyHttpd shutting down");

	Stop();
	return 0;
}

void MyHttpd::AllowSignals()
{
	sigset_t set;
	sigfillset(&set);
	pthread_sigmask(SIG_UNBLOCK, &set, NULL);
}

void MyHttpd::BlockSignals()
{
	sigset_t set;
	sigfillset(&set);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
}

void MyHttpd::SigINTHandler(int signal)
{
	mKeepRunning = false;
}

void MyHttpd::Stop()
{

	// All sites should be stopped (ie. their Socket closed).
	StopSites();
	// Stop request queue
	StopRequestQueue();
	// Wait for request worker threads to die
	WaitForRequestWorkers();

	// Stop Connections. Should have no more requests to handle
	StopConnectionWorkers();
	// Wait for Connection Threads to die
	WaitForConnectionWorkers();

}

bool MyHttpd::LoadConfig(ConfigReader* cr)
{
	ConfigReader::LoadStatus ls = cr->Load(
			"/home/magnus/Devel/myhttpd/myhttpd_conf.xml");
	bool ok = true;
	if (ls == ConfigReader::LOAD_OK)
	{
		const ServerOptions& so = cr->GetServerOptions();

		mNrConnectionWorkers = so.GetNoIOWorkers();
		mNrRequestWorkers = so.GetNoRequstWorkers();
	}
	else if (ls == ConfigReader::BAD_FILE)
	{
		AppLog(Logger::ERROR,"Error in config file");
		ok = false;
	}
	else if (ls == ConfigReader::NO_FILE)
	{
		AppLog(Logger::ERROR,"Problem accessing config file");
		ok = false;
	}
	return ok;
}

void MyHttpd::StartRequestQueue()
{
	mRequestQueue = new RequestQueue();
}

void MyHttpd::StopRequestQueue()
{
	AppLog(Logger::DEBUG,"Shutting down Request Queue");
	mRequestQueue->Shutdown();
}

void MyHttpd::StartRequestWorkers()
{
	mRequestWorker = new RequestQueueWorker*[mNrRequestWorkers];
	for (int i = 0; i < mNrRequestWorkers; i++)
	{
		mRequestWorker[i] = new RequestQueueWorker(mRequestQueue);
		mRequestWorker[i]->Start();
	}
}

void MyHttpd::WaitForRequestWorkers()
{
	for (int i = 0; i < mNrRequestWorkers; i++)
	{
		if (mRequestWorker[i]->Join())
		{
			AppLog(Logger::DEBUG,"Request worker shut down");
		}
	}
}

void MyHttpd::StartConnectionWorkers()
{
	mConnectionManager->AddWorker(mNrConnectionWorkers);
}

void MyHttpd::StopConnectionWorkers()
{
	mConnectionManager->ShutdownWorkers();
}

void MyHttpd::WaitForConnectionWorkers()
{
	mConnectionManager->WaitForWorkers();
}


void MyHttpd::StartSites(const ConfigReader* cr)
{


	const std::vector<SiteOptions> siteOpts = cr->GetSiteOptions();

	for (size_t i = 0; i < siteOpts.size(); i++)
	{
		const SiteOptions& so = siteOpts[i];
		Site s(&so, mConnectionManager);
		if (s.Setup())
		{
			AppLog(Logger::DEBUG,"Site set up and added:" + s.GetDocumentRoot());
			mSites.push_back(s);
		}
	}

}

void MyHttpd::StopSites()
{
	for (unsigned int i=0;i<mSites.size();i++)
	{
		mSites[i].Stop();
	}
}
