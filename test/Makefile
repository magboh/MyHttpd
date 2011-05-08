

all:
	g++ requestsuite.cpp -c
	g++ testrunner.cpp requestsuite.o \
	../Debug/src/request.o \
	../Debug/src/bytebuffer.o \
	../Debug/src/logger.o \
	../Debug/src/site.o \
	../Debug/src/config/siteoptions.o \
	-L/usr/local/lib -lcpptest
clean:
	rm *.o  -rf 