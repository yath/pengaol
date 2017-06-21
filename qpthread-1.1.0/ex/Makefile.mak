#
#
#	This is free software. You can redistribute it and/or modify under
#	the terms of the GNU Library General Public License version 2.
# 
#	Copyright (C) 2000 by J.Antony
#

######################################################
# example: nmake /f ex32.mak CFG=Release
# All relevant paths to VC++ bin and lib directory should be set
# Set MSVCBASE (see Win32 related section)!
######################################################

!IF "$(CFG)" == ""
CFG=Debug
!MESSAGE No configuration specified. Defaulting to Debug.
!ENDIF 

!IF "$(CFG)" != "Release" && "$(CFG)" != "Debug"
!ERROR An invalid configuration is specified.
!ENDIF 

######################################################
# WIN32 Related

MSVCBASE=C:\PROGRA~1\MICROS~2\VC98
PTHREADS=..\PTHREADS

CPP=$(MSVCBASE)\Bin\cl.exe
CC=$(CPP)

LIB=$(MSVCBASE)\LIB;%LIB%

.SUFFIXES: .cc

# Using NMAKE default rules ...

######################################################
!IF  "$(CFG)" == "Release"

OUTDIR=.
INTDIR=.

CPPFLAGS=/MD /O2 /D "NDEBUG" 
	
LIB32_FLAGS=

######################################################
!ELSEIF  "$(CFG)" == "Debug"

OUTDIR=.
INTDIR=.

CPPFLAGS=/MDd /Od /GZ /Z7 /D "_DEBUG" /D "QP_DEBUG" /FR"$(INTDIR)\\" 

LIB32_FLAGS=

!ENDIF 
######################################################

ALL :	\
		cancel.exe \
		hello.exe \
		hellopool.exe \
		hellorun.exe \
		once.exe \
		pool.exe \
		queue.exe \
		rwlock.exe \
		sleep.exe \
		stack.exe \
		threadtest.exe \
		timer.exe \
		tsd.exe \
		tsdt.exe

#		signal.exe \
#		synchperf.exe \

CPPFLAGS_COMM=/nologo /W3 /GX /YX /FD \
/I . /I $(MSVCBASE)\INCLUDE /I. /I $(PTHREADS) /I ".." /I ".\win32" \
/D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /D "HAVE_SIGSET_T" \
/Fp"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"

CPPFLAGS=$(CPPFLAGS_COMM) $(CPPFLAGS)

CFLAGS=$(CPPFLAGS)

CLEAN :
	-@erase *.obj
	-@erase *.exe
	-@erase *.opt
	-@erase *.ilk
	-@erase *.pdb
	-@erase *.idb
	-@erase *.pch

LIB32FLAGS_COMM=$(INTDIR)\getopt.obj ..\qptw32.lib $(PTHREADS)\pthread.lib

LIB32_OBJS= \
	win32\getopt.obj \
	cancel.obj \
	hello.obj \
	hellopool.obj \
	hellorun.obj \
	inf.obj \
	once.obj \
	pool.obj \
	queue.obj \
	rwlock.obj \
	sleep.obj \
	stack.obj \
	threadtest.obj \
	timer.obj \
	tsd.obj \
	tsdt.obj

#	synchperf.obj \
#	signal.obj \

.cc.obj:
   $(CPP) $(CPPFLAGS) -c -TP $<
                                        
.obj.exe:
    link $(LIB32_FLAGS) $< $(LIB32FLAGS_COMM) /out:$@ 

# Trick to resolve ...???
.\cancel.exe : $(LIB32_OBJS)
