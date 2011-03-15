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

#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include <string>
#include <map>

class File
{
public:
	int GetFd() const {return mFd;};
	size_t GetSize() const {return mSize;};
	File(int fd, size_t size)
	{
		mFd = fd ;
		mSize = size;
	}
private:
	int mFd;
	size_t mSize;
	int mRefCount;
};

class FileHandler
{
public:
	enum FileStatus {FILESTATUS_OK, FILESTATUS_NO_FILE,FILESTATUS_NOT_AUTHORIZED, FILESTATUS_INTERNAL_ERROR };
	FileHandler();
	virtual ~FileHandler();

	/**
	 *
	 * @param file
	 * @param status
	 * @return
	 */
	const File* GetFile(const std::string& file , FileStatus &status);

private:

	File* CreateFile(const std::string& file, FileStatus &status);

	std::map<std::string , File * > mFileMap;

};

#endif /* FILEHANDLER_H_ */
