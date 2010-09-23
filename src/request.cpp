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
	mVersion = HTTP_VERSION_1_1;
	mType= HTTP_GET;
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
	int status=0;
	line=strtok_r((char*)data,"\n",&ptr);

	if (line != NULL)
	{
		char *lptr;
		char *cmd;
		std::cout << "row " << line;
		// GET / POST
		cmd = strtok_r((char*)line," ",&lptr);
		if (cmd)
		{
			if (strcmp(cmd,"GET")==0)
			{
				request->mType = Request::HTTP_GET;
				status++;
			}
			else if (strcmp(cmd,"POST")==0)
			{
				request->mType = Request::HTTP_POST;
				status++;
			}
		}

		//URI
		cmd = strtok_r(NULL," ",&lptr);

		if (cmd && strlen(cmd)<=MAX_URI_LENGTH)
		{
			request->mUri = cmd;
			status++;
		}


		// HTTP Version
		cmd = strtok_r(NULL," ",&lptr);

		if (cmd)
		{
			if(strcmp(cmd,"HTTP/1.1\r")==0)
			{
				request->mVersion = HTTP_VERSION_1_1;
				status++;
			}
			else if(strcmp(cmd,"HTTP/1.0\r")==0)
			{
				request->mVersion = HTTP_VERSION_1_0;
				status++;
			}
		}
	}

	while( (line=strtok_r(NULL,"\n",&ptr)) )
	{
		std::cout << "row " << l++ <<" =" << line <<"\n";
	}

	// IF not enough info retrieved
	if (status < 3 )
	{
		delete request;
		request = NULL;
	}

	return request;
}

std::string Request::ToString()
{
	std::string str = (mType == HTTP_GET) ? "GET" : "POST";
	str+= " " + mUri +" ";
	str+= (mVersion == HTTP_VERSION_1_1) ? "HTTP/1.1" : "HTTP/1.0";
	return str;
}
