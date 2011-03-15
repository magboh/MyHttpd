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

#ifndef SITEOPTIONS_H_
#define SITEOPTIONS_H_

#include <string>
#include <netinet/in.h>

class SiteOptions
{
public:


	SiteOptions();
	virtual ~SiteOptions();

    in_addr_t GetIp4Address() const;
    in_port_t GetPort() const;
    bool GetAllowDirectoryBrowsing() const;
    int GetConnectionTimeout() const;
    std::string GetDefaultFile() const;
    const std::string & GetDocumentRoot() const;

    void SetPort(in_port_t port);
    void SetAllowDirectoryBrowsing(bool mAllowDirectoryBrowsing);
    void SetConnectionTimeout(int mConnectionTimeout);
    void SetDefaultFile(std::string mDefaultFile);
    void SetIp4Address(in_addr_t addr);
    void SetDocumentRoot(const std::string & docRoot);
private:

	std::string mDefaultFile;
	std::string mDocumentRoot;
	bool mAllowDirectoryBrowsing;
	int mConnectionTimeout;
	in_addr_t mAddr;
	in_port_t mPort;
};

#endif /* SITEOPTIONS_H_ */
