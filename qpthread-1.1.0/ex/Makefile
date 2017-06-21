include ../Make.def

#DEBUG_FL=-g

#
# Solaris Sparc, SUNWspro
#
ifeq ($(DEST), Solaris_Sparc)
LIBNAME=libqpthr.so
LDFLAGS=-mt
LDLIB=-lposix4 -lpthread
CFLAGS=$(DEBUG_FL) -pta -mt -D_REENTRANT -I../$(STL_INC) -I..
CXX=CC
endif

#
# Solaris Intel, SUNWspro
#
ifeq ($(DEST), Solaris_Intel)
LIBNAME=libqpthr.so
LDFLAGS=-pta -mt
LDLIB=-lposix4 -lpthread
CFLAGS=$(DEBUG_FL) -pta -mt -D_REENTRANT -I../$(STL_INC) -I..
CXX=CC
endif

#
# Linux Intel
#
ifeq ($(DEST), Linux_Intel)
LIBNAME=libqpthr.so.0
LDFLAGS=
LDLIB=-lpthread -lstdc++
CFLAGS=$(DEBUG_FL) -Wall -O2 -D_REENTRANT -I..
CXX=g++
endif

#
# Linux Alpha
#
ifeq ($(DEST), Linux_Alpha)
LIBNAME=libqpthr.so.0
LDFLAGS=
LDLIB=-lpthread -lstdc++
CFLAGS=$(DEBUG_FL) -Wall -O2 -D_REENTRANT -I..
CXX=g++
endif

#
# Linux Sparc
#
ifeq ($(DEST), Linux_Sparc)
LIBNAME=libqpthr.so.0
LDFLAGS=
LDLIB=-lpthread -lstdc++
CFLAGS=$(DEBUG_FL) -Wall -O2 -D_REENTRANT -I..
CXX=g++
endif

#
# DEC Alpha, DEC C++ 6.0
#
ifeq ($(DEST), OSF1_Alpha)
LIBNAME=libqpthr.so
LDFLAGS=
LDLIB=-lrt -pthread
#CFLAGS=$(DEBUG_FL) -pta -pthread -D_REENTRANT -I..
CFLAGS=-O4 -pta -pthread -D_REENTRANT -I..
CXX=cxx
endif

#LIBNAME=libqpthr.a

QPTHRLIB=-lqpthr

PROG=hello hellorun synchperf queue sleep hellopool signal timer pool cancel \
	threadtest stack once tsd tsdt rwlock

all: ${PROG}


.cc.o:
	$(CXX) ${CFLAGS} -c -o $@ $<

ifeq ($(DEST), Solaris_Intel)
synchperf.o: synchperf.cc
	$(CXX) ${CFLAGS} -DZERO_RUSAGE -c -o $@ $<
endif

hello: hello.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

hellorun: hellorun.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

synchperf: synchperf.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

queue: queue.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

sleep: sleep.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

hellopool: hellopool.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

signal: signal.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB) -lm

timer: timer.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

pool: pool.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

cancel: cancel.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

threadtest: threadtest.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

stack: stack.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

once: once.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

tsd: tsd.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

tsdt: tsdt.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)

rwlock: rwlock.o 
	$(CXX) ${LDFLAGS} -o $@ $< -L.. $(QPTHRLIB) $(LDLIB)


clean:
	rm -rf *.o core a.out Templates.DB cxx_repository ${PROG}

cleandist: clean

clean-dist: clean

dist-clean: clean

distclean: clean
