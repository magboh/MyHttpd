/*
 * logger.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: magnus
 */

#include <iostream>
#include <pthread.h>
#include "logger.h"



Logger sAppLog("myhttpd.log");

Logger::Logger(const std::string& fileName)
{
	mMutex = new pthread_mutex_t;
	pthread_mutex_init(mMutex, NULL);

}

Logger::~Logger()
{
	delete mMutex;
	mMutex = 0 ;
}


void Logger::Log(LogType type, const std::string & message)
{
	Write(type,message);
}

void Logger::Log(LogType type, const std::stringstream & ss )
{
	Write(type,ss.str());
}


void Logger::Write(LogType type, const std::string & message)
{
	pthread_mutex_lock(mMutex);
	std::cout << GetCurrentTime() << " : " << GetTypeStr(type) <<" : "<< " " << message << "\n";
	pthread_mutex_unlock(mMutex);
}

const std::string & Logger::GetTypeStr(LogType type)
{
	static std::string typNames[ sizeof(LogType) ] = {"DBG","INF","ERR","CRI"};
	return typNames[type];
}


std::string  Logger::GetCurrentTime()
{
    time_t rawtime;
    time ( &rawtime );
    char buff[100];

    ctime_r(&rawtime,buff);

    std::string s = std::string(buff);
    s = s.substr(0,s.length()-1);
    return s;
}
