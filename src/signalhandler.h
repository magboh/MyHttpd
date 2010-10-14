/*
 * signalhandler.h
 *
 *  Created on: Oct 14, 2010
 *      Author: magnus
 */

#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

template <class T>
class SignalHandler
{
public:
	virtual ~SignalHandler();
	static SignalHandler* GetInstance();

	void BlockAll();
	void AllowAll();
	typedef void (T::*pFkn)(int);
	void RegisterINTHandler(pFkn fkn );
	void IntHandler(int s);
private:
	SignalHandler();
	pFkn mIntfkn;
};

#endif /* SIGNALHANDLER_H_ */
