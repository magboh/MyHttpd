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
	std::cout << "\n" << type << " " << message;
	pthread_mutex_unlock(mMutex);
}
