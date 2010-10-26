/*
 * configreader.h
 *
 *  Created on: Oct 24, 2010
 *      Author: magnus
 */

#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

#include <string>

class ConfigReader
{
public:
	ConfigReader();
	virtual ~ConfigReader();
	void Load(const std::string & filename);

	void ParseLogOptions();
	void ParseSiteOptions();

	void ParseServerOptions();

	void ParseSites();
	void ParseSite();

	void ParseVirtualHostSites();
	void ParseVirtualHostSite();


};

#endif /* CONFIGREADER_H_ */
