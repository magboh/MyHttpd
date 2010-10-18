/*
 * myhttpd.h
 *
 *  Created on: Sep 14, 2010
 *      Author: magnus
 */

#ifndef MYHTTPD_H_
#define MYHTTPD_H_
class VirtualServer;
class MyHttpd {
public:
/**
 *
 * @return
 */
	MyHttpd();
/**
 *
 * @return
 */
	virtual ~MyHttpd();
/**
 *
 * @return
 */
	int Start();
	void Stop();
	void SigINTHandler(int signal);

	void AllowSignals();
	void BlockSignals();

	static MyHttpd* myhttpd;
private:
	VirtualServer *mServer;
};

#endif /* MYHTTPD_H_ */
