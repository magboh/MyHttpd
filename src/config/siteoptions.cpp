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

#include <string>
#include "siteoptions.h"

SiteOptions::SiteOptions()
{
	// TODO Auto-generated constructor stub

}

SiteOptions::~SiteOptions()
{
	// TODO Auto-generated destructor stub
}

bool SiteOptions::GetAllowDirectoryBrowsing() const
{
	return mAllowDirectoryBrowsing;
}

int SiteOptions::GetConnectionTimeout() const
{
	return mConnectionTimeout;
}

std::string SiteOptions::GetDefaultFile() const
{
	return mDefaultFile;
}

void SiteOptions::SetAllowDirectoryBrowsing(bool allowDirectoryBrowsing)
{
	mAllowDirectoryBrowsing=allowDirectoryBrowsing;
}

void SiteOptions::SetConnectionTimeout(int connectionTimeout)
{
	mConnectionTimeout=connectionTimeout;
}

void SiteOptions::SetDefaultFile(std::string defaultFile)
{
	mDefaultFile=defaultFile;
}

in_addr_t SiteOptions::GetIp4Address() const
{
	return mAddr;
}

void SiteOptions::SetIp4Address(in_addr_t addr)
{
	mAddr=addr;
}

void SiteOptions::SetPort(in_port_t port)
{
	mPort=port;
}

in_port_t SiteOptions::GetPort() const
{
	return mPort;
}

const std::string & SiteOptions::GetDocumentRoot() const
{
	return mDocumentRoot;
}

void SiteOptions::SetDocumentRoot(const std::string & docRoot)
{
	mDocumentRoot=docRoot;
}
