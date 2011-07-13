# Microsoft Developer Studio Project File - Name="SecurityServerLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SecurityServerLib - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SecurityServerLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SecurityServerLib.mak" CFG="SecurityServerLib - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SecurityServerLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SecurityServerLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Live/Dev/Libraries++/SecurityServerLib", VHRHAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SecurityServerLib - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\Temp\Release\SecurityServerLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\stdobj" /I "..\ServerLib" /I "..\App" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "SecurityServerLib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\Temp\Debug\SecurityServerLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\stdobj" /I "..\ServerLib" /I "..\App" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\SecurityServerLibD.lib"

!ENDIF 

# Begin Target

# Name "SecurityServerLib - Win32 Release"
# Name "SecurityServerLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Certificate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AccessToken.cpp
# End Source File
# Begin Source File

SOURCE=.\Certificate.cpp
# End Source File
# End Group
# Begin Group "Encryption"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MCCrypt.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MCDecrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\MCEncrypt.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\DBTokens.cpp
# End Source File
# Begin Source File

SOURCE=.\LicenseFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SecurityServerLib.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccessToken.h
# End Source File
# Begin Source File

SOURCE=.\allocstring.h
# End Source File
# Begin Source File

SOURCE=.\Certificate.h
# End Source File
# Begin Source File

SOURCE=.\DBDatabase.h
# End Source File
# Begin Source File

SOURCE=.\DBTokens.h
# End Source File
# Begin Source File

SOURCE=.\DecryptTables.hpp
# End Source File
# Begin Source File

SOURCE=.\EncryptTables.hpp
# End Source File
# Begin Source File

SOURCE=.\LicenseFile.h
# End Source File
# Begin Source File

SOURCE=.\MCCrypt.h
# End Source File
# Begin Source File

SOURCE=.\MCEncrypt.h
# End Source File
# Begin Source File

SOURCE=.\ReadWriteObject.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
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
