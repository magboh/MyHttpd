/*
 * siteoptions.h
 *
 *  Created on: Oct 26, 2010
 *      Author: magnus
 */

#ifndef SITEOPTIONS_H_
#define SITEOPTIONS_H_

class SiteOptions
{
public:


	SiteOptions();
	virtual ~SiteOptions();
    bool GetAllowDirectoryBrowsing() const;
    int GetConnectionTimeout() const;
    std::string GetDefaultFile() const;
    void SetAllowDirectoryBrowsing(bool mAllowDirectoryBrowsing);
    void SetConnectionTimeout(int mConnectionTimeout);
    void SetDefaultFile(std::string mDefaultFile);
private:

	std::string mDefaultFile;

	bool mAllowDirectoryBrowsing;
	int mConnectionTimeout;
};

#endif /* SITEOPTIONS_H_ */
