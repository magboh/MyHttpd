/*
 * thread.h
 *
 *  Created on: Oct 17, 2010
 *      Author: magnus
 */

#ifndef THREAD_H_
#define THREAD_H_

class Thread
{
public:
	Thread();
	virtual ~Thread();

	bool Start();
	bool Join();
	bool Stop();
protected:
	virtual void DoWork();
private:
	static void* ThreadCallBack(void* arg);
	pthread_t* mThread;
};

#endif /* THREAD_H_ */
