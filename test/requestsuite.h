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

	void TestGood();
	void TestSimpleGet();
	void TestSimpleGet2();
};

#endif /* MATHTESTSUITE_H_ */
