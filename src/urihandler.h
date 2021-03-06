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

#ifndef URIHANDLER_H_
#define URIHANDLER_H_

#include <string>
#include <map>

class Uri
{
public:
	enum FileStatus
	{
		FILESTATUS_OK, FILESTATUS_NO_FILE, FILESTATUS_NOT_AUTHORIZED, FILESTATUS_INTERNAL_ERROR
	};

	int GetFd() const
	{
		return mFd;
	}
	size_t GetSize() const
	{
		return mSize;
	}

	FileStatus GetStatus() const
	{
		return mStatus;
	}
	Uri(FileStatus status):
		mFd(-1),mSize(0), mContentType("") , mStatus(status), mTime(0)
	{
	}

	Uri(int fd, size_t size,const std::string& ct,FileStatus status,time_t t):
		mFd(fd),mSize(size),mContentType(ct),mStatus(status),mTime(t)
	{
	}

	const std::string& GetContentType() const
	{
		return mContentType;
	}

	const time_t GetTime() const
	{
		return mTime;
	}
private:
	int mFd;
	size_t mSize;
	int mRefCount;
	std::string mContentType;
	FileStatus mStatus;
	time_t mTime;
};

class UriHandler
{
public:
	UriHandler();
	virtual ~UriHandler();

	/**
	 *
	 * @param file
	 * @param status
	 * @return
	 */
	const Uri* GetFile(const std::string& file);
	const Uri& GetNullFile()
	{
		static std::string s("");
		static Uri sUri(-1,0,s,Uri::FILESTATUS_NO_FILE,0);
		return sUri;
	}
private:
	Uri* CreateFile(const std::string& file);
	std::map<std::string, Uri *> mFileMap;

};

#endif /* URIHANDLER_H_ */
