/*
 * site.h
 *
 *  Created on: Oct 26, 2010
 *      Author: magnus
 */

#ifndef SITE_H_
#define SITE_H_

#include <string>
#include <netinet/in.h>

class SiteOptions;
class ConnectionManager;

class Site
{
public:
	Site(const SiteOptions* siteOptions );
	virtual ~Site();

	bool Setup();
	void HandleIncomming();
	const std::string & GetDocumentRoot() const;
	int GetSocket() const { return mSocket ; } ;
private:
	int SetupSocket();

	int mSocket;
	std::string mDefaultFile;

	std::string mDocumentRoot;
	in_addr_t mAddress;
	in_port_t mPort ;
	size_t mListenQueue;
	ConnectionManager* mConnectionManager;
};

#endif /* SITE_H_ */
