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

	// Resturns 200
	void TestManyGoodGets();

	// Returns 200 also check so keep-alive is correct
	void TestRequestConnectionKeepAlive();

	// Should return status 501
	void TestUnsupportedMethod();

	// returns 505
	void TestUnsupportedVersion();

	// returns 400
	void TestMandatoryHeaderMissing();

	// should be no problem
	void TestReqeustsByteForByte();

	// Dont crash.. Handle graceful, or return 500 from a try {} catch{}
	void TestEvilRequests();

	void TestMultilineHeaders();

	// Returns 414
	void TestLongURI();

	// returns 413
	void TestLongRequests();

};

#endif /* MATHTESTSUITE_H_ */
