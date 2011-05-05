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

#ifndef MIMEDB_H_
#define MIMEDB_H_

#include <map>
#include <string>
/*!
 * The MimeDb class handles looking-up file extensions ie. .txt,.html and .jpg and mapping them to
 * the correct MIME type as defined by IANA http://www.iana.org/assignments/media-types/index.html
 * A mime.types file can be read by ReadFile() to either add new or change existing MIME type mappings.
 *
 * Multiple extensions can map to a single MIME.
 */
class MimeDb
{
public:
	virtual ~MimeDb();

	/**
	 * Gets the mime type/subtype ie. text/html dependent of the extension ie. htm||html
	 * Thread-Safe
	 * @param extension file extension. Only part txt,html,jpg in small
	 * @return corresponding mime-type if found, or default "text/html"
	 */
	const std::string & LookUp(const std::string& extension) const;

	/**
	 * NOT implemented yet
	 * @param fileName
	 * @return
	 */
	bool ReadFile(const std::string& fileName);

	/**
	 * Gets the singleton instance
	 * NOT thread-safe, do not call first time from multiple threads
	 * @return
	 */
	static MimeDb& GetInstance();
private:


	MimeDb();
	MimeDb(const MimeDb&); // No implementation
	MimeDb & operator=(const MimeDb&); // No implementation

	/**
	 * Set up a bunch of standard MIME-types. Could be overrided by ReadFile()
	 */
	void InitDefaultTypes();

	/**
	 * Helper funtion to add MIME-types and extensions
	 * @param ext
	 * @param type
	 */
	void AddType(const std::string ext, const std::string type);

	/**
	 * One map to keep 'em all.
	 */
	std::map<const std::string, const std::string> mDb;

	/**
	 * Default MIMR type
	 */
	const std::string mDefaultMime;
};

#endif /* MIMEDB_H_ */
