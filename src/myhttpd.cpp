/*
 * myhttpd.cpp
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#include <iostream>
#include "myhttpd.h"
#include "virtualserver.h"
#include  <signal.h>

MyHttpd::MyHttpd() {
	// TODO Auto-generated constructor stub
	mServer = NULL;

}
MyHttpd::~MyHttpd() {
	// TODO Auto-generated destructor stub
	delete mServer;
}

int MyHttpd::Start() {
	std::cout << __FUNCTION__;
	sigset_t set;
	signal(SIGINT,SigINTHandler);

	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	pthread_sigmask(SIG_BLOCK,&set,NULL);

	mServer = new VirtualServer();
	mServer->Start();

	pthread_sigmask(SIG_UNBLOCK,&set,NULL);


	mServer->WaitForIncomming();
	return 0;
}

void MyHttpd::SigINTHandler(int signal)
{
	std::cout << "\n HELLO \n" ;
}
