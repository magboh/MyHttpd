/*
 * requestqueueworker.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#include "pthread.h"
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "request.h"
#include "requestqueue.h"
#include "requestqueueworker.h"
#include "response.h"
#include "connection.h"

RequestQueueWorker::RequestQueueWorker(RequestQueue* requestQueue)
{
	mThread = new pthread_t;
	mKeepRunning = true;
	mRequestQueue = requestQueue;
}

RequestQueueWorker::~RequestQueueWorker()
{
	delete mThread;
}

/* Worker is responsible to DELETE the request gotten from queue*/
void RequestQueueWorker::HandleRequests()
{
	const Request* request;
	while(mKeepRunning)
	{
		request = mRequestQueue->GetNextRequest();

		if (request != NULL)
		{
			HandleRequest(request);
		}
	}

}

void* RequestQueueWorker::ThreadCallBack(void* arg)
{
	((RequestQueueWorker*)arg)->HandleRequests();
	return (NULL);
}

bool RequestQueueWorker::Start()
{
	bool ok = (pthread_create(mThread,NULL, RequestQueueWorker::ThreadCallBack,(void*)this)==0);
	return ok ;
}

void RequestQueueWorker::HandleRequest(const Request* request)
{
	Response* response  = Response::CreateResponse(request);

	std::string filename = std::string("/home/magnus/") + request->GetUri();

	int fd = open(filename.c_str(),O_RDONLY);
	int error = errno;

	if (fd != -1)
	{
		response->SetFile(fd);
		struct stat fileStat;
		fstat(fd,&fileStat);
		response->SetContentLength(fileStat.st_size);
		response->SetStatus(Http::HTTP_OK);
	}
	else /* Set fail */
	{
		*((char*)0)=1;
		switch (error)
		{
		case EACCES:
			response->SetStatus(Http::HTTP_NO_ACCESS);
			break;
		case ENOENT:
			response->SetStatus(Http::HTTP_NOT_FOUND);
			break;
		default:
			response->SetStatus(Http::HTTP_INTERNAL_SERVER_ERROR);
			break;
		}

	}
	request->GetConnection()->Write(response);
	delete response;
	delete request ;
}
