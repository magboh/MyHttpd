#include "cpptest.h"

#ifndef MATHTESTSUITE_H_
#define MATHTESTSUITE_H_

/*
 *
 */
class RequestSuite : public Test::Suite {
public:
	RequestSuite();
	virtual ~RequestSuite();

	void TestGood1();
	void TestGood2();
	void TestBADRequest();
	void TestSimpleGet();
	void TestVersionNotSupported();

	void TestHalf();

	void TestManyGoodGets();
	void TestRequestConnectionKeepAlive();

};

#endif /* MATHTESTSUITE_H_ */
