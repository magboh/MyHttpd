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