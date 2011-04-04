#include <string>
#include <string.h>
#include <iostream>

#include "requestsuite.h"
#include "../src/request.h"
#include "../src/bytebuffer.h"
#include "../src/site.h"
#include "../src/logger.h"
#include "../src/config/siteoptions.h"

void RequestSuite::TestGood1() {
	Site site(NULL, NULL);
	Request* r = new Request(NULL, site);
	ByteBuffer* buf = new ByteBuffer(2000);
	char *txt = "GET /good.html HTTP/1.1\r\n\r\n";
	buf->Add(txt, strlen(txt));
	bool result = Request::ParseRequest(r, buf);
	TEST_ASSERT(result);
	delete buf;
	delete r;
}

void RequestSuite::TestBADRequest() {
	TEST_ASSERT(true);
}

RequestSuite::RequestSuite() {

	TEST_ADD(RequestSuite::TestManyGoodGets);
	//TEST_ADD(RequestSuite::TestRequestConnectionKeepAlive);
	sAppLog.SetLogLevel(Logger::DEBUG);

}

RequestSuite::~RequestSuite() {
}

void RequestSuite::TestRequestConnectionKeepAlive() {
	SiteOptions so;
	so.SetPort(50);

	Site site(&so, NULL);

	const int NR = 6;
	const char *gs[] = {
			"GET /good.html HTTP/1.1\r\nConnection: keep-alive\r\n\r\n",
			"GET /good.html HTTP/1.1\r\nConnection: close\r\n\r\n",
			"GET /good.html HTTP/1.1\r\n\r\n",
			"GET /good.html HTTP/1.0\r\nConnection: keep-alive\r\n\r\n",
			"GET /good.html HTTP/1.0\r\nConnection: close\r\n\r\n",
			"GET /good.html HTTP/1.0\r\n\r\n"
	};

	typedef struct {
		const char * getstring;
		bool keepalive;
	} test_data_t;

	test_data_t data[NR] =
			{ { gs[0], true }, { gs[1], false }, { gs[2], true }, { gs[3],true }, { gs[4], false }, { gs[5], false } };

	ByteBuffer* buf = new ByteBuffer(2000);
	for (int i = 0; i < NR; i++) {
		Request *r = new Request(NULL, site);

		buf->Clear();
		buf->Add(data[i].getstring, strlen(data[i].getstring));
		bool result = Request::ParseRequest(r, buf);
		std::cout << "clossss:" << r->GetKeepAlive() << " "<< data[i].keepalive <<  "result=" << result << " status=" << Http::GetStatusString(r->GetStatus()) << "\n";

		TEST_ASSERT(r->GetStatus() == Http::HTTP_OK);
		TEST_ASSERT(r->GetKeepAlive() == data[i].keepalive);
		delete r;
	}
	delete buf;
}

void RequestSuite::TestManyGoodGets() {

	SiteOptions so;
	so.SetPort(50);

	Site site(&so, NULL);
	const int NR = 13;

	const char *getstring[NR] = {
			"GET /good.html HTTP/1.1\r\nConnection: keep-alive\r\n\r\n",
			"GET /good.html HTTP/1.1\r\nConnection: close\r\n\r\n",
			"GET /good.html HTTP/1.0\r\nConnection: keep-alive\r\n\r\n",
			"GET /good.html HTTP/1.0\r\nConnection: close\r\n\r\n",
			"GET /good.html HTTP/1.0\r\n\r\n",
			"GET /good.html HTTP/1.0\r\n\r\n",
			"GET / HTTP/1.1\r\n\r\n",
			"GET / HTTP/1.0\r\n\r\n",
			"GET /last.html HTTP/1.0\r\n\r\n",
			"GET /last.html HTTP/1.0\r\n\r\n",
			"GET /last.html HTTP/1.0\r\n\r\n",
			"GET /last.html HTTP/1.1\r\nConnection:        \t close\r\n\r\n",
			"GET /last.html HTTP/1.0\r\nConnection:        \t close\r\n\r\n"
	};

	ByteBuffer* buf = new ByteBuffer(2000);
	for (int i = 0; i < NR; i++) {
		Request *r  = new Request(NULL, site);
		//std::cout << "getstring[" << i << "]=" << getstring[i];

		buf->Clear();
		buf->Add(getstring[i], strlen(getstring[i]));
		bool result = Request::ParseRequest(r, buf);
		//TEST_ASSERT(result);
		//TEST_ASSERT(r->GetStatus() == Http::HTTP_OK);
		delete r;
	}
//	std::cout << "\n";
	delete buf;
}
