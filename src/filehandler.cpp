/*
 * filehandler.cpp
 *
 *  Created on: Dec 14, 2010
 *      Author: magnus
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
	// TODO Auto-generated constructor stub

}

FileHandler::~FileHandler()
{
	// TODO Auto-generated destructor stub
}

const File* FileHandler::GetFile(const std::string& file , FileStatus &status)
{
	std::map<std::string , File * >::iterator it;
	File* f = NULL;

	// file already in FileHandler?
	if ( (it = mFileMap.find(file)) != mFileMap.end() )
	{
		f=it->second;
		status = FILESTATUS_OK;
	} else
	{
		f = CreateFile(file,status);
		if (f)
		{
			status = FILESTATUS_OK;
			mFileMap[file] = f;
		}
	}

	return f;
}


File* FileHandler::CreateFile(const std::string& file, FileStatus &status)
{
	int fd = open(file.c_str(),O_RDONLY);
	int error = errno;
	File* f = NULL;

	if (fd != -1)
	{
		struct stat fileStat;
		fstat(fd,&fileStat);
		f = new File(fd,fileStat.st_size);
	}
	else
	{
		switch (error)
		{
		case EACCES:
			status =  FileHandler::FILESTATUS_NOT_AUTHORIZED;
			break;
        case ENOENT:
        	status = FileHandler::FILESTATUS_NO_FILE;
        	break;
        default:
        	status = FileHandler::FILESTATUS__INTERNAL_ERROR;
		}
	}

	return f ;
}

