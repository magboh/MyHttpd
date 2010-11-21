/*
 * configreader.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: magnus
 */

#include <map>

#include "../tinyxml/tinyxml.h"
#include "configreader.h"

ConfigReader::ConfigReader()
{
	// TODO Auto-generated constructor stub

}

ConfigReader::~ConfigReader()
{
	// TODO Auto-generated destructor stub
}

ConfigReader::LoadStatus ConfigReader::Load(const std::string & filename)
{

	TiXmlDocument doc(filename);
	ConfigReader::LoadStatus status ;

   if (!doc.LoadFile())
   {
	   std::cout << "ConfigReader::Load() Unable to Load file:"<< filename <<"\n";
    	return NO_FILE ;
   }

    TiXmlElement* rootElement = doc.FirstChildElement("MyHttpd");

    if (rootElement==0)
    {
    	std::cout << "ConfigReader::Load() No root element 'MyHttpd'\n";
    	return  BAD_FILE;
    }

    TiXmlElement* siteOptionsElement = rootElement->FirstChildElement("DefaultSiteOptions");
    TiXmlElement* serverOptionsElement = rootElement->FirstChildElement("ServerOptions");
    TiXmlElement* sitesElement = rootElement->FirstChildElement("ServerOptions");

    status = LOAD_OK ;
    if (siteOptionsElement!=0)
    	ParseSiteOptions(siteOptionsElement);
    if (serverOptionsElement!=0)
    	ParseServerOptions(serverOptionsElement);
    if (sitesElement!=0)
    	ParseSites(sitesElement);

    return status;
}

bool ConfigReader::ParseSiteOptions(TiXmlElement* element)
{
	assert(element!=0);


	std::map<std::string,std::string> map;
	std::map<std::string,std::string>::iterator it;
	std::stringstream ss;

	TiXmlElement* child = element->FirstChildElement();

	for (; child!=0; child=child->NextSiblingElement())
	{
		map[child->ValueStr()] = std::string(child->GetText());
	}


	if ( (it=map.find("DefaultFile")) != map.end() )
	{
		mDefaultSiteOptions.SetDefaultFile(it->second);
	}

	if ( (it=map.find("AllowDirectoryBrowsing")) != map.end() )
	{
		if (it->second.compare("true") == 0 )
			mDefaultSiteOptions.SetAllowDirectoryBrowsing(true);
		else
			mDefaultSiteOptions.SetAllowDirectoryBrowsing(false);
	}


	if ( (it=map.find("ConnectionTimeout")) != map.end() )
	{
		int timeout=0;

		ss << it->second;
		ss >> timeout;

		mDefaultSiteOptions.SetConnectionTimeout(timeout);
	}

	return true;
}

bool ConfigReader::ParseServerOptions(TiXmlElement* element)
{
	assert(element!=0);

	std::map<std::string,std::string> map;
	std::map<std::string,std::string>::iterator it;
	std::stringstream ss;

	TiXmlElement* child = element->FirstChildElement();

	for ( ; child!=0; child=child->NextSiblingElement())
	{
		map[child->ValueStr()] = std::string(child->GetText());
	}

	std::string tags[] =
  	{
  			std::string("RequestWorkers"),
  			std::string("IOWorkers"),
  			std::string("RequestBuffer"),
  			std::string("ResponseBuffer"),
  			std::string("ConnectionQueue")
  	};

	for ( int i=0 ; i< 5 ; i++)
	{

		if ( (it=map.find( tags[i] ) )!= map.end())
		{
			int value=0;
			ss.clear();
			ss << it->second;
			ss >> value;

			switch (i)
			{
				case 0: mServerOptions.SetNoRequstWorkers(value); break;
				case 1: mServerOptions.SetNoIOWorkers(value); break;
				case 2: mServerOptions.SetRequestBufferSize(value); break;
				case 3: mServerOptions.SetResponseBufferSize(value); break;
				case 4: mServerOptions.SetConnectionQueueSize(value); break;
			}
		}
	}

	return true;
}

const ServerOptions & ConfigReader::GetServerOptions()
{
	return mServerOptions;
}
