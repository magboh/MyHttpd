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
};

#endif /* CONFIGREADER_H_ */
