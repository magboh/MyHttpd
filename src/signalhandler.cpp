/*
 * signalhandler.cpp
 *
 *  Created on: Oct 14, 2010
 *      Author: magnus
 */
#include "myhttpd.h"
#include "signalhandler.h"
#include  <signal.h>
#include <pthread.h>


void IntHandler(int s);

template <class T>
void IntHandler(int s)
{
	SignalHandler<T>::GetInstance()->IntHandler();
}






template <class T>
SignalHandler<T>::~SignalHandler()
{
	// TODO Auto-generated destructor stub
}

template <class T>
SignalHandler<T>::SignalHandler()
{
	// TODO Auto-generated constructor stub

}

template <class T>
SignalHandler<T>* SignalHandler<T>::GetInstance()
{
	static SignalHandler<MyHttpd> sInstance;
	return &sInstance;
}


template <class T>
void SignalHandler<T>::RegisterINTHandler(pFkn fkn)
{
	mIntfkn = fkn;
}

template <class T>
void SignalHandler<T>::BlockAll()
{
	sigset_t set;
	sigfillset(&set);
	pthread_sigmask(SIG_BLOCK,&set,NULL);

}
template <class T>
void SignalHandler<T>::AllowAll()
{
	sigset_t set;
	sigfillset(&set);
	pthread_sigmask(SIG_UNBLOCK,&set,NULL);

}

template <class T>
void SignalHandler<T>::IntHandler(int s)
{
	mIntfkn(s);
}

