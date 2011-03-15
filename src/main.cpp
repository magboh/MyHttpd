/*
 * main.cpp
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#include "myhttpd.h"

int main(int argc, char** argv)
{
	MyHttpd myHttpd;
	myHttpd.Start();

	return 0;
}
