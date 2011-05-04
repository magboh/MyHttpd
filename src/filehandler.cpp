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

#include "filehandler.h"

FileHandler::FileHandler()
{
}

FileHandler::~FileHandler()
{
	std::map<std::string, File *>::iterator it;
	// file already in FileHandler?
	for (it=mFileMap.begin(); it!=mFileMap.end();it++)
	{
		delete it->second;
	}
}

const File* FileHandler::GetFile(const std::string& file, FileStatus &status)
{
	std::map<std::string, File *>::iterator it;
	File* f=NULL;

	// file already in FileHandler?
	if ((it=mFileMap.find(file))!=mFileMap.end())
	{
		f=it->second;
		status=FILESTATUS_OK;
	}
	else
	{
		f=CreateFile(file, status);
		if (f)
		{
			status=FILESTATUS_OK;
			mFileMap[file]=f;
		}
	}

	return f;
}

File* FileHandler::CreateFile(const std::string& file, FileStatus &status)
{
	int fd=open(file.c_str(), O_RDONLY);
	int error=errno;
	File* f=NULL;

	if (fd!=-1)
	{
		struct stat fileStat;
		fstat(fd, &fileStat);
		f=new File(fd, fileStat.st_size);
	}
	else
	{
		switch (error)
		{
		case EACCES:
			status=FileHandler::FILESTATUS_NOT_AUTHORIZED;
			break;
		case ENOENT:
			status=FileHandler::FILESTATUS_NO_FILE;
			break;
		default:
			status=FileHandler::FILESTATUS_INTERNAL_ERROR;
		}
	}

	return f;
}

