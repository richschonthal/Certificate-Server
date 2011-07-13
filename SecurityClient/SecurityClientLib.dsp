# Microsoft Developer Studio Project File - Name="SecurityClientLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SecurityClientLib - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SecurityClientLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SecurityClientLib.mak" CFG="SecurityClientLib - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SecurityClientLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SecurityClientLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Live/Dev/Libraries++/SecurityClient", JHRHAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SecurityClientLib - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SecurityClientLib___Win32_Release"
# PROP BASE Intermediate_Dir "SecurityClientLib___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Lib"
# PROP Intermediate_Dir "Temp\Release\ClientLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "SecurityServerLib" /I "stdobj\include" /I "ServerLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\SecurityServerLib" /I "..\stdobj" /I "..\ServerLib" /I "..\Hardlock\api\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_CHECK_DONGLE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\SecurityClientLib.lib"

!ELSEIF  "$(CFG)" == "SecurityClientLib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SecurityClientLib___Win32_Debug"
# PROP BASE Intermediate_Dir "SecurityClientLib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "..\Temp\Debug\ClientLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GR /GX /Z7 /Od /I "SecurityServerLib" /I "stdobj\include" /I "ServerLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /GR /GX /Z7 /Od /I "..\SecurityServerLib" /I "..\stdobj" /I "..\ServerLib" /I "..\Hardlock\api\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_CHECK_DONGLE" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\SecurityClientLibD.lib"
# ADD LIB32 /nologo /out:"..\Lib\SecurityClientLibD.lib"

!ENDIF 

# Begin Target

# Name "SecurityClientLib - Win32 Release"
# Name "SecurityClientLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\dongle.cpp
# End Source File
# Begin Source File

SOURCE=.\SecurityClient.cpp
# End Source File
# Begin Source File

SOURCE=.\SecurityConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\SecurityTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SecurityServerLib\AccessToken.h
# End Source File
# Begin Source File

SOURCE=.\SecurityServerLib\Certificate.h
# End Source File
# Begin Source File

SOURCE=.\Dongle.h
# End Source File
# Begin Source File

SOURCE=.\Hardlock\api\src\fastapi.h
# End Source File
# Begin Source File

SOURCE=.\Hardlock\api\src\hlapi_c.h
# End Source File
# Begin Source File

SOURCE=.\SecurityClient.h
# End Source File
# Begin Source File

SOURCE=.\SecurityConnection.h
# End Source File
# Begin Source File

SOURCE=.\SecurityTarget.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
