/*
 * main.cpp
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#include "myhttpd.h"

int main(int argc, char** argv)
{



	MyHttpd* myHttpd = new MyHttpd();

	if (myHttpd)
	{
		myHttpd->Start();
	}
	return 0;
}
