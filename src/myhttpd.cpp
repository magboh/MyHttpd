/*
 * myhttpd.cpp
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#include <iostream>
#include "myhttpd.h"
#include "virtualserver.h"

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

	mServer = new VirtualServer();
	mServer->Start();

	return 0;
}
