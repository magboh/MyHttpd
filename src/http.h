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

	enum Version {
		HTTP_VERSION_1_0,
		HTTP_VERSION_1_1
	};

	enum Status {
		HTTP_OK=200,
		HTTP_BAD_REQUEST=400,
		HTTP_NO_ACCESS=403,
		HTTP_NOT_FOUND=404,
		HTTP_REQUEST_TO_LARGE=413,
		HTTP_REQUEST_URI_TO_LONG=414,
		HTTP_INTERNAL_SERVER_ERROR=500,
		HTTP_REQUEST_VERSION_NOT_SUPPORTED=505
	};

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
		static const std::string str400 ="BAD REQUEST";
		static const std::string str403 ="NO ACCESS";
		static const std::string str404 ="NOT FOUND";
		static const std::string str413 ="HTTP REQUEST TOO LARGE";
		static const std::string str414 ="HTTP URI TOO LONG";
		static const std::string str500 ="INTERNAL SERVER ERROR";
		static const std::string str505 ="HTTP VERSION NOT SUPPORTED";
		static const std::string str ="GNU GNU GNU";
		switch (status)
			{
			case HTTP_OK: return str200; break;
			case HTTP_BAD_REQUEST: 	return str400; break;
			case HTTP_NO_ACCESS: return str403; break;
			case HTTP_NOT_FOUND: return str404; break;
			case HTTP_REQUEST_TO_LARGE: return str413; break;
			case HTTP_REQUEST_URI_TO_LONG: return str414; break;
			case HTTP_INTERNAL_SERVER_ERROR: return str500; break;
			case HTTP_REQUEST_VERSION_NOT_SUPPORTED: return str505; break;

			default:
				return str;
			}

	}

	Http() {};
	virtual ~Http() {};

	Http::Version GetHttpVersion()  const
	{
		return mVersion;
	}

	void SetHttpVersion(Version version)
    {
    	mVersion = version ;
    }


	Http::Status GetStatus() const
	{
		return mStatus;
	}

    void SetStatus(Status status)
    {
    	mStatus = status ;
    }

private:
	Http::Version mVersion;
	Http::Status mStatus;
};

#endif /* HTTP_H_ */
