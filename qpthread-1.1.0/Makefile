include Make.def

MAJOR_VERSION=1
MINOR_VERSION=1
RELEASE_VERSION=0
LIBNAME_BASE=libqpthr.so
LIBNAME_S=libqpthr.a

#DEBUG_FL=-g

#
# Solaris Sparc, SUNWspro
#
ifeq ($(DEST), Solaris_Sparc)
LIBNAME=$(LIBNAME_BASE).$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION)
LDFLAGS=-G -mt
LDLIBS=
CFLAGS=$(DEBUG_FL) -pta -mt -pic -D_REENTRANT -DARCH_TAS -DSOLARIS -I$(STL_INC) -I.
ARCH_OBJ=arch/sparc/spinlock.o
CXX=CC
CXXLD=CC
INSTALL=/usr/sbin/install
endif

#
# Solaris Sparc, egcs
#
ifeq ($(DEST), Solaris_Sparc_egcs) 
LIBNAME=$(LIBNAME_BASE).$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION) 
LDFLAGS=-shared -Wl 
LDLIBS= 
CFLAGS=$(DEBUG_FL) -fPIC -Wall -O2 -D_REENTRANT  -D_SVID_SOURCE -DARCH_TAS -DSOLARIS -I. 
ARCH_OBJ=arch/sparc/spinlock.o 
CXXLD=gcc 
CXX=g++ 
INSTALL=/usr/bin/install 
endif 

#
# Solaris Intel, SUNWspro
#
ifeq ($(DEST), Solaris_Intel)
LIBNAME=$(LIBNAME_BASE).$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION)
LDFLAGS=-G -mt
LDLIBS=
CFLAGS=$(DEBUG_FL) -pta -mt -pic -D_REENTRANT -DARCH_TAS -DSOLARIS -I$(STL_INC) -I.
ARCH_OBJ=arch/i386/spinlock_sunwspro.o
CXX=CC
CXXLD=CC
INSTALL=/usr/sbin/install
endif

#
# Linux Intel, EGCS
#
ifeq ($(DEST), Linux_Intel)
LIBNAME=$(LIBNAME_BASE).$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION)
LDFLAGS=-shared -Wl,-soname,$(LIBNAME_BASE).$(MAJOR_VERSION)
LDLIBS=
CFLAGS=$(DEBUG_FL) -fPIC -Wall -O2 -D_REENTRANT -D_XOPEN_SOURCE=500 -D_SVID_SOURCE \
		-DARCH_TAS -DLINUX -I.
ARCH_OBJ=arch/i386/spinlock_gnu.o
CXX=g++
CXXLD=g++
INSTALL=/usr/bin/install
endif

#
# Linux Alpha, EGCS
#
ifeq ($(DEST), Linux_Alpha)
LIBNAME=$(LIBNAME_BASE).$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION)
LDFLAGS=-shared -Wl,-soname,$(LIBNAME_BASE).$(MAJOR_VERSION)
LDLIBS=
CFLAGS=$(DEBUG_FL) -fPIC -Wall -O2 -D_REENTRANT -D_XOPEN_SOURCE=500 -D_SVID_SOURCE \
		-DARCH_TAS -DLINUX -I. -fhandle-exceptions
ARCH_OBJ=arch/alpha/spinlock_gnu.o
CXX=g++
CXXLD=g++
INSTALL=/usr/bin/install
endif

#
# Linux Sparc, EGCS
#
ifeq ($(DEST), Linux_Sparc)
LIBNAME=$(LIBNAME_BASE).$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION)
LDFLAGS=-shared -Wl,-soname,$(LIBNAME_BASE).$(MAJOR_VERSION)
LDLIBS=
CFLAGS=$(DEBUG_FL) -fPIC -Wall -O2 -D_REENTRANT -D_XOPEN_SOURCE=500 -D_SVID_SOURCE \
		-DARCH_TAS -DLINUX -I.
ARCH_OBJ=arch/sparc/spinlock_gnu.o
CXX=g++
CXXLD=g++
INSTALL=/usr/bin/install
endif

#
# Linux Other, EGCS
#
ifeq ($(DEST), Linux_Other)
LIBNAME=$(LIBNAME_BASE).$(MAJOR_VERSION).$(MINOR_VERSION).$(RELEASE_VERSION)
LDFLAGS=-shared -Wl,-soname,$(LIBNAME_BASE).$(MAJOR_VERSION)
LDLIBS=
CFLAGS=$(DEBUG_FL) -fPIC -Wall -O2 -D_REENTRANT -D_XOPEN_SOURCE=500 -D_SVID_SOURCE \
		-DLINUX -I.
ARCH_OBJ=
CXX=g++
CXXLD=g++
INSTALL=/usr/bin/install
endif

#
# DEC Alpha, DEC C++ 6.0
#
ifeq ($(DEST), OSF1_Alpha)
LIBNAME=$(LIBNAME_BASE)
LDFLAGS=-shared -set_version $(LIBNAME_BASE).$(MAJOR_VERSION)
LDLIBS=-lrt -pthread
#CFLAGS=$(DEBUG_FL) -pta -pthread -D_REENTRANT -DARCH_TAS -DOSF1 -I.
CFLAGS=-O4 -pta -pthread -D_REENTRANT -DARCH_TAS -DOSF1 -I.
ARCH_OBJ=arch/alpha/spinlock.o arch/alpha/spinlock_nognu.o
CXX=cxx
CXXLD=cxx
INSTALL=/usr/bin/install
endif

