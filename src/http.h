/*
 * http.h
 *
 *  Created on: Sep 26, 2010
 *      Author: magnus
 */

#ifndef HTTP_H_
#define HTTP_H_
#include <string>
class Http
{
public:
	enum Version {HTTP_VERSION_1_0,HTTP_VERSION_1_1};

	static const std::string & GetVersionString(Version v)
	{
		static const std::string v0 ="HTTP/1.0";
		static const std::string v1 ="HTTP/1.1";
		switch (v)
			{
			case HTTP_VERSION_1_0: return v0; break;
			case HTTP_VERSION_1_1: return v1; break;
			}
	}

	Http();
	virtual ~Http();
};

#endif /* HTTP_H_ */
