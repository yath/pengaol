# $Id: Makefile,v 1.1.1.1 2001/08/22 12:57:05 birdy57 Exp $
#----------------------------------------------------------------------
# C O M P I L A T I O N :
#----------------------------------------------------------------------
#
# To compile it all, just type:
#
#   $ make
#
# To force a complete recompile, do:
#
#   $ make clobber all
#
# If you don't have X server/libraries, compile as follows:
#
#   $ make no_x
#
#----------------------------------------------------------------------
# I N S T A L L A T I O N :
#----------------------------------------------------------------------
# 
# To install everything (wavplay, wavrec and xltwavplay):
# 
# # make install
# 
# To install wavplay and wavrec only (no X based programs):
# 
# # make install_no_x
#
# To add realtime scheduling do:
#
# # make setuid_root

#----------------------------------------------------------------------
# C O N F I G U R A T I O N   S E C T I O N :
#----------------------------------------------------------------------

#----------------------------------------------------------------------
# If you are running a 386 you should comment this line out.
#
CPU=-m486

#----------------------------------------------------------------------
# If you are experiencing troubles (like core dumps!), then uncomment
# the DEBUG= line and comment out the OPT= line, and recompile. When
# turning off debug, you may want to uncomment the OPT= line. Also
# adjust the STRIP option to suit compile mode:
#
#DEBUG=-g
OPT=-O2

#----------------------------------------------------------------------
# When using 'DEBUG=-g' above, then comment out the 'strip' line:
#
STRIP=strip
#STRIP=echo

#----------------------------------------------------------------------
# This parameter indicates where the program binaries should be
# placed (wavplay, wavrec, and xltwavplay)
#
INSTDIR=/usr/local/bin

#----------------------------------------------------------------------
# Uncomment this line if you want to install the xltwavplay.res resource
# file into your resource file(s) directory, instead of relying on the
# builtin fallback resources in the program itself. If you don't know
# what this is all about, then leave the next line commented out.
#
#USERES=-DINSTALLED_RESOURCE_FILE

#----------------------------------------------------------------------
# If the resource file is to be installed, indicate where. If the above
# parameter is commented out, then this parameter is ignored.
#
RESDIR=/usr/X11R6/lib/X11/app-defaults

#----------------------------------------------------------------------
# If you don't have libXmu.so (which you should have) or you simply
# don't want EDITRES capability in xltwavplay, then uncomment this
# to disable the EDITRES facility. If you're not compiling xltwavplay
# then this setting can be left asis.
#
#NOEDITRES=-DNO_EDITRES

#----------------------------------------------------------------------
# If you COMMENTED OUT the NOEDITRES line, then UNCOMMENT this line.
# If you UNCOMMENTED the NOEDITRES line, then COMMENT out this line.
# (ie. do the opposite of NOEDITRES)
#
LIBXMU=-lXmu

#----------------------------------------------------------------------
# These are the non X load options (for wavplay/wavrec)
#
LDOPTS=

#----------------------------------------------------------------------
# These are the MOTIF/LessTif + X load options (for xltwavplay)
# Make sure that -lXm is always placed before -lXt
#
XLDOPTS=-L/usr/X11R6/lib/ -L/usr/local/lesstif/lib -lXm $(LIBXMU) -lXt -lX11 

#----------------------------------------------------------------------
# When UNcommented, this causes the wavplay/wavrec programs to be compiled to use
# only the standard getopt() routine. When commented out, the long options like --help
# are recognized, with the help of the getopt_long() routine.
#
#STDGETOPTS= -DUSE_GETOPT_STD

#----------------------------------------------------------------------
# The next option specifies the pathname that the xltwavplay X client should exec
# when starting the wavplay server program. This should be a full pathname.
#
# Environment variable WAVPLAYPATH will override this value at runtime, if
# present.
#
PLAYPATH= -DWAVPLAYPATH=\"$(INSTDIR)/wavplay\"

#----------------------------------------------------------------------
# Minimum DSP sampling rate (default is 4000)
#
#DSPMIN= -DDSP_MIN=4000

#----------------------------------------------------------------------
# Maximum DSP sampling rate (default is 48000)
#
#DSPMAX= -DDSP_MAX=48000

#----------------------------------------------------------------------
# Pathname of the audio device (default is /dev/dsp)
#
# Environment variable DSPPATH will override this value at runtime, if
# present.
#
#DSPPATH= -DAUDIODEV=\"/dev/dsp\"

#----------------------------------------------------------------------
# Semaphore IPC Key for the Play & Record locks (Default is 0x33333333)
#
# Environment variable DSPLOCKS will override this value at runtime, if
# present.
#
#DSPLOCKS= -DAUDIOLCK=0x33333333

#----------------------------------------------------------------------
# Comment out this next parameter, if your kernel does not support
# the sched_setscheduler() call (probably all kernels prior to 2.x.x)
# The priority must be in the range of 1-99. Note however, that this
# requires root privileges to do, and so wavplay must run setuid root.
#
SCHED_PRIORITY=-DSCHED_PRIORITY=9

#----------------------------------------------------------------------
# Only for unusual sites would you change this: the name of the
# install program. (has no impact if you choose to manually install)
#
INSTALL=install

#----------------------------------------------------------------------
# If you have any custom include directories, like for X or
# MOTIF/LessTif files, uncomment, and modify to suit (but normally
# this should be unecessary if the appropriate symlinks are
# installed).
#
#CUSTINCL= -I/usr/X11R6/include/X11 -I/usr/local/lesstif/include/Xm

