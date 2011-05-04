#include <string>
#include <string.h>
#include <iostream>

#include "requestsuite.h"
#include "../src/request.h"
#include "../src/bytebuffer.h"
#include "../src/site.h"
#include "../src/logger.h"
#include "../src/config/siteoptions.h"

RequestSuite::RequestSuite() {
	sAppLog.SetLogLevel(Logger::DEBUG);
#if 1
	TEST_ADD(RequestSuite::TestManyGoodGets);

	TEST_ADD(RequestSuite::TestRequestConnectionKeepAlive);
//	TEST_ADD(RequestSuite::TestEvilRequests);
	TEST_ADD(RequestSuite::TestMandatoryHeaderMissing);
//	TEST_ADD(RequestSuite::TestMultilineHeaders);
#endif
	TEST_ADD(RequestSuite::TestReqeustsByteForByte);
}

RequestSuite::~RequestSuite() {
}

void RequestSuite::TestRequestConnectionKeepAlive() {
	SiteOptions so;
	so.SetPort(50);

	Site site(&so);

	const int NR = 6;
	const char
			*gs[] =
					{
							"GET /good.html HTTP/1.1\r\nConnection: keep-alive\r\nHost: localhost:8080\r\n\r\n",
							"GET /good.html HTTP/1.1\r\nConnection: close\r\nHost: localhost:8080\r\n\r\n",
							"GET /good.html HTTP/1.1\r\nHost: localhost:8080\r\n\r\n",
							"GET /good.html HTTP/1.0\r\nConnection:   \t keep-alive\r\n\r\n",
							"GET /good.html HTTP/1.0\r\nConnection: close\r\n\r\n",
							"GET /good.html HTTP/1.0\r\n\r\n" };

	typedef struct {
		const char * getstring;
		bool keepalive;
	} test_data_t;

	test_data_t data[NR] = { { gs[0], true }, { gs[1], false },
			{ gs[2], true }, { gs[3], true }, { gs[4], false },
			{ gs[5], false } };

	ByteBuffer* buf = new ByteBuffer(2000);
	for (int i = 0; i < NR; i++) {
		Request *r = new Request(NULL, &site);

		buf->Clear();
		buf->Add(data[i].getstring, strlen(data[i].getstring));
		bool result = Request::ParseRequest(r, buf);
		TEST_ASSERT(r->GetStatus() == Http::HTTP_OK);
		TEST_ASSERT(r->GetKeepAlive() == data[i].keepalive);
		delete r;
	}
	delete buf;
}

void RequestSuite::TestManyGoodGets() {

	SiteOptions so;
	so.SetPort(50);

	Site site(&so);
	const int NR = 13;

	const char
			*getstring[NR] =
					{
							"GET /good.html HTTP/1.1\r\nConnection: keep-alive\r\nHost: \t   localhost:80\r\n\r\n",
							"GET /good.html HTTP/1.1\r\nConnection: close\r\nHost:    localhost:81\r\n\r\n",
							"GET /good.html HTTP/1.0\r\nConnection: keep-alive\r\n\r\n",
							"GET /good.html HTTP/1.0\r\nConnection: close\r\n\r\n",
							"GET /good.html HTTP/1.0\r\n\r\n",
							"GET /good.html HTTP/1.0\r\n\r\n",
							"GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n",
							"GET / HTTP/1.0\r\n\r\n",
							"GET /last.html HTTP/1.0\r\nHost: localhost:8080\r\n\r\n",
							"GET /last.html HTTP/1.0\r\n\r\n",
							"GET /last.html HTTP/1.0\r\n\r\n",
							"GET /last.html HTTP/1.1\r\nHost: localhost:8080\r\nConnection:        \t close\r\n\r\n",
							"GET /last.html HTTP/1.0\r\nConnection:        \t close\r\n\r\n" };

	ByteBuffer* buf = new ByteBuffer(2000);
	for (int i = 0; i < NR; i++) {
		Request *r = new Request(NULL, &site);
		buf->Clear();
		buf->Add(getstring[i], strlen(getstring[i]));
		bool result = Request::ParseRequest(r, buf);
		TEST_ASSERT(result);
		TEST_ASSERT(r->GetStatus() == Http::HTTP_OK);
		delete r;
	}
	delete buf;
}

