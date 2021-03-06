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
#include "acceptworker.h"
#include "connectionworker.h"

#include "config/configreader.h"
#include "logger.h"

MyHttpd* MyHttpd::myhttpd=NULL;

void handlerInt(int s);

void handlerInt(int s)
{
	MyHttpd::myhttpd->SigINTHandler(s);
}

MyHttpd::MyHttpd()
{
	myhttpd=this;
}

MyHttpd::~MyHttpd()
{

}

int MyHttpd::Start(const RunOptions& options)
{
	ConfigReader cr;

	if (!LoadConfig(cr,options.configFile))
	{
		AppLog(Logger::ERROR,"MyHttpd exiting due to problem loading configuration");
		return 0;
	}

	sAppLog.SetLogLevel((options.debugLog) ? Logger::DEBUG : Logger::INFO);
	BlockSignals();

	RequestQueue &requestQueue=RequestQueue::GetInstance();
	requestQueue.AddWorker(mNrRequestWorkers);

	AddConnectionWorker(mNrConnectionWorkers);

	AllowSignals();
	signal(SIGINT,handlerInt);
	StartSites(cr);

	mKeepRunning=true;

	while (mKeepRunning)
	{
		usleep(1000);
	}

	AppLog(Logger::INFO,"MyHttpd shutting down");

	Stop();
	return 0;
}

void MyHttpd::AllowSignals()
{
	sigset_t set;
	sigfillset(&set);
	pthread_sigmask(SIG_UNBLOCK,&set,NULL);
}

void MyHttpd::BlockSignals()
{
	sigset_t set;
	sigfillset(&set);
	pthread_sigmask(SIG_BLOCK,&set,NULL);
}

void MyHttpd::SigINTHandler(int signal)
{
	mKeepRunning=false;
}

void MyHttpd::Stop()
{
	// All sites should be stopped (ie. their Socket closed).
	StopSites();
	// Stop request queue
	AppLog(Logger::DEBUG,"Shutting down Request Queue");
	RequestQueue::GetInstance().Shutdown();
	// Wait for request worker threads to die
	RequestQueue::GetInstance().WaitForWorkers();

	// Stop Connections. Should have no more requests to handle
	// Wait for Connection Threads to die
	ShutdownWorkers();
	WaitForWorkers();

}

bool MyHttpd::LoadConfig(ConfigReader& cr, const std::string& fileName)
{
	ConfigReader::LoadStatus ls=cr.Load(fileName);
	bool ok=true;
	if (ls==ConfigReader::LOAD_OK)
	{
		const ServerOptions& so=cr.GetServerOptions();

		mNrConnectionWorkers=so.GetNoConnectionWorkers();
		mNrRequestWorkers=so.GetNoRequstWorkers();
	}
	else if (ls==ConfigReader::BAD_FILE)
	{
		AppLog(Logger::ERROR,"Error in config file");
		ok=false;
	}
	else if (ls==ConfigReader::NO_FILE)
	{
		AppLog(Logger::ERROR,"Problem accessing config file");
		ok=false;
	}
	return ok;
}

void MyHttpd::StartSites(const ConfigReader& cr)
{
	const std::vector<SiteOptions> siteOpts=cr.GetSiteOptions();

	mAcceptWorker=new AcceptWorker(mWorkerVector);
	for (size_t i=0;i<siteOpts.size();i++)
	{
		const SiteOptions& so=siteOpts[i];
		Site *s=new Site(&so);
		if (s->Setup())
		{
			AppLog(Logger::DEBUG,"Site set up and added:" + s->GetDocumentRoot());
			mSites.push_back(s);
			mAcceptWorker->AddSite(s);
		}
	}
	if (!mAcceptWorker->Start())
	{
		delete mAcceptWorker;
		mAcceptWorker=0;
		AppLog(Logger::CRIT,"Failed to start Accept worker");
	}

}

void MyHttpd::StopSites()
{

	for (size_t i=0;i<mSites.size();i++)
	{
		mSites[i]->Stop();
	}

	mAcceptWorker->Stop();
	mAcceptWorker->Join();
	delete mAcceptWorker;
	mAcceptWorker=0;

	for (size_t i=0;i<mSites.size();i++)
	{
		delete mSites[i];
	}

	mSites.clear();
}

void MyHttpd::AddConnectionWorker(int nr)
{
	for (int i=0;i<nr;i++)
	{
		ConnectionWorker* cqw=new ConnectionWorker();
		if (cqw->Start())
		{
			mWorkerVector.push_back(cqw);
			AppLog(Logger::DEBUG,"Connection worker added");
		}
		else
		{
			delete cqw;
			AppLog(Logger::CRIT,"Failed to create Connection worker");
		}
	}
}

void MyHttpd::ShutdownWorkers()
{
	AppLog(Logger::DEBUG,"MyHttpd Shutdown workers");

	for (size_t i=0;i<mWorkerVector.size();i++)
	{
		mWorkerVector[i]->Stop();
	}
}

void MyHttpd::WaitForWorkers()
{
	AppLog(Logger::DEBUG,"MyHttpd Waiting for workers");
	for (size_t i=0;i<mWorkerVector.size();i++)
	{
		mWorkerVector[i]->Join();
		delete mWorkerVector[i];
		AppLog(Logger::DEBUG,"Connection worker shut down");
	}
}
