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

#ifndef SERVEROPTIONS_H_
#define SERVEROPTIONS_H_

class ServerOptions
{
public:
	ServerOptions();
	virtual ~ServerOptions();
	int GetConnectionQueueSize() const;
	int GetNoConnectionWorkers() const;
	int GetNoRequstWorkers() const;
	int GetRequestBufferSize() const;
	int GetResponseBufferSize() const;
	void SetConnectionQueueSize(int connectionQueueSize);
	void SetNoConnectionWorkers(int noIOWorkers);
	void SetNoRequstWorkers(int noRequstWorkers);
	void SetRequestBufferSize(int requestBufferSize);
	void SetResponseBufferSize(int responseBufferSize);

private:
	int mNoRequstWorkers;
	int mNoIOWorkers;
	int mResponseBufferSize;
	int mRequestBufferSize;
	int mConnectionQueueSize;
};

#endif /* SERVEROPTIONS_H_ */