#----------------------------------------------------------------------
# Normally, you shouldn't need to change anything beyond this point:
#----------------------------------------------------------------------
.SUFFIXES: .c .cc .h .o

CUSTMOPTS=$(STDGETOPTS) $(PLAYPATH) $(DSPMIN) $(DSPMAX) $(DSPPATH) $(DSPLOCKS) $(USERES) $(NOEDITRES) $(SCHED_PRIORITY)
ELF=	# -fPIC
INCL=	-I. $(CUSTINCL)
CCFLAGS= -c $(DEBUG)$(OPT) $(CPU) $(ELF) -fhandle-exceptions $(CUSTMOPTS) -Wall -Wstrict-prototypes
CFLAGS= -c $(DEBUG)$(OPT) $(CPU) $(ELF) -Wall -Wstrict-prototypes $(CUSTMOPTS)
CC=	gcc
CP=	g++

HDRFIL= client.h server.h wavfile.h wavplay.h xltwavplay.h xmsprint.h
SVROBJ=	main.o file.o recplay.o wavfile.o locks.o msg.o server.o
CLNOBJ=	client.o msg.o xmsprint.o xltwavplay.o menu.o procterm.o save_as.o
WVDMPO= wavdump.o

.cc.o:
	$(CP) $(CCFLAGS) $(INCL) $<

.c.o:
	$(CC) $(CFLAGS) $(INCL) $<

all:	wavplay wavrec xltwavplay

no_x:	wavplay wavrec

# wavdump: $(WVDMPO)
#	$(CP) $(WVDMPO) $(LDOPTS) $(DEBUG) -o wavdump -lRIFF

wavplay: $(SVROBJ)
	$(CC) $(SVROBJ) $(LDOPTS) $(DEBUG) -o wavplay
	ls -l wavplay

wavrec:	wavplay
	rm -f wavrec
	ln wavplay wavrec

xltwavplay: $(CLNOBJ) xltwavplay.o
	$(CC) $(CLNOBJ) $(XLDOPTS) $(DEBUG) -o xltwavplay
	$(STRIP) xltwavplay
	ls -l xltwavplay

install: install_wavplay install_xltwavplay

install_no_x: install_wavplay

install_wavplay: wavplay wavrec
	@[ ! -d "$(INSTDIR)" ] && echo "You do not have a directory $(INSTDIR); Check INSTDIR in make file." && exit 13 || :
	$(INSTALL) -o root -g root -m 555 wavplay $(INSTDIR)
	rm -f $(INSTDIR)/wavrec
	ln $(INSTDIR)/wavplay $(INSTDIR)/wavrec
	@[ ! -z "$(SCHED_PRIORITY)" ] && echo "*** DO 'make setuid_root' IF YOU WANT REAL TIME SCHEDULING ***" || :
	@echo

xltwavplay_test: wavplay xltwavplay
	WAVPLAYPATH=./wavplay ./xltwavplay

setuid_root:
	@[ -z "$(SCHED_PRIORITY)" ] && echo "No real time scheduling was compiled in. Check SCHED_PRIORITY." && exit 13 || :
	chmod u+s $(INSTDIR)/wavplay
	@ls -dlL $(INSTDIR)/wavplay
	@echo "***************************************************************************************"
	@echo "*** YOU REMAIN RESPONSIBLE FOR THE POSSIBLE SECURITY RISKS THAT SETUID ROOT IMPLIES ***"
	@echo "***************************************************************************************"
	@echo

install_xltwavplay: xltwavplay
	$(INSTALL) -o root -g root -m 555 xltwavplay $(INSTDIR)
	@[ ! -z "$(USERES)" ] \
		&& $(INSTALL) -o root -g root -m 555 xltwavplay.res $(RESDIR)/xltwavplay \
		&& ls -dlL $(RESDIR)/xltwavplay/xltwavplay.res \
		|| rm -f $(RESDIR)/xltwavplay

clean:
	rm -f *.o a.out core t.t errs.t

clobber: clean
	rm -f xltwavplay wavplay wavrec

DISTRELEASE=wavplay-1.4
MANIFEST= BUGS COPYING INSTALL README wavplay.1 wavplay.lsm xltwavplay.res \
	Makefile client.h server.h wavfile.h wavplay.h xmsprint.h xltwavplay.h \
	client.c file.c locks.c main.c menu.c msg.c procterm.c recplay.c save_as.c \
	server.c wavfile.c xltwavplay.c xmsprint.c \

dist:	clobber
	rm -fr $(DISTRELEASE) $(DISTRELEASE).tar $(DISTRELEASE).tar.gz
	mkdir $(DISTRELEASE)
	cp $(MANIFEST) $(DISTRELEASE)/.
	mv $(DISTRELEASE)/wavplay.lsm $(DISTRELEASE)/$(DISTRELEASE).lsm
	tar czf $(DISTRELEASE).tar.gz $(DISTRELEASE)
	@ls -l $(DISTRELEASE).tar.gz

zonk:   clobber
	rm -fr $(DISTRELEASE) $(DISTRELEASE).tar $(DISTRELEASE).tar.gz

# $Log: Makefile,v $
# Revision 1.1.1.1  2001/08/22 12:57:05  birdy57
#
#
# Revision 1.2  1999/12/04 00:01:20  wwg
# Implement wavplay-1.4 release changes
#
