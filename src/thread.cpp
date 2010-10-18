/*
 * thread.cpp
 *
 *  Created on: Oct 17, 2010
 *      Author: magnus
 */

#include <pthread.h>
#include "thread.h"

Thread::Thread()
{
	mThread = new pthread_t;
}

Thread::~Thread()
{
	delete mThread;
}

void Thread::DoWork()
{

}



bool Thread::Join()
{
	void** arg;
	pthread_join(*mThread,arg);
	return true;
}



void *Thread::ThreadCallBack(void *arg)
{
	((Thread*)arg)->DoWork();
	return NULL;
}



bool Thread::Stop()
{
}



bool Thread::Start()
{
	pthread_create(mThread,NULL, Thread::ThreadCallBack,(void*)this);
	return true;
}