all2: $(LIBNAME) links

all: $(LIBNAME) $(LIBNAME_S) links

alls: $(LIBNAME_S)


$(LIBNAME): qp.o qp_base.o qp_synch.o qp_thr.o qp_sig.o \
		qp_timer.o qp_misc.o qp_rwlock.o \
		$(ARCH_OBJ)
	$(CXXLD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(LIBNAME_S): qp.o qp_base.o qp_synch.o qp_thr.o qp_sig.o \
		qp_timer.o qp_misc.o qp_rwlock.o \
		$(ARCH_OBJ)
	ar -rs $@ $^
ifeq ($(DEST), Solaris_Sparc)
	find Templates.DB \( -type f -a -name \*.o \) -print >/tmp/templates.oo
	ar -rs $@ `cat /tmp/templates.oo`
	rm -f /tmp/templates.oo
	ranlib $@
endif
ifeq ($(DEST), Solaris_Intel)
	find Templates.DB \( -type f -a -name \*.o \) -print >/tmp/templates.oo
	ar -rs $@ `cat /tmp/templates.oo`
	rm -f /tmp/templates.oo
	ranlib $@
endif
ifeq ($(DEST), OSF1_Alpha)
	find cxx_repository \( -type f -a -name \*.o \) -print >/tmp/templates.oo
	ar -rs $@ `cat /tmp/templates.oo`
	rm -f /tmp/templates.oo
	ranlib $@
endif

.cc.o:
	$(CXX) ${CFLAGS} -c -o $@ $<

.c.o:
	$(CXX) ${CFLAGS} -c -o $@ $<

links:
	ln -sf $(LIBNAME) $(LIBNAME_BASE)
	ln -sf $(LIBNAME) $(LIBNAME_BASE).$(MAJOR_VERSION)



install:
	@if [ ! -x "$(INSTALL)" ]; then echo set INSTALL variable in Makefile; exit 1; fi
ifeq ($(DEST), Linux_Intel)
	$(INSTALL) -d $(INST_PREFIX)/include/qpthr $(INST_PREFIX)/lib
	$(INSTALL) -m 0644 *.h    $(INST_PREFIX)/include/qpthr
	$(INSTALL) -m 0755 $(LIBNAME)  $(INST_PREFIX)/lib

	ln -sf $(LIBNAME) $(INST_PREFIX)/lib/$(LIBNAME_BASE)
	ln -sf $(LIBNAME) $(INST_PREFIX)/lib/$(LIBNAME_BASE).$(MAJOR_VERSION)

	$(INSTALL) -m 0644 $(LIBNAME_S)  $(INST_PREFIX)/lib
endif
ifeq ($(DEST), Solaris_Intel)
	$(INSTALL) -d $(INST_PREFIX)/include/qpthr $(INST_PREFIX)/lib
	for i in *.h; do \
		$(INSTALL) -m 0644 -f $(INST_PREFIX)/include/qpthr $$i ;\
	done
	$(INSTALL) -m 0755 -f $(INST_PREFIX)/lib $(LIBNAME)  

	ln -s $(LIBNAME) $(INST_PREFIX)/lib/$(LIBNAME_BASE)
	ln -s $(LIBNAME) $(INST_PREFIX)/lib/$(LIBNAME_BASE).$(MAJOR_VERSION)

	$(INSTALL) -m 0644 -f $(INST_PREFIX)/lib $(LIBNAME_S)
endif
ifeq ($(DEST), OSF1_Alpha)
	if [ ! -d $(INST_PREFIX)/include/qpthr ]; then mkdir -p $(INST_PREFIX)/include/qpthr; fi
	if [ ! -d $(INST_PREFIX)/lib ]; then mkdir -p $(INST_PREFIX)/lib; fi
	for i in *.h; do \
		$(INSTALL) -m 0644 -f $(INST_PREFIX)/include/qpthr $$i ;\
	done
	$(INSTALL) -m 0755 -f $(INST_PREFIX)/lib $(LIBNAME)  

	ln -s $(LIBNAME) $(INST_PREFIX)/lib/$(LIBNAME_BASE).$(MAJOR_VERSION)

	$(INSTALL) -m 0644 -f $(INST_PREFIX)/lib $(LIBNAME_S)
endif
	



clean:
	rm -rf *.o core a.out Templates.DB arch/i386/*.o arch/alpha/*.o \
	arch/sparc/*.o so_locations cxx_repository

clean-dist: clean
	rm -rf *.a Make.def
	for i in $(LIBNAME) $(LIBNAME_BASE)* $(LIBNAME_BASE).$(MAJOR_VERSION); do \
		if [ -f $$i -o -h $$i ]; then rm $$i; fi \
	done

cleandist: clean-dist

distclean: clean-dist

dist-clean: clean-dist
