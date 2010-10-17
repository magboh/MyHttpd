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
}

int MyHttpd::Start() {

	BlockSignals();
	mServer = new VirtualServer();

	// Start up threads, sockets etc.

	mServer->Start();
	AllowSignals();
	signal(SIGINT,handlerInt);
	mServer->WaitForIncomming();
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
