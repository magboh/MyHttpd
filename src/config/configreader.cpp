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

#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <netinet/in.h>

#include <algorithm>

#include "../tinyxml/tinyxml.h"
#include "configreader.h"

int GetIntValue(TiXmlElement* element);
in_addr_t GetIp4Value(TiXmlElement* element);
std::string GetString(TiXmlElement* element);
bool GetBool(TiXmlElement* element);

int GetIntValue(TiXmlElement* element)
{
	int value=0;
	std::stringstream ss;
	ss<<element->GetText();
	ss>>value;
	return value;
}

bool GetBool(TiXmlElement* element)
{
	bool value=false;
	std::string s=element->GetText();
	std::transform(s.begin(),s.end(),s.begin(),::tolower);

	if (s.compare("true")==0)
	{
		value=true;
	}
	return value;
}
in_addr_t GetIp4Value(TiXmlElement* element)
{
	std::stringstream ss;
	std::istringstream ss2(element->GetText());
	std::string token;
	in_addr_t addr=0;
	int i=3;
	while (getline(ss2,token,'.'))
	{
		ss.clear();
		ss<<token;
		unsigned int a;
		ss>>a;
		addr|=(unsigned char) a<<(i--)*8;
	}
	return addr;
}

std::string GetString(TiXmlElement* element)
{
	return std::string(element->GetText());
}

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
	ConfigReader::LoadStatus status;

	if (!doc.LoadFile())
	{
		std::cout<<"ConfigReader::Load() Unable to Load file:"<<filename<<"\n";
		return NO_FILE;
	}

	TiXmlElement* rootElement=doc.FirstChildElement("MyHttpd");

	if (rootElement==0)
	{
		std::cout<<"ConfigReader::Load() No root element 'MyHttpd'\n";
		return BAD_FILE;
	}

	TiXmlElement* defaultSiteOptionsElement=rootElement->FirstChildElement("DefaultSiteOptions");
	TiXmlElement* serverOptionsElement=rootElement->FirstChildElement("ServerOptions");
	TiXmlElement* siteOptionsElement=rootElement->FirstChildElement("Sites");

	status=LOAD_OK;
	if (defaultSiteOptionsElement!=0)
		ParseDefaultSiteOptions(defaultSiteOptionsElement);
	if (serverOptionsElement!=0)
		ParseServerOptions(serverOptionsElement);

	if (siteOptionsElement!=0)
		ParseSites(siteOptionsElement);

	return status;
}

bool ConfigReader::ParseDefaultSiteOptions(TiXmlElement* element)
{
	assert(element!=0);

	std::map<std::string, std::string> map;
	std::map<std::string, std::string>::iterator it;
	std::stringstream ss;

	TiXmlElement* child=element->FirstChildElement();

	for (;child!=0;child=child->NextSiblingElement())
	{
		map[child->ValueStr()]=std::string(child->GetText());
	}

	if ((it=map.find("DefaultFile"))!=map.end())
	{
		mDefaultSiteOptions.SetDefaultFile(it->second);
	}

	if ((it=map.find("AllowDirectoryBrowsing"))!=map.end())
	{
		if (it->second.compare("true")==0)
			mDefaultSiteOptions.SetAllowDirectoryBrowsing(true);
		else
			mDefaultSiteOptions.SetAllowDirectoryBrowsing(false);
	}

	if ((it=map.find("ConnectionTimeout"))!=map.end())
	{
		int timeout=0;

		ss<<it->second;
		ss>>timeout;

		mDefaultSiteOptions.SetConnectionTimeout(timeout);
	}

	return true;
}

bool ConfigReader::ParseSites(TiXmlElement* element)
{
	assert(element!=0);

	TiXmlElement* child=element->FirstChildElement();

	for (;child!=0;child=child->NextSiblingElement())
	{
		SiteOptions so=mDefaultSiteOptions;

		if (ParseSiteOptions(child,&so))
		{
			mSiteOptionsVec.push_back(so);
		}
	}
	return true;
}

bool ConfigReader::ParseSiteOptions(TiXmlElement* element, SiteOptions *siteOptions)
{

	TiXmlElement* child=0;

	if ((child=element->FirstChildElement("DefaultFile")))
		siteOptions->SetDefaultFile(GetString(child));
	child=0;

	if ((child=element->FirstChildElement("AllowDirectoryBrowsing")))
		siteOptions->SetAllowDirectoryBrowsing(GetBool(child));
	child=0;

	if ((child=element->FirstChildElement("ConnectionTimeout")))
		siteOptions->SetConnectionTimeout(GetIntValue(child));
	child=0;

	if ((child=element->FirstChildElement("Port")))
		siteOptions->SetPort(GetIntValue(child));
	child=0;

	if ((child=element->FirstChildElement("IP4-Address")))
		siteOptions->SetIp4Address(GetIp4Value(child));
	child=0;

	if ((child=element->FirstChildElement("DocRoot")))
		siteOptions->SetDocumentRoot(GetString(child));
	child=0;

	return true;
}

bool ConfigReader::ParseServerOptions(TiXmlElement* element)
{
	TiXmlElement* child=NULL;

	element->FirstChild("RequestWorkers");
	if (child)
		mServerOptions.SetNoRequstWorkers(GetIntValue(child));

	child=element->FirstChildElement("IOWorkers");
	if (child)
		mServerOptions.SetNoIOWorkers(GetIntValue(child));

	child=element->FirstChildElement("RequestBuffer");
	if (child)
		mServerOptions.SetRequestBufferSize(GetIntValue(child));

	child=element->FirstChildElement("ResponseBuffer");
	if (child)
		mServerOptions.SetResponseBufferSize(GetIntValue(child));

	child=element->FirstChildElement("ConnectionQueue");
	if (child)
		mServerOptions.SetConnectionQueueSize(GetIntValue(child));
	return true;
}

const ServerOptions & ConfigReader::GetServerOptions()
{
	return mServerOptions;
}

const std::vector<SiteOptions> & ConfigReader::GetSiteOptions() const
{
	return mSiteOptionsVec;
}

