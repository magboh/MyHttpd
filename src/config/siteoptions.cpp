/*
 * siteoptions.cpp
 *
 *  Created on: Oct 26, 2010
 *      Author: magnus
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
    mAllowDirectoryBrowsing = allowDirectoryBrowsing;
}

void SiteOptions::SetConnectionTimeout(int connectionTimeout)
{
    mConnectionTimeout = connectionTimeout;
}

void SiteOptions::SetDefaultFile(std::string defaultFile)
{
    mDefaultFile = defaultFile;
}

in_addr_t SiteOptions::GetIp4Address() const
{
	return mAddr;
}

void SiteOptions::SetIp4Address(in_addr_t addr)
{
	mAddr= addr;
}


void SiteOptions::SetPort(in_port_t port)
{
	mPort = port;
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
	mDocumentRoot = docRoot ;
}
