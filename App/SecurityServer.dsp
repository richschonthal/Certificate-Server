# Microsoft Developer Studio Project File - Name="SecurityServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SecurityServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SecurityServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SecurityServer.mak" CFG="SecurityServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SecurityServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SecurityServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "$/Live/Dev/SecurityServer/SecurityServer"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SecurityServer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Output\Release"
# PROP Intermediate_Dir "..\Temp\Release\Server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\SecurityClient" /I "..\SecurityServerLib" /I "...\ServerLib" /I "...\stdobj\include" /I "..\ServerLib" /I "..\stdobj" /I "..\hardlock\api\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Z<none> /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ServerLib.lib stdobj.lib SecurityServerLib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBC" /libpath:"..\lib" /libpath:"..\Hardlock\Api\win32\msc"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "SecurityServer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Output\Debug"
# PROP Intermediate_Dir "..\Temp\Debug\SecurityServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\SecurityClient" /I "..\SecurityServerLib" /I "...\ServerLib" /I "...\stdobj\include" /I "..\ServerLib" /I "..\stdobj" /I "..\hardlock\api\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /pdbtype:sept /libpath:"..\lib" /libpath:"..\Hardlock\Api\win32\msc"

!ENDIF 

# Begin Target

# Name "SecurityServer - Win32 Release"
# Name "SecurityServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Database"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CertificateMaster.cpp
# End Source File
# Begin Source File

SOURCE=.\DBCleanUpThread.cpp
# End Source File
# Begin Source File

SOURCE=.\DBSubSystem.cpp
# End Source File
# End Group
# Begin Group "Connection"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClientConnectThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientHandlerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientSubSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicNumber.cpp
# End Source File
# End Group
# Begin Group "Configuration"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SSConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\SSConfigBackup.cpp
# End Source File
# Begin Source File

SOURCE=.\SSConfigGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\SSConfigMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\SSConfigPerformance.cpp
# End Source File
# End Group
# Begin Group "IO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SSIOSubSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\SSIOThread.cpp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SecurityServer.cpp
# End Source File
# Begin Source File

SOURCE=.\SecurityServerService.cpp
# End Source File
# Begin Source File

SOURCE=.\SecuritySystem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SSThread.cpp
# End Source File
# End Group
# Begin Group "SystemMonitor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MonitorSubSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemMonitorHandlerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemMonitorThread.cpp
# End Source File
# End Group
# Begin Group "Dongle/Email"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dongle.cpp
# End Source File
# Begin Source File

SOURCE=.\LicenseCheckThread.cpp
# End Source File
# Begin Source File

SOURCE=.\SSQuickMail.cpp
# End Source File
# End Group
# Begin Group "Backup"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BackupThread.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClientHandlerThread.h
# End Source File
# Begin Source File

SOURCE=.\ClientSubSystem.h
# End Source File
# Begin Source File

SOURCE=.\DBSubSystem.h
# End Source File
# Begin Source File

SOURCE=.\DBThread.h
# End Source File
# Begin Source File

SOURCE=.\Dongle.h
# End Source File
# Begin Source File

SOURCE=.\MCCfgReg.h
# End Source File
# Begin Source File

SOURCE=.\MCItemProperties.h
# End Source File
# Begin Source File

SOURCE=.\MCReg.h
# End Source File
# Begin Source File

SOURCE=.\MCVerInfo.h
# End Source File
# Begin Source File

SOURCE=.\MemDatabase.h
# End Source File
# Begin Source File

SOURCE=.\MonitorSubSystem.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SecurityServer.h
# End Source File
# Begin Source File

SOURCE=.\SecurityServerService.h
# End Source File
# Begin Source File

SOURCE=.\SecuritySystem.h
# End Source File
# Begin Source File

SOURCE=.\SSConfig.h
# End Source File
# Begin Source File

SOURCE=.\SSConfigPerformance.h
# End Source File
# Begin Source File

SOURCE=.\SSIOSubSystem.h
# End Source File
# Begin Source File

SOURCE=.\SSIOThread.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SystemMonitorThread.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SecurityServer.ico
# End Source File
# Begin Source File

SOURCE=.\SecurityServer.rc
# End Source File
# Begin Source File

SOURCE=.\res\SecurityServer.rc2
# End Source File
# Begin Source File

SOURCE=.\version.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=..\Hardlock\api\win32\msc\hlw32lmc.lib
# End Source File
# End Target
# End Project
