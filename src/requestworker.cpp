/***************************************************************************
 *      MyHTTPd
 *
 *      Tue, 15 Mar 2011 22:16:12 +0100
 *      Copyright 2011 Magnus Bohman
 *      magnus.bohman@gmail.com
 ***************************************************************************/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, US.
 */

#include "request.h"
#include "requestqueue.h"
#include "requestworker.h"
#include "response.h"
#include "connection.h"
#include "site.h"
#include "filehandler.h"

RequestWorker::RequestWorker(RequestQueue* requestQueue)
{
	mRequestQueue=requestQueue;
	mFilehandler=new FileHandler();
}

RequestWorker::~RequestWorker()
{
	if (mFilehandler)
		delete mFilehandler;
	mFilehandler=0;
}

/* Worker is responsible to DELETE the request gotten from queue*/
void RequestWorker::DoWork()
{
	const Request* request;
	/* Handle request until Request queue is closed, IE. returns NULL*/
	while ((request=mRequestQueue->GetNextRequest()))
	{
		HandleRequest(request);
		delete request;
		request=NULL;
	}
}

void RequestWorker::HandleRequest(const Request* request)
{
	Response* response=Response::CreateResponse(request);

	if (request->GetStatus()==Http::HTTP_OK)
	{
		const std::string & root=request->GetSite().GetDocumentRoot();

		std::string filename=root+request->GetUri();

		FileHandler::FileStatus status;
		const File* file=mFilehandler->GetFile(filename,status);

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
	}
	else
	{
		response->SetStatus(request->GetStatus());
	}
	request->GetConnection()->SetResponse(response);
	request->GetConnection()->SetHasData(true);
}