void RequestSuite::TestEvilRequests() {
	SiteOptions so;
	so.SetPort(50);

	Site site(&so);
	const int NR = 3;

	const char
			*getstring[NR] =
					{
							",                                                                                          \r\n",
							"\r\n\r\n\r\n", "\0" };

	ByteBuffer* buf = new ByteBuffer(2000);
	for (int i = 0; i < NR; i++) {
		Request *r = new Request(NULL, &site);
		buf->Clear();
		buf->Add(getstring[i], strlen(getstring[i]));
		bool result = Request::ParseRequest(r, buf);
		TEST_ASSERT(result==false);
		TEST_ASSERT(r->GetStatus() != Http::HTTP_OK);
		delete r;
	}
	delete buf;

}

void RequestSuite::TestMandatoryHeaderMissing() {
	SiteOptions so;
	so.SetPort(50);

	Site site(&so);
	const int NR = 1;

	const char *getstring[NR] = {
			"GET /good.html HTTP/1.1\r\nConnection: keep-alive\r\n\r\n", };

	ByteBuffer* buf = new ByteBuffer(2000);
	for (int i = 0; i < NR; i++) {
		Request *r = new Request(NULL, &site);
		buf->Clear();
		buf->Add(getstring[i], strlen(getstring[i]));
		bool result = Request::ParseRequest(r, buf);
		TEST_ASSERT(result==true);
		TEST_ASSERT(r->GetStatus() == Http::HTTP_BAD_REQUEST);
		delete r;
	}
	delete buf;

}

void RequestSuite::TestMultilineHeaders() {
	SiteOptions so;
	so.SetPort(50);

	Site site(&so);
	const int NR = 2;

	const char
			*getstring[NR] =
					{
							"GET /good.html HTTP/1.1\r\nHost: keep-alive\r\nMyHeader: apa\r\n Gnu\r\n\r\n",
							"GET /good.html HTTP/1.1\r\nHost: keep-alive\r\nMyHeader: apa\r\n Gnu\r\n Gnu2\r\n\r\n", };

	ByteBuffer* buf = new ByteBuffer(2000);
	for (int i = 0; i < NR; i++) {
		Request *r = new Request(NULL, &site);
		buf->Clear();
		buf->Add(getstring[i], strlen(getstring[i]));
		bool result = Request::ParseRequest(r, buf);
		TEST_ASSERT(result==true);
		TEST_ASSERT(r->GetStatus() == Http::HTTP_OK);
		delete r;
	}
	delete buf;

}


void RequestSuite::TestReqeustsByteForByte() {

	SiteOptions so;
	so.SetPort(50);

	Site site(&so);
	const int NR = 11;

	const char
			*getstring[NR] =
					{
							"GET / HTTP/1.0\r\n\r\n",
							"GET /good.html HTTP/1.1\r\nConnection: keep-alive\r\nHost: \t   localhost:80\r\n\r\n",
							"GET /good.html HTTP/1.1\r\nConnection: close\r\nHost:    localhost:81\r\n\r\n",
							"GET /good.html HTTP/1.0\r\nConnection: keep-alive\r\n\r\n",
							"GET /good.html HTTP/1.0\r\nConnection: close\r\n\r\n",
							"GET /good.html HTTP/1.0\r\n\r\n",
							"GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n",
							"GET /last.html HTTP/1.0\r\nHost: localhost:8080\r\n\r\n",
							"GET /last.html HTTP/1.0\r\n\r\n",
							"GET /last.html HTTP/1.1\r\nHost: localhost:8080\r\nConnection:        \t close\r\n\r\n",
							"GET /last.html HTTP/1.0\r\nConnection:        \t close\r\n\r\n" };

	ByteBuffer* buf = new ByteBuffer(2000);
	for (int i = 0; i < NR; i++) {
		Request *r = new Request(NULL, &site);
		buf->Clear();
		size_t len=strlen(getstring[i]);
		for (size_t j=0;j<len;j++)
		{
			buf->Add(getstring[i]+j, 1);
			bool result = Request::ParseRequest(r, buf);
			if (j+1<len)
			{
				TEST_ASSERT_MSG(result==false,getstring[i]);
			}
			else
			{
				TEST_ASSERT_MSG(result,getstring[i]);
				TEST_ASSERT_MSG(r->GetStatus() == Http::HTTP_OK,getstring[i]);
			}
		}
		delete r;
	}
	delete buf;
}
