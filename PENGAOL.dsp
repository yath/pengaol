# Microsoft Developer Studio Project File - Name="PENGAOL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=PENGAOL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PENGAOL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PENGAOL.mak" CFG="PENGAOL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PENGAOL - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "PENGAOL - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PENGAOL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "PENGAOL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "HAVE_CONFIG_H" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib snmpapi.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "PENGAOL - Win32 Release"
# Name "PENGAOL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\pengaol2\CableDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\caim.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\caolcmd30.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\caolheader30.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\caoltoclient30.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cclienttoaol30.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ccommand.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cconfig.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ccrypto.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ciptools.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ckernel.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cloader.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cmsgerror.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cparamconfig.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cppp.cpp

!IF  "$(CFG)" == "PENGAOL - Win32 Release"

!ELSEIF  "$(CFG)" == "PENGAOL - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pengaol2\cppptools.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\crouteur.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\crouteurlinux.cpp

!IF  "$(CFG)" == "PENGAOL - Win32 Release"

!ELSEIF  "$(CFG)" == "PENGAOL - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pengaol2\crouteurWin.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ctcpipheader.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ctextlinux.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ctuntapdriver.cpp

!IF  "$(CFG)" == "PENGAOL - Win32 Release"

!ELSEIF  "$(CFG)" == "PENGAOL - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pengaol2\cusermanager.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cvjcompress.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\CWinTap.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\globals.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\kernel30.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\main.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\modemdriver.cpp

!IF  "$(CFG)" == "PENGAOL - Win32 Release"

!ELSEIF  "$(CFG)" == "PENGAOL - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pengaol2\nulldriver.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\SnmpWin.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\threadELV3.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\TunTapErrors.cpp
# End Source File
# Begin Source File

SOURCE=.\pengaol2\vjcompress.c
# End Source File
# Begin Source File

SOURCE=.\pengaol2\winconf.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\pengaol2\CableDriver.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\caim.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\caolcmd30.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\caolheader30.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\caoltoclient30.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cclienttoaol30.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ccommand.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cconfig.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ccrypto.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ciptools.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ckernel.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cloader.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cmsgerror.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\confwin.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cparamconfig.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cppp.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cppptools.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\crouteur.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\crouteurlinux.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\crouteurWin.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ctcpipheader.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ctextlinux.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ctuntapdriver.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cusermanager.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\cvjcompress.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\CWinTap.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\globals.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\if_ppp.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\if_tun.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\kernel30.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\linuxconf.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\main.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\modemdriver.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\nulldriver.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\ppp_defs.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\SnmpWin.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\threadELV3.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\type.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\vjcompress.h
# End Source File
# Begin Source File

SOURCE=.\pengaol2\winconf.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
