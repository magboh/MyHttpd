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
	enum LoadStatus {LOAD_OK,NO_FILE,BAD_FILE};
	ConfigReader();
	virtual ~ConfigReader();
	ConfigReader::LoadStatus Load(const std::string & filename);

	const std::vector<Site> & GetSites();

	const ServerOptions & GetServerOptions();
private:

	void ParseLogOptions();
	bool ParseSiteOptions(TiXmlElement* element);
	bool ParseServerOptions(TiXmlElement* element);

	void ParseSites();
	void ParseSite();

	void ParseVirtualHostSites();
	void ParseVirtualHostSite();



	std::vector <Site> mSiteVec;
	SiteOptions mDefaultSiteOptions;
	ServerOptions mServerOptions;

};

#endif /* CONFIGREADER_H_ */
