/*
 * testrunner.cpp
 *
 *  Created on: 2008-nov-08
 *      Author: magnus
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ctime>
#include "testrunner.h"
#include "requestsuite.h"

#include <fstream>

using namespace std;


Testrunner::Testrunner() {

}

Testrunner::~Testrunner() {
	// TODO Auto-generated destructor stub
}

void Testrunner::RunTests()
{
	Test::Suite ts;
	ts.add(auto_ptr<Test::Suite>(new RequestSuite));

	Test::HtmlOutput *out=  new Test::HtmlOutput();
	Test::TextOutput *textout = new Test::TextOutput(Test::TextOutput::Terse, cout ) ;

//	out->Output();

	ts.run(*out, true);
	ts.run(*textout,true);
	ofstream* file = new std::ofstream();
	string timestr;
	timestr = GetCurrentTime();
	for(unsigned int i=0;i<timestr.length();i++) {
		if (timestr[i]==' ' || timestr[i]==':' )
			timestr[i]='_';
	}
	string filename = "results/test_"+ timestr + ".html";
	file->open(filename.c_str() , ios::out);

	out->generate(*file, true, "MyHTTPD Tests");
	file->close();

	delete file;
	delete out;
}


std::string Testrunner::GetCurrentTime()
{
    time_t rawtime;
    time ( &rawtime );
    char buff[100];

    #ifdef _WIN32
        ctime_s(buff, 100, &rawtime);
    #else
        ctime_r(&rawtime,buff);
    #endif

    std::string s = std::string(buff);
    return s.substr(0, s.length() -1 );
}



int
main(int argc, char* argv[])
{
  try
    {


		Testrunner *tr = new Testrunner();
		tr->RunTests();
		delete tr ;
	}
	catch (...)
	{
		cout << "unexpected exception encountered\n";
		return -1;
	}
	return 0;
}
