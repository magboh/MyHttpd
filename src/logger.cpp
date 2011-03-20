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
#include <pthread.h>
#include "logger.h"

Logger sAppLog("myhttpd.log");

Logger::Logger(const std::string& fileName)
{
	mMutex=new pthread_mutex_t;
	pthread_mutex_init(mMutex, NULL);
}

Logger::~Logger()
{
	delete mMutex;
	mMutex=0;
}

void Logger::Log(LogType type, const std::string & message)
{
	Write(type, message);
}

void Logger::Log(LogType type, const std::stringstream & ss)
{
	Write(type, ss.str());
}

void Logger::Write(LogType type, const std::string & message)
{
	pthread_mutex_lock(mMutex);
	std::cout<<GetCurrentTime()<<" : "<<GetTypeStr(type)<<" : "<<message<<"\n";
	pthread_mutex_unlock(mMutex);
}

const std::string & Logger::GetTypeStr(LogType type)
{
	static std::string typNames[sizeof(LogType)]=
	{ "DBG", "INF", "ERR", "CRI" };
	return typNames[type];
}

std::string Logger::GetCurrentTime()
{
	time_t rawtime;
	time(&rawtime);
	char buff[100];

	ctime_r(&rawtime, buff);

	std::string s=std::string(buff);
	s=s.substr(0, s.length()-1);
	return s;
}
