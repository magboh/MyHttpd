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

#include <pthread.h>
#include "thread.h"

Thread::Thread() : mKeepRunning(false)
{
	mThread=new pthread_t;
}

Thread::~Thread()
{
	delete mThread;
}

bool Thread::Join()
{
	void** arg=NULL;
	pthread_join(*mThread, arg);
	return true;
}

void *Thread::ThreadCallBack(void *arg)
{
	((Thread*) arg)->mKeepRunning = true ;
	((Thread*) arg)->DoWork();
	return NULL;
}

void Thread::Stop()
{
	mKeepRunning = false;
}

bool Thread::Start()
{
	return (pthread_create(mThread, NULL, Thread::ThreadCallBack, (void*) this)==0);
}

