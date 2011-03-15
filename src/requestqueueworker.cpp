/*
 * requestqueueworker.cpp
 *
 *  Created on: Sep 25, 2010
 *      Author: magnus
 */

#include <iostream>

#include "request.h"
#include "requestqueue.h"
#include "requestqueueworker.h"
#include "response.h"
#include "connection.h"
#include "site.h"
#include "filehandler.h"

RequestQueueWorker::RequestQueueWorker(RequestQueue* requestQueue)
{
	mRequestQueue = requestQueue;
	mFilehandler = new FileHandler();
}

RequestQueueWorker::~RequestQueueWorker()
{
}

/* Worker is responsible to DELETE the request gotten from queue*/
void RequestQueueWorker::DoWork()
{
	const Request* request;
	/* Handle request until Request queue is closed, IE. returns NULL*/
	while( (request = mRequestQueue->GetNextRequest()))
	{
		HandleRequest(request);
		delete request ;
		request = NULL;
	}
}

void RequestQueueWorker::HandleRequest(const Request* request)
{
	Response* response  = Response::CreateResponse(request);

	const std::string & root = request->GetSite()->GetDocumentRoot();

	std::string filename =  root + request->GetUri();


	FileHandler::FileStatus status ;
	const File* file =  mFilehandler->GetFile(filename,status);

	if (file!=NULL)
	{
		response->SetContentLength(file->GetSize());
		response->SetFile(file->GetFd());
		response->SetStatus(Http::HTTP_OK);
	}
	else
	{
		switch (status)
		{
		case FileHandler::FILESTATUS_NOT_AUTHORIZED:
			response->SetStatus(Http::HTTP_NO_ACCESS);
			break;
		case FileHandler::FILESTATUS_NO_FILE:
			response->SetStatus(Http::HTTP_NOT_FOUND);
			break;

		case FileHandler::FILESTATUS_INTERNAL_ERROR:
		default:
			response->SetStatus(Http::HTTP_INTERNAL_SERVER_ERROR);
			break;
		}

	}

	request->GetConnection()->SetResponse(response);
	request->GetConnection()->SetHasData(true);
}

