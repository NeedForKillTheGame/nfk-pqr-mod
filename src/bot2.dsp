# Microsoft Developer Studio Project File - Name="bot2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BOT2 - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bot2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bot2.mak" CFG="BOT2 - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "bot2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "bot2 - Win32 Release_NO_CHEAT" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "bot2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "obj"
# PROP Intermediate_Dir "obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BOT2_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PQRMOD_EXPORTS" /D "NC" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /dll /machine:I386 /out:"special_release/pqrmod.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy c:\Games\nfk_bot_project\frogbot_01_alpha3\special_release\pqrmod.dll c:\Games\nfk\nfk057\pqrmod\pqrmod.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "bot2 - Win32 Release_NO_CHEAT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "bot2___Win32_Release_NO_CHEAT"
# PROP BASE Intermediate_Dir "bot2___Win32_Release_NO_CHEAT"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "obj"
# PROP Intermediate_Dir "obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PQRMOD_EXPORTS" /D "NO_CHEAT" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PQRMOD_EXPORTS" /D "NO_CHEAT" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib /nologo /dll /machine:I386 /out:"Release/pqrmod.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"public_release/pqrmod.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy c:\Games\nfk_bot_project\frogbot_01_alpha3\public_release\pqrmod.dll c:\Games\nfk\nfk057\pqrmod\pqrmod.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "bot2 - Win32 Release"
# Name "bot2 - Win32 Release_NO_CHEAT"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ai.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\aim.cpp
# End Source File
# Begin Source File

SOURCE=.\alias.cpp
# End Source File
# Begin Source File

SOURCE=.\autorecord.cpp
# End Source File
# Begin Source File

SOURCE=.\config_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\debug_utils.cpp
# End Source File
# Begin Source File

SOURCE=.\dll_register.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\graph.cpp
# End Source File
# Begin Source File

SOURCE=.\health.cpp
# End Source File
# Begin Source File

SOURCE=.\hud.cpp
# End Source File
# Begin Source File

SOURCE=.\list.cpp
# End Source File
# Begin Source File

SOURCE=.\main_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\map_dm2.cpp
# End Source File
# Begin Source File

SOURCE=.\map_tourney1.cpp
# End Source File
# Begin Source File

SOURCE=.\map_tourney4.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\multibot.cpp
# End Source File
# Begin Source File

SOURCE=.\mw_hook.cpp
# End Source File
# Begin Source File

SOURCE=.\pqrmod.cpp
# End Source File
# Begin Source File

SOURCE=.\proc_hack.cpp
# End Source File
# Begin Source File

SOURCE=.\time_help.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ai.h
# End Source File
# Begin Source File

SOURCE=.\aim.h
# End Source File
# Begin Source File

SOURCE=.\alias.h
# End Source File
# Begin Source File

SOURCE=.\autorecord.h
# End Source File
# Begin Source File

SOURCE=.\config_manager.h
# End Source File
# Begin Source File

SOURCE=.\console.h
# End Source File
# Begin Source File

SOURCE=.\debug_utils.h
# End Source File
# Begin Source File

SOURCE=.\dll_register.h
# End Source File
# Begin Source File

SOURCE=.\graph.h
# End Source File
# Begin Source File

SOURCE=.\health.h
# End Source File
# Begin Source File

SOURCE=.\hud.h
# End Source File
# Begin Source File

SOURCE=.\list.h
# End Source File
# Begin Source File

SOURCE=.\main_menu.h
# End Source File
# Begin Source File

SOURCE=.\map.h
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\multibot.h
# End Source File
# Begin Source File

SOURCE=.\mw_hook.h
# End Source File
# Begin Source File

SOURCE=.\my_math.h
# End Source File
# Begin Source File

SOURCE=.\pqrmod.h
# End Source File
# Begin Source File

SOURCE=.\proc_hack.h
# End Source File
# Begin Source File

SOURCE=.\time_help.h
# End Source File
# Begin Source File

SOURCE=.\types.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\pqrmod.rc
# End Source File
# End Group
# Begin Group "PAS"

# PROP Default_Filter "pas"
# Begin Source File

SOURCE=.\delphi\Bot.dpr

!IF  "$(CFG)" == "bot2 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "bot2 - Win32 Release_NO_CHEAT"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\delphi\bot_register.pas

!IF  "$(CFG)" == "bot2 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "bot2 - Win32 Release_NO_CHEAT"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "public_release"

# PROP Default_Filter "txt"
# Begin Source File

SOURCE=.\public_release\pqrmod\changelog.txt
# End Source File
# Begin Source File

SOURCE=.\public_release\pqrmod\readme.txt
# End Source File
# End Group
# Begin Group "special_release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\special_release\pqrmod\changelog.txt
# End Source File
# Begin Source File

SOURCE=.\special_release\pqrmod\readme.txt
# End Source File
# End Group
# Begin Group "basic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vb\STAT.txt
# End Source File
# End Group
# End Target
# End Project
