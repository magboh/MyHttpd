/*
 * configreader.h
 *
 *  Created on: Oct 24, 2010
 *      Author: magnus
 */

#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_

#include <string>
#include <vector>
#include "site.h"
#include "siteoptions.h"

class TiXmlElement;

class ConfigReader
{
public:
	ConfigReader();
	virtual ~ConfigReader();
	bool Load(const std::string & filename);

	void ParseLogOptions();
	bool ParseSiteOptions(TiXmlElement* element);

	bool ParseServerOptions(TiXmlElement* element);

	void ParseSites();
	void ParseSite();

	void ParseVirtualHostSites();
	void ParseVirtualHostSite();
private:

	std::vector <Site> mSiteVec;
	SiteOptions mSiteOptions;
};

#endif /* CONFIGREADER_H_ */
