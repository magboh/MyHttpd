#include "requestsuite.h"
#include "../src/request.h"

#include <string>
#include <string.h>
void RequestSuite::TestGood()
{
  Request* r;
  r = Request::ParseRequest((unsigned char*)"",0);
  TEST_ASSERT(r==NULL); 
}

void RequestSuite::TestSimpleGet()
{
  Request* r;
  const char* txt ="GET /localhost:8080 HTTP/1.1\r\n";
  char *t2 = new char[strlen(txt)+1];
  memcpy(t2,txt,strlen(txt)+1);

  r = Request::ParseRequest((unsigned char*)t2,strlen(txt)+1);
  delete t2;
  TEST_ASSERT(r!=NULL);
  if (r) {
	  TEST_ASSERT(r->GetHttpType()==Request::HTTP_GET);
	  TEST_ASSERT(r->GetHttpVersion()==Request::HTTP_VERSION_1_1);
	  TEST_ASSERT(r->GetUri()== std::string("/localhost:8080"));
  }

}
void RequestSuite::TestSimpleGet2()
{
  Request* r;
  const char* txt ="POST abcdefg12312321312312312312321312312312312312312312312312222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222a HTTP/1.0\r\n";
  char *t2 = new char[strlen(txt)+1];
  memcpy(t2,txt,strlen(txt)+1);

  r = Request::ParseRequest((unsigned char*)t2,strlen(txt)+1);
  delete t2;
  TEST_ASSERT(r!=NULL);
  if (r) {
	  TEST_ASSERT(r->GetHttpType()==Request::HTTP_POST);
	  TEST_ASSERT(r->GetHttpVersion()==Request::HTTP_VERSION_1_0);
	  TEST_ASSERT(r->GetUri()== std::string("abcdefg12312321312312312312321312312312312312312312312312222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222a"));
  }

}


RequestSuite::RequestSuite()
{
  TEST_ADD(RequestSuite::TestGood);
  TEST_ADD(RequestSuite::TestSimpleGet);
  TEST_ADD(RequestSuite::TestSimpleGet2);
}
RequestSuite:: ~RequestSuite()
{
}
