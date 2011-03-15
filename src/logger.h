/*
 * logger.h
 *
 *  Created on: Mar 14, 2011
 *      Author: magnus
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <sstream>


class Logger
{
public:
	enum LogType{ LOG_DEBUG, LOG_INFO, LOG_ERROR , LOG_CRIT};

	void Log(LogType type, const std::string & message);
	void Log(LogType type, const std::stringstream & ss );

	Logger(const std::string& fileName);
	virtual ~Logger();

private:
	void Write(LogType type, const std::string & message);

	const std::string & GetTypeStr(LogType type);
	std::string GetCurrentTime();

	pthread_mutex_t* mMutex;
};


extern Logger sAppLog;

#define AppLog(t,m) sAppLog.Log(t,m)

#endif /* LOGGER_H_ */
