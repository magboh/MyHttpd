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


