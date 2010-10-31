/*
 * myhttpd.cpp
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "myhttpd.h"
#include "virtualserver.h"
#include "requestqueue.h"
#include "requestqueueworker.h"
#include "connectionmanager.h"
#include "connectionqueueworker.h"
#include "config/configreader.h"

MyHttpd* MyHttpd::myhttpd = NULL;


void handlerInt(int s);

void handlerInt(int s)
{
	MyHttpd::myhttpd->SigINTHandler(s);
}

MyHttpd::MyHttpd() {
	// TODO Auto-generated constructor stub
	mServer = NULL;
	myhttpd = this;
}

MyHttpd::~MyHttpd() {
	// TODO Auto-generated destructor stub
	delete mServer;

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

int MyHttpd::Start() {

	LoadConfig();

	BlockSignals();

	StartRequestQueue();
	StartRequestWorkers();
	StartConnectionWorkers();




	AllowSignals();
	signal(SIGINT,handlerInt);
	StartVirtualServers();

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
	std::cout << "\n Shutting Down Now\n" ;
	Stop();
}

void MyHttpd::Stop()
{
	mServer->Stop();
}

void MyHttpd::LoadConfig()
{
	ConfigReader* cm = new ConfigReader();
	cm->Load("/home/magnus/Devel/myhttpd/myhttpd_conf.xml");

	const ServerOptions& so  = cm->GetServerOptions();

	mNrConnectionWorkers = so.GetNoIOWorkers();
	mNrRequestWorkers = so.GetNoRequstWorkers();

}

void MyHttpd::StartRequestQueue()
{
	mRequestQueue = new RequestQueue();
}


void MyHttpd::StopRequestQueue()
{
	std::cout << "Shutting down Request Queue\n";
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

void MyHttpd::StopConnectionWorkers()
{
	// Turn of reading new request from connections
	std::cout << "Blocking Connection Workers\n";

	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		mConnectionWorker[i]->Stop();
	}
}

void MyHttpd::WaitForRequestWorkers()
{
	for(int i=0; i<mNrRequestWorkers;i++)
	{
		if(mRequestWorker[i]->Join())
		{
			std::cout << "Request Worker shut down\n";
		}
	}
}

void MyHttpd::WaitForConnectionWorkers()
{
	for(int i=0; i<mNrConnectionWorkers;i++)
	{
		if(mConnectionWorker[i]->Join())
		{
			std::cout << "Connection Worked shut down\n";
		}
	}
}

void MyHttpd::StartVirtualServers()
{
	ConnectionManager* cm = new ConnectionManager(400,mNrConnectionWorkers,mConnectionWorker);
	mServer = new VirtualServer(cm);

	mServer->Start();
	mServer->WaitForIncomming();

}
