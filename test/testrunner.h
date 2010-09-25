/*
 * testrunner.h
 *
 *  Created on: 2008-nov-08
 *      Author: magnus
 */

#include <string>
#include "cpptest.h"
#ifndef TESTRUNNER_H_
#define TESTRUNNER_H_

/*
 *
 */
class Testrunner : public Test::Suite {
public:
	Testrunner();
	virtual ~Testrunner();
	void RunTests();
private:
	std::string GetCurrentTime();
};

#endif /* TESTRUNNER_H_ */
