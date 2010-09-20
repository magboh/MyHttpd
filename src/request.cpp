/*
 * request.cpp
 *
 *  Created on: Sep 17, 2010
 *      Author: magnus
 */

#include <string>
#include <string.h>
#include <iostream>
#include "request.h"

Request::Request() {
	// TODO Auto-generated constructor stub
	mHost="";
	mUri="";
	mVersion="";
	mKeepAlive=false;

}

Request::~Request() {
	// TODO Auto-generated destructor stub
}

/**
 *
 * @param data
 * @param size
 * @return
 */
Request* Request::ParseRequest(unsigned char* data, size_t size)
{
	Request* request = new Request();
	char *ptr;
	char *line;
	int l=0;
	line=strtok_r((char*)data,"\n",&ptr);

	while( line != NULL )
	{
		std::cout << "row " << l++ <<" =" << line <<"\n";
		line=strtok_r(NULL,"\n",&ptr);

		if (strncmp(line,"GET",3)==0)
		{
			request->mType = Request::HTTP_GET;
		}

	}
	return request;
}
