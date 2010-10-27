/*
 * main.cpp
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#include "myhttpd.h"
#include "configreader.h"

int main(int argc, char** argv)
{

	ConfigReader* cm = new ConfigReader();
	cm->Load("/home/magnus/Devel/myhttpd/myhttpd_conf.xml");

/*
	MyHttpd* myHttpd = new MyHttpd();

	if (myHttpd)
	{
		myHttpd->Start();
	}*/
	return 0;
}
