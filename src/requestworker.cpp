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
#include "urihandler.h"

RequestWorker::RequestWorker()
{
	mUrihandler=new UriHandler();
}

RequestWorker::~RequestWorker()
{
	if (mUrihandler)
		delete mUrihandler;
	mUrihandler=0;
}

/* Worker is responsible to DELETE the request gotten from queue*/
void RequestWorker::DoWork()
{
	/* Handle request until Request queue is closed, IE. returns NULL*/
	const Request* request;
	RequestQueue& requestQueue=RequestQueue::GetInstance();
	while ((request=requestQueue.GetNextRequest()))
	{
		if (request->GetStatus()==Http::HTTP_OK)
		{
			HandleRequest(request);
		}
		else
		{
			HandleFailedRequest(request);
		}

		delete request;
		request=NULL;
	}
}

Http::Status MapFileStatusToHttpStatus(Uri::FileStatus fileStatus);

Http::Status MapFileStatusToHttpStatus(Uri::FileStatus fileStatus)
{
	Http::Status httpStatus=Http::HTTP_OK;
	switch (fileStatus)
	{
	case Uri::FILESTATUS_OK:
		break;
	case Uri::FILESTATUS_NOT_AUTHORIZED:
		httpStatus=Http::HTTP_NO_ACCESS;
		break;
	case Uri::FILESTATUS_NO_FILE:
		httpStatus=Http::HTTP_NOT_FOUND;
		break;
	case Uri::FILESTATUS_INTERNAL_ERROR:
	default:
		httpStatus=Http::HTTP_INTERNAL_SERVER_ERROR;
		break;
	}
	return httpStatus;
}

void RequestWorker::HandleRequest(const Request* request)
{
	const std::string & root=request->GetSite()->GetDocumentRoot();
	const std::string & filename=root+request->GetUri();
	const Uri* uri=mUrihandler->GetFile(filename);
	//TODO: Make SURE no URI-path exploits can happen

	const std::string & contentType =uri->GetContentType();

	Uri::FileStatus status=uri->GetStatus();
	Http::Status httpStatus = MapFileStatusToHttpStatus(status);
	int fd = uri->GetFd();
	size_t contentLength= uri->GetSize();

	Response* response=new Response(request->GetHttpVersion(),request->GetKeepAlive());
	response->SetStatus(httpStatus);
	response->SetFile(fd);
	response->SetContentLength(contentLength);
	response->SetContentType(contentType);

	// Transfer ovnership of response to Connection
	request->GetConnection()->SetResponse(response);
	request->GetConnection()->SetHasData(true);
}



void RequestWorker::HandleFailedRequest(const Request* request)
{
	Response* response=new Response(request->GetHttpVersion(),request->GetKeepAlive());
	response->SetStatus(Http::HTTP_NOT_IMPLEMENTED);
	request->GetConnection()->SetResponse(response);
	request->GetConnection()->SetHasData(true);

}
