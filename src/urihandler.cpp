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

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include "urihandler.h"
#include "mimedb.h"
#include "logger.h"

UriHandler::UriHandler()
{
}

UriHandler::~UriHandler()
{
	std::map<std::string, Uri *>::iterator it;
	// file already in UriHandler?
	for (it=mFileMap.begin();it!=mFileMap.end();it++)
	{
		delete it->second;
	}
}

const Uri* UriHandler::GetFile(const std::string& uri)
{
	std::map<std::string, Uri *>::iterator it;
	Uri* u=NULL;

	// file already in UriHandler?
	if ((it=mFileMap.find(uri))!=mFileMap.end())
	{
		u=it->second;
	}
	else
	{
		u=CreateFile(uri);
		mFileMap[uri]=u;
	}

	return u;
}

Uri* UriHandler::CreateFile(const std::string& file)
{
	int fd=open(file.c_str(),O_RDONLY);
	int error=errno;
	Uri* f=NULL;

	if (fd!=-1)
	{
		struct stat fileStat;
		fstat(fd,&fileStat);

		const MimeDb& mimeDb=MimeDb::GetInstance();
		size_t pos=0;

		if ((pos=file.find_last_of("."))!=std::string::npos)
		{
			const std::string& ct=mimeDb.LookUp(file.substr(pos+1));
			f=new Uri(fd,fileStat.st_size,ct,Uri::FILESTATUS_OK,fileStat.st_mtim.tv_sec);
		}
		if (f==NULL)
		{
			const std::string& ct=mimeDb.GetDefault();
			f=new Uri(fd,fileStat.st_size,ct,Uri::FILESTATUS_OK,fileStat.st_mtim.tv_sec);
		}
	}
	else
	{
		Uri::FileStatus status;
		switch (error)
		{
		case EACCES:
			status=Uri::FILESTATUS_NOT_AUTHORIZED;
			break;
		case ENOENT:
			status=Uri::FILESTATUS_NO_FILE;
			break;
		default:
			status=Uri::FILESTATUS_INTERNAL_ERROR;
		}

		f=new Uri(status);

	}

	return f;
}

