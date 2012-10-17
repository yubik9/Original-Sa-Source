# Microsoft Developer Studio Project File - Name="SAAC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SAAC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SAAC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SAAC.mak" CFG="SAAC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SAAC - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SAAC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SAAC", DAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SAAC - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "NDEBUG"
# ADD RSC /l 0x404 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SAAC - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "_DEBUG"
# ADD RSC /l 0x404 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SAAC - Win32 Release"
# Name "SAAC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "worker"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\wk\error.c
# End Source File
# Begin Source File

SOURCE=.\wk\error.h
# End Source File
# Begin Source File

SOURCE=.\wk\makefile
# End Source File
# Begin Source File

SOURCE=.\wk\TCPIP.c
# End Source File
# Begin Source File

SOURCE=.\wk\TCPIP.h
# End Source File
# Begin Source File

SOURCE=.\wk\tools.c
# End Source File
# Begin Source File

SOURCE=.\wk\tools.h
# End Source File
# Begin Source File

SOURCE=.\wk\version.h
# End Source File
# Begin Source File

SOURCE=.\wk\wk.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\acfamily.c
# End Source File
# Begin Source File

SOURCE=.\auction.c
# End Source File
# Begin Source File

SOURCE=.\char.c
# End Source File
# Begin Source File

SOURCE=.\chatroom.c
# End Source File
# Begin Source File

SOURCE=.\db.c
# End Source File
# Begin Source File

SOURCE=.\deathcontend.c
# End Source File
# Begin Source File

SOURCE=.\defend.c
# End Source File
# Begin Source File

SOURCE=.\lock.c
# End Source File
# Begin Source File

SOURCE=.\mail.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\recv.c
# End Source File
# Begin Source File

SOURCE=.\saacproto_lserver.c
# End Source File
# Begin Source File

SOURCE=.\saacproto_oac.c
# End Source File
# Begin Source File

SOURCE=.\saacproto_serv.c
# End Source File
# Begin Source File

SOURCE=.\saacproto_util.c
# End Source File
# Begin Source File

SOURCE=.\saacproto_work.c
# End Source File
# Begin Source File

SOURCE=.\sasql.c
# End Source File
# Begin Source File

SOURCE=.\tcpip_util.c
# End Source File
# Begin Source File

SOURCE=.\util.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\acfamily.h
# End Source File
# Begin Source File

SOURCE=.\auction.h
# End Source File
# Begin Source File

SOURCE=.\char.h
# End Source File
# Begin Source File

SOURCE=.\chatroom.h
# End Source File
# Begin Source File

SOURCE=.\db.h
# End Source File
# Begin Source File

SOURCE=.\deathcontend.h
# End Source File
# Begin Source File

SOURCE=.\defend.h
# End Source File
# Begin Source File

SOURCE=.\lock.h
# End Source File
# Begin Source File

SOURCE=.\mail.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\recv.h
# End Source File
# Begin Source File

SOURCE=.\saacproto_lserver.h
# End Source File
# Begin Source File

SOURCE=.\saacproto_oac.h
# End Source File
# Begin Source File

SOURCE=.\saacproto_serv.h
# End Source File
# Begin Source File

SOURCE=.\saacproto_util.h
# End Source File
# Begin Source File

SOURCE=.\saacproto_work.h
# End Source File
# Begin Source File

SOURCE=.\sasql.h
# End Source File
# Begin Source File

SOURCE=.\tcpip_util.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
