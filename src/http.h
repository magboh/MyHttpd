/*
 * http.h
 *
 *  Created on: Sep 26, 2010
 *      Author: magnus
 */

#ifndef HTTP_H_
#define HTTP_H_

class Http
{
public:
	enum Version {HTTP_VERSION_1_0,HTTP_VERSION_1_1};
	Http();
	virtual ~Http();
};

#endif /* HTTP_H_ */
