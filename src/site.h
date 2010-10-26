/*
 * site.h
 *
 *  Created on: Oct 26, 2010
 *      Author: magnus
 */

#ifndef SITE_H_
#define SITE_H_

#include <string>

class Site
{
public:
	Site();
	virtual ~Site();



private:

	std::string mDefaultFile;

	std::string mDocRoot;
	in_addr_t mAddress;
	unsigned short mPort ;
};

#endif /* SITE_H_ */
