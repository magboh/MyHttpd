#include "requestsuite.h"
#include "../src/request.h"
#include "../src/bytebuffer.h"

#include <string>
#include <string.h>

void RequestSuite::TestGood1()
{
  Request* r = new Request(NULL);
  ByteBuffer* buf = new ByteBuffer(2000);
  char *txt ="GET /good.html HTTP/1.1\r\n\r\n";
		  buf->Add(txt,strlen(txt));
  Request::ParseReturn pr= Request::ParseRequest(r,buf);
  TEST_ASSERT(pr == Request::REQUEST_OK);
  delete buf;
  delete r;
}

void RequestSuite::TestGood2()
{
  Request* r = new Request(NULL);
  ByteBuffer* buf = new ByteBuffer(2000);
  char *txt ="GET /good.html HTTP/1.1\r\nHost:bohman.mine.nu\r\n\r\n";
		  buf->Add(txt,strlen(txt));
  Request::ParseReturn pr= Request::ParseRequest(r,buf);
  TEST_ASSERT(pr == Request::REQUEST_OK);
  delete buf;
  delete r;
}
void RequestSuite::TestBADRequest()
{
	  TEST_ASSERT(true);
}
void RequestSuite::TestHalf()
{
  Request* r = new Request(NULL);
  ByteBuffer* buf = new ByteBuffer(2000);
  char *txt ="GET /testhalf.html HTTP/1.1\r\nHost:bohman.mine.nu\r\n\r\n";
  int len = strlen(txt);

  int i=0;
  for (i;i < len-1; i++)
  {
	  buf->Add(txt+i,1);
	  Request::ParseReturn pr= Request::ParseRequest(r,buf);
	  TEST_ASSERT(pr == Request::REQUEST_UNFINISHED);
  }
  buf->Add(txt+i,1);
  Request::ParseReturn pr= Request::ParseRequest(r,buf);
  TEST_ASSERT(pr == Request::REQUEST_OK);


  delete buf;
  delete r;
}

void RequestSuite::TestVersionNotSupported()
{
	  Request* r = new Request(NULL);
	  ByteBuffer* buf = new ByteBuffer(2000);
	  char *txt ="GET /versionnotsupported.html HTTP/2.1\n\r";
			  buf->Add(txt,strlen(txt));

	  TEST_ASSERT(pr == Request::REQUEST_HTPP_VERSION_NOT_SUPPORTED);
	  delete buf;
	  delete r;
}


RequestSuite::RequestSuite()
{
/*  TEST_ADD(RequestSuite::TestGood1);
  TEST_ADD(RequestSuite::TestGood2);
  TEST_ADD(RequestSuite::TestBADRequest);
  TEST_ADD(RequestSuite::TestVersionNotSupported);*/
  TEST_ADD(RequestSuite::TestHalf);

}
RequestSuite:: ~RequestSuite()
{
}
