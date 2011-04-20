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

#include <iostream>
#include <string>

#include "myhttpd.h"
#include "logger.h"

void Usage();
void ParseArgs(int argc, char** argv);

void Usage()
{
	std::cout<<"MyHtppd version 0.1"<<"\n"<<"Author Magnus Bohman (magnus.bohman@gmail.com)\n"<<"\n"<<"Usage: myhttpd [-f file] [-debuglog]";
}

void ParseArgs(int argc, char** argv)
{

}

int main(int argc, char** argv)
{
	ParseArgs(argc,argv);

	Usage();

	enum State_t
	{
		NONE, LOG_LEVEL, CONF_FILE
	};

	State_t state=NONE;

	std::string logLevel="";
	std::string configFile="";
	for (int i=1;i<argc;i++)
	{
		std::string strArg(argv[i]);

		switch (state)
		{
		case LOG_LEVEL:
		{
			logLevel=strArg;
			state=NONE;
			break;
		}
		case CONF_FILE:
		{
			configFile = strArg;
			break;
		}
		default:
			break;
		}

		if (strArg.compare("-l")==0)
		{
			state=LOG_LEVEL;
		}
		else if (strArg.compare("-f")==0)
		{
			state=CONF_FILE;
		}

	}

	if (logLevel.length()>0)
	{
		Logger::LogType defaultLevel=Logger::INFO;

		if (logLevel.compare("DEBUG")==0)
			defaultLevel=Logger::DEBUG;
		if (logLevel.compare("INFO")==0)
			defaultLevel=Logger::INFO;
		std::cout<<"\nSetting Log level to:"<<logLevel<<"\n";
	}

	std::cout<<"Setting configuration file:"<<configFile<<"\n";

	MyHttpd myHttpd;

	myHttpd.Start();
	return 0;
}
