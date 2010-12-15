/*
 * filehandler.h
 *
 *  Created on: Dec 14, 2010
 *      Author: magnus
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
	enum FileStatus {FILESTATUS_OK, FILESTATUS_NO_FILE,FILESTATUS_NOT_AUTHORIZED, FILESTATUS__INTERNAL_ERROR };
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
