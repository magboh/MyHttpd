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
		switch (request->GetHttpType())
		{
		case Request::HTTP_HEAD:
			HandleHead(request);
			break;
		case Request::HTTP_GET:
			HandleGet(request);
			break;
		default:
			HandleUnsupported(request);
			break;
		}
		delete request;
		request=NULL;
	}
}

void RequestWorker::HandleGet(const Request* request)
{
	Response* response=new Response(request->GetHttpVersion(),request->GetKeepAlive());

	if (request->GetStatus()==Http::HTTP_OK)
	{
		const std::string & root=request->GetSite()->GetDocumentRoot();

		const std::string & filename=root+request->GetUri();

		const Uri* uri=mUrihandler->GetFile(filename);

		Uri::FileStatus status=uri->GetStatus();
		if (status==Uri::FILESTATUS_OK)
		{
			response->SetContentLength(uri->GetSize());
			response->SetFile(uri->GetFd());
			response->SetStatus(Http::HTTP_OK);
			response->SetContentType(uri->GetContentType());
		}
		else
		{
		switch (status)
			{
			case Uri::FILESTATUS_NOT_AUTHORIZED:
				response->SetStatus(Http::HTTP_NO_ACCESS);
				break;
			case Uri::FILESTATUS_NO_FILE:
				response->SetStatus(Http::HTTP_NOT_FOUND);
				break;

			case Uri::FILESTATUS_INTERNAL_ERROR:
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

void RequestWorker::HandleHead(const Request* request)
{
	Response* response=new Response(request->GetHttpVersion(),request->GetKeepAlive());

	if (request->GetStatus()==Http::HTTP_OK)
	{
		const std::string & root=request->GetSite()->GetDocumentRoot();

		const std::string & filename=root+request->GetUri();

		//TODO: Make SURE no URI-path exploits can happen
		const Uri* uri=mUrihandler->GetFile(filename);

			Uri::FileStatus status = uri->GetStatus();
			switch (status)
			{
			case Uri::FILESTATUS_OK:
				response->SetStatus(Http::HTTP_OK);
				break;
			case Uri::FILESTATUS_NOT_AUTHORIZED:
				response->SetStatus(Http::HTTP_NO_ACCESS);
				break;
			case Uri::FILESTATUS_NO_FILE:
				response->SetStatus(Http::HTTP_NOT_FOUND);
				break;

			case Uri::FILESTATUS_INTERNAL_ERROR:
			default:
				response->SetStatus(Http::HTTP_INTERNAL_SERVER_ERROR);
				break;
			}

	}
	request->GetConnection()->SetResponse(response);
	request->GetConnection()->SetHasData(true);
}

void RequestWorker::HandleUnsupported(const Request* request)
{
	Response* response=new Response(request->GetHttpVersion(),request->GetKeepAlive());
	response->SetStatus(Http::HTTP_NOT_IMPLEMENTED);
	request->GetConnection()->SetResponse(response);
	request->GetConnection()->SetHasData(true);
}
