/*
 * myhttpd.cpp
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
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

MyHttpd::MyHttpd() {
	// TODO Auto-generated constructor stub
	myhttpd = this;
}

MyHttpd::~MyHttpd() {
	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		delete mConnectionWorker[i] ;
	}

	for(int i=0; i<mNrRequestWorkers;i++)
	{
		delete mRequestWorker[i];
	}

	delete [] mConnectionWorker;
	delete [] mRequestWorker;


}

int MyHttpd::Start()
{
	ConfigReader cr;

	if (!LoadConfig(&cr))
	{
		AppLog(Logger::LOG_ERROR,"MyHttpd exiting due to problem loading configuration");
		return 0;
	}

	BlockSignals();

	StartRequestQueue();
	StartRequestWorkers();
	StartConnectionWorkers();


	AllowSignals();
	signal(SIGINT,handlerInt);
	StartSites(&cr);

	mKeepRunning = true;
	while (mKeepRunning)
	{
		for (size_t i=0; i<mSites.size() ; i++)
		{
			mSites[i].HandleIncomming();
		}
		usleep(10);
	}

	AppLog(Logger::LOG_INFO,"MyHttpd shutting down");

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
	mKeepRunning = false;
}

void MyHttpd::Stop()
{
	// Stop request queue
	StopRequestQueue();
	// Wait for request worker threads to die
	WaitForRequestWorkers();
	// Connection
	StopConnectionWorkers();
	//WaitForConnectionWorkers();
}

bool MyHttpd::LoadConfig(ConfigReader* cr)
{
	ConfigReader::LoadStatus ls= cr->Load("/home/magnus/Devel/myhttpd/myhttpd_conf.xml");
	bool ok = true;
	if (ls == ConfigReader::LOAD_OK)
	{
		const ServerOptions& so  = cr->GetServerOptions();

		mNrConnectionWorkers = so.GetNoIOWorkers();
		mNrRequestWorkers = so.GetNoRequstWorkers();
	}
	else if (ls == ConfigReader::BAD_FILE)
	{
		AppLog(Logger::LOG_ERROR,"Error in config file");
		ok=false;
	}
	else if (ls == ConfigReader::NO_FILE)
	{
		AppLog(Logger::LOG_ERROR,"Problem accessing config file");
		ok=false;
	}
	return ok;
}

void MyHttpd::StartRequestQueue()
{
	mRequestQueue = new RequestQueue();
}


void MyHttpd::StopRequestQueue()
{
	AppLog(Logger::LOG_DEBUG,"Shutting down Request Queue");
	mRequestQueue->Shutdown();
}

void MyHttpd::StartRequestWorkers()
{
	mRequestWorker = new RequestQueueWorker*[mNrRequestWorkers];
	for(int i=0; i<mNrRequestWorkers;i++)
	{
		mRequestWorker[i]= new RequestQueueWorker(mRequestQueue);
		mRequestWorker[i]->Start();
	}
}


void MyHttpd::StartConnectionWorkers()
{

	mConnectionWorker = new ConnectionQueueWorker*[mNrConnectionWorkers];

	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		mConnectionWorker[i]= new ConnectionQueueWorker(mRequestQueue);
		mConnectionWorker[i]->Start();
	}
}

void MyHttpd::WaitForRequestWorkers()
{
	for(int i=0; i<mNrRequestWorkers;i++)
	{
		if(mRequestWorker[i]->Join())
		{
			AppLog(Logger::LOG_DEBUG,"Request worker shut down");
		}
	}
}

void MyHttpd::StopConnectionWorkers()
{
	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		mConnectionWorker[i]->Stop();
		AppLog(Logger::LOG_DEBUG,"Shutting down Connection worker");
	}
}

void MyHttpd::WaitForConnectionWorkers()
{
	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		if(mConnectionWorker[i]->Join())
		{
			AppLog(Logger::LOG_DEBUG,"Connection worker shut down");
		}
	}
}

void MyHttpd::StartSites(const ConfigReader* cr)
{
	ConnectionManager* cm = new ConnectionManager(400,mNrConnectionWorkers,mConnectionWorker);

	const std::vector<SiteOptions> siteOpts = cr->GetSiteOptions();

	for (size_t i=0 ; i< siteOpts.size();i++)
	{

		const SiteOptions& so = siteOpts[i];
		Site s(&so,cm);
		if (s.Setup())
		{
			AppLog(Logger::LOG_DEBUG,"Site set up and added:" + s.GetDocumentRoot());
			mSites.push_back(s);
		}
	}

}
