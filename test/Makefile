

all:
	g++ requestsuite.cpp -c
	g++ testrunner.cpp requestsuite.o ../Debug/src/request.o ../Debug/src/bytebuffer.o -L/usr/local/lib -lcpptest
clean:
	rm *.o  -rf 