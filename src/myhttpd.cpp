/*
 * myhttpd.cpp
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#include <iostream>
#include <stdlib.h>
#include "myhttpd.h"
#include "virtualserver.h"
#include "signalhandler.h"

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

	SignalHandler<MyHttpd>::GetInstance()->BlockAll();

	mServer = new VirtualServer();

	// Start up threads, sockets etc.
	mServer->Start();

	SignalHandler<MyHttpd>::GetInstance()->BlockAll();
	SignalHandler<MyHttpd>::pFkn fkn;
	fkn = &(MyHttpd::SigINTHandler);
	SignalHandler<MyHttpd>::GetInstance()->RegisterINTHandler(fkn);

	mServer->WaitForIncomming();
	return 0;
}

void MyHttpd::SigINTHandler(int signal)
{
	std::cout << "\n HELLO \n" ;
	Stop();
	exit(0);
}

void MyHttpd::Stop()
{
	mServer->Stop();
}
