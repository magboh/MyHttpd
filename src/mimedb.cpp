/***************************************************************************
 *      MyHTTPd
 *
 *      Wed, 4 May 2011 22:47:12 +0100
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

#include "mimedb.h"
#include "logger.h"
MimeDb::MimeDb() :
	mDefaultMime("text/html")
{
	InitDefaultTypes();
}

MimeDb::~MimeDb()
{
}

MimeDb& MimeDb::GetInstance()
{
	static MimeDb sInstance;
	return sInstance;
}

void MimeDb::InitDefaultTypes()
{
	AddType("htm","text/html");
	AddType("html","text/html");
	AddType("txt","text/plain");
	AddType("jpg","image/jpeg");
	AddType("jpeg","image/jpeg");
	AddType("jpe","image/jpeg");
	AddType("js","application/javascript");
	AddType("doc","application/msword");
}

const std::string & MimeDb::LookUp(const std::string& extension) const
{
	std::map<const std::string, const std::string>::const_iterator it=mDb.find(extension);
	AppLog(Logger::INFO,"Looking forextension " + it->second);

	if (it!=mDb.end())
	{
		AppLog(Logger::INFO,extension+" " + it->second);
		return it->second;
	}
	else
	{
		AppLog(Logger::INFO,extension+" " + mDefaultMime);
		return mDefaultMime;
	}
}

bool ReadFile(const std::string& fileName)
{
	// TODO: implement
	return true;
}

void MimeDb::AddType(const std::string ext, const std::string type)
{
	mDb.insert(std::pair<const std::string, const std::string>(ext,type));
}

