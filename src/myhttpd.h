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
	int Start();

private:
	VirtualServer *mServer;
	static void SigINTHandler(int signal);
};

#endif /* MYHTTPD_H_ */
