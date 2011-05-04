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

#ifndef MUTEX_H_
#define MUTEX_H_

class Mutex
{
public:
	Mutex();
	virtual ~Mutex();

	/***
	 * Current thread is stopped until it gets a lock
	 */
	void Lock();
	/***
	 * Release lock
	 */
	void UnLock();
private:
	Mutex(const Mutex &);  // No implementation
	Mutex& operator=(const Mutex &);  // No implementation

	pthread_mutex_t *mMutex;
};

#endif /* MUTEX_H_ */
