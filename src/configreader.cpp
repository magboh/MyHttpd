/*
 * configreader.cpp
 *
 *  Created on: Oct 24, 2010
 *      Author: magnus
 */

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
    	return false ;
   }

    TiXmlElement* levelElement = doc.FirstChildElement("gamedata");
    if (levelElement==0) {
    	LOG("GameDataLoader::No root node 'level' found. Aborting!",Log::MAJOR );
    	return false ;
    }
}
