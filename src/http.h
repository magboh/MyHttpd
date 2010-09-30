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
	enum Status {HTTP_OK=200,HTTP_NO_ACCESS=403, HTTP_NOT_FOUND=404, HTTP_INTERNAL_SERVER_ERROR=500};

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

	static const std::string & GetStatusString(Status status)
	{
		static const std::string str200 ="OK";
		static const std::string str403 ="NO ACCESS";
		static const std::string str404 ="NOT FOUND";
		static const std::string str500 ="INTERNAL SERVER ERROR";
		switch (status)
			{
			case HTTP_OK: return str200; break;
			case HTTP_NO_ACCESS: return str403; break;
			case HTTP_NOT_FOUND: return str404; break;
			case HTTP_INTERNAL_SERVER_ERROR: return str500; break;

			}

	}

	Http();
	virtual ~Http();
};

#endif /* HTTP_H_ */
