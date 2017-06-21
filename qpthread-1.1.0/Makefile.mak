#
#
#	This is free software. You can redistribute it and/or modify under
#	the terms of the GNU Library General Public License version 2.
# 
#	Copyright (C) 2000 by J.Antony
#

######################################################
# example: nmake /f qpw32.mak CFG=Release
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
PTHREADS=.\PTHREADS

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
	
LIB32_FLAGS=-lib

######################################################
!ELSEIF  "$(CFG)" == "Debug"

OUTDIR=.
INTDIR=.

CPPFLAGS=/MDd /Od /GZ /Z7 /D "_DEBUG" /D "QP_DEBUG" /FR"$(INTDIR)\\" 

LIB32_FLAGS=-lib 

!ENDIF 
######################################################

ALL : .\qptw32.lib

CPPFLAGS_COMM=/nologo /W3 /GX /YX /FD \
/I . /I $(MSVCBASE)\INCLUDE /I $(PTHREADS) /I "./qpthr" \
/D "WIN32" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /D "HAVE_SIGSET_T" \
/Fp"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"

CPPFLAGS=$(CPPFLAGS_COMM) $(CPPFLAGS)

CFLAGS=$(CPPFLAGS)

CLEAN :
	-@erase *.obj
	-@erase *.lib
	-@erase *.opt
	-@erase *.ilk
	-@erase *.pdb
	-@erase *.idb
	-@erase *.pch

LIB32_OBJS= \
	qp.obj \
	qp_base.obj \
	qp_misc.obj \
	qp_rwlock.obj \
	qp_sig.obj \
	qp_synch.obj \
	qp_thr.obj \
	qp_timer.obj

.\qptw32.lib : $(LIB32_OBJS)
    link $(LIB32_FLAGS) $(LIB32_OBJS) $(PTHREADS)\pthread.lib /out:qptw32.lib 

.cc.obj:
   $(CPP) $(CPPFLAGS) -c -TP $<
                                        
