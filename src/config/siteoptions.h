/*
 * siteoptions.h
 *
 *  Created on: Oct 26, 2010
 *      Author: magnus
 */

#ifndef SITEOPTIONS_H_
#define SITEOPTIONS_H_

#include <string>
#include <netinet/in.h>

class SiteOptions
{
public:


	SiteOptions();
	virtual ~SiteOptions();

    in_addr_t GetIp4Address() const;
    in_port_t GetPort() const;
    bool GetAllowDirectoryBrowsing() const;
    int GetConnectionTimeout() const;
    std::string GetDefaultFile() const;
    const std::string & GetDocumentRoot() const;

    void SetPort(in_port_t port);
    void SetAllowDirectoryBrowsing(bool mAllowDirectoryBrowsing);
    void SetConnectionTimeout(int mConnectionTimeout);
    void SetDefaultFile(std::string mDefaultFile);
    void SetIp4Address(in_addr_t addr);
    void SetDocumentRoot(const std::string & docRoot);
private:

	std::string mDefaultFile;
	std::string mDocumentRoot;
	bool mAllowDirectoryBrowsing;
	int mConnectionTimeout;
	in_addr_t mAddr;
	in_port_t mPort;
};

#endif /* SITEOPTIONS_H_ */
