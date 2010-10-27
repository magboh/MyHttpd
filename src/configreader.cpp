/*
 * configreader.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: magnus
 */

#include <map>

#include "tinyxml.h"
#include "configreader.h"

ConfigReader::ConfigReader()
{
	// TODO Auto-generated constructor stub

}

ConfigReader::~ConfigReader()
{
	// TODO Auto-generated destructor stub
}

bool ConfigReader::Load(const std::string & filename)
{

	TiXmlDocument doc(filename);

   if (!doc.LoadFile())
   {
	   std::cout << "ConfigReader::Load() Unable to Load file:"<< filename <<"\n";
    	return false ;
   }

    TiXmlElement* rootElement = doc.FirstChildElement("MyHttpd");

    if (rootElement==0)
    {
    	std::cout << "ConfigReader::Load() No root element 'MyHttpd'\n";
    	return false ;
    }

    TiXmlElement* siteOptionsElement = rootElement->FirstChildElement("DefaultSiteOptions");
    if (siteOptionsElement!=0)
    	ParseSiteOptions(siteOptionsElement);
    return true;
}

bool ConfigReader::ParseSiteOptions(TiXmlElement* element)
{
	assert(element!=0);


	std::map<std::string,std::string> map;
	std::map<std::string,std::string>::iterator it;
	std::stringstream ss;

	TiXmlElement* child = element->FirstChildElement();

	for (child; child!=0; child=child->NextSiblingElement())
	{
		map[child->ValueStr()] = std::string(child->GetText());
	}


	if ( (it=map.find("DefaultFile")) != map.end() )
	{
		mSiteOptions.SetDefaultFile(it->second);
	}

	if ( (it=map.find("AllowDirectoryBrowsing")) != map.end() )
	{
		if (it->second.compare("true") == 0 )
			mSiteOptions.SetAllowDirectoryBrowsing(true);
		else
			mSiteOptions.SetAllowDirectoryBrowsing(false);
	}


	if ( (it=map.find("ConnectionTimeout")) != map.end() )
	{
		int timeout=0;

		ss << it->second;
		ss >> timeout;

		mSiteOptions.SetConnectionTimeout(timeout);
	}

	return true;
}
