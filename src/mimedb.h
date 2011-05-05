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

class MimeDb
{
public:
	MimeDb();
	virtual ~MimeDb();
	/**
	 * Gets the mime type/subtype ie. text/html dependent of the extension ie. htm||html
	 * Thread-Safe
	 * @param extension
	 * @return
	 */
	const std::string & LookUp( const std::string& extension) const;
	bool ReadFile(const std::string& fileName);
private:
	MimeDb(const MimeDb& );
	MimeDb & operator=(const MimeDb&);

	void InitDefaultTypes();
	void AddType(const std::string type, const std::string ext);
	std::map<const std::string, const std::string> mDb;
	const std::string mDefaultMime;
};

#endif /* MIMEDB_H_ */
