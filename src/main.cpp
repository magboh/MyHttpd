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
enum Action
{
	RUN, USAGE, VER
};
void Usage();
void Ver();
Action ParseArgs(int argc, char** argv, RunOptions& options);

void Usage()
{
	std::cout<<"MyHtppd version 0.1"<<"\n"<<"Author Magnus Bohman (magnus.bohman@gmail.com)\n"<<"\n"<<"Usage: myhttpd [-f file] [-l DEBUG] [--version]\n";
}

void Ver()
{
	std::cout<<"MyHtppd version 0.1"<<"\n"<<"Author Magnus Bohman (magnus.bohman@gmail.com)\n"<<"Uses TinyXml\n";
}

Action ParseArgs(int argc, char** argv, RunOptions& options)
{
	enum State_t
	{
		NONE, LOG_LEVEL, CONF_FILE
	};
	Action action=RUN;
	State_t state=NONE;

	std::string logLevel="";

	for (int i=1;i<argc;i++)
	{
		std::string strArg(argv[i]);

		switch (state)
		{
		case LOG_LEVEL:
		{
			logLevel=strArg;
			state=NONE;
			continue;
		}
		case CONF_FILE:
		{
			options.configFile=strArg;
			state=NONE;
			continue;
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
		else if (strArg.compare("--version")==0)
			action=VER;
		else
			action=USAGE;
	}

	if (logLevel.length()>0)
	{
		if (logLevel.compare("DEBUG")==0)
			options.defaultLogType=Logger::DEBUG;
		if (logLevel.compare("INFO")==0)
			options.defaultLogType=Logger::INFO;
	}
	return action;
}

int main(int argc, char** argv)
{
	RunOptions options;
	options.defaultLogType=Logger::INFO;
	options.configFile="/etc/myhttpd.conf";

	Action action=ParseArgs(argc,argv,options);
	switch (action)
	{
	case RUN:
	{
		MyHttpd myHttpd;
		myHttpd.Start(options);
		break;
	}
	case USAGE:
		Usage();
		break;
	case VER:
		Ver();
		break;
	}
	return 0;
}
