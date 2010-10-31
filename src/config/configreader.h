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
#include "serveroptions.h"

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

	const std::vector<Site> & GetSites();

	const ServerOptions & GetServerOptions();
private:

	std::vector <Site> mSiteVec;
	SiteOptions mSiteOptions;
	ServerOptions mServerOptions;
};

#endif /* CONFIGREADER_H_ */
