# Microsoft Developer Studio Project File - Name="GMSV" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GMSV - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GMSV.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GMSV.mak" CFG="GMSV - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GMSV - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GMSV - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GMSV", SBAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GMSV - Win32 Release"

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

!ELSEIF  "$(CFG)" == "GMSV - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
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

# Name "GMSV - Win32 Release"
# Name "GMSV - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "battle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\battle\battle.c
# End Source File
# Begin Source File

SOURCE=.\battle\battle_ai.c
# End Source File
# Begin Source File

SOURCE=.\battle\battle_command.c
# End Source File
# Begin Source File

SOURCE=.\battle\battle_event.c
# End Source File
# Begin Source File

SOURCE=.\battle\battle_item.c
# End Source File
# Begin Source File

SOURCE=.\battle\battle_magic.c
# End Source File
# Begin Source File

SOURCE=.\battle\Makefile
# End Source File
# Begin Source File

SOURCE=.\battle\pet_skill.c
# End Source File
# Begin Source File

SOURCE=.\battle\profession_skill.c
# End Source File
# End Group
# Begin Group "char"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\char\addressbook.c
# End Source File
# Begin Source File

SOURCE=.\char\char.c
# End Source File
# Begin Source File

SOURCE=.\char\char_angel.c
# End Source File
# Begin Source File

SOURCE=.\char\char_base.c
# End Source File
# Begin Source File

SOURCE=.\char\char_data.c
# End Source File
# Begin Source File

SOURCE=.\char\char_event.c
# End Source File
# Begin Source File

SOURCE=.\char\char_item.c
# End Source File
# Begin Source File

SOURCE=.\char\char_party.c
# End Source File
# Begin Source File

SOURCE=.\char\char_talk.c
# End Source File
# Begin Source File

SOURCE=.\char\char_walk.c
# End Source File
# Begin Source File

SOURCE=.\char\chatmagic.c
# End Source File
# Begin Source File

SOURCE=.\char\chatroom.c
# End Source File
# Begin Source File

SOURCE=.\char\deathcontend.c
# End Source File
# Begin Source File

SOURCE=.\char\defaultGroundEnemy.h
# End Source File
# Begin Source File

SOURCE=.\char\defaultPlayer.h
# End Source File
# Begin Source File

SOURCE=.\char\defend.c
# End Source File
# Begin Source File

SOURCE=.\char\encount.c
# End Source File
# Begin Source File

SOURCE=.\char\enemy.c
# End Source File
# Begin Source File

SOURCE=.\char\event.c
# End Source File
# Begin Source File

SOURCE=.\char\family.c
# End Source File
# Begin Source File

SOURCE=.\char\Makefile
# End Source File
# Begin Source File

SOURCE=.\char\pet.c
# End Source File
# Begin Source File

SOURCE=.\char\pet_event.c
# End Source File
# Begin Source File

SOURCE=.\char\petmail.c
# End Source File
# Begin Source File

SOURCE=.\char\skill.c
# End Source File
# Begin Source File

SOURCE=.\char\title.c
# End Source File
# Begin Source File

SOURCE=.\char\trade.c
# End Source File
# End Group
# Begin Group "item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\item\item.c
# End Source File
# Begin Source File

SOURCE=.\item\item_contract.c
# End Source File
# Begin Source File

SOURCE=.\item\item_event.c
# End Source File
# Begin Source File

SOURCE=.\item\item_gen.c
# End Source File
# Begin Source File

SOURCE=.\item\item_trade.c
# End Source File
# Begin Source File

SOURCE=.\item\Makefile
# End Source File
# End Group
# Begin Group "magic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\magic\magic.c
# End Source File
# Begin Source File

SOURCE=.\magic\magic_base.c
# End Source File
# Begin Source File

SOURCE=.\magic\magic_field.c
# End Source File
# Begin Source File

SOURCE=.\magic\Makefile
# End Source File
# End Group
# Begin Group "map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\map\Makefile
# End Source File
# Begin Source File

SOURCE=.\map\map_deal.c
# End Source File
# Begin Source File

SOURCE=.\map\map_util.c
# End Source File
# Begin Source File

SOURCE=.\map\map_warppoint.c
# End Source File
# Begin Source File

SOURCE=.\map\readmap.c
# End Source File
# End Group
# Begin Group "npc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\handletime.c
# End Source File
# Begin Source File

SOURCE=.\npc\Makefile
# End Source File
# Begin Source File

SOURCE=.\npc\npc_action.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_airplane.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_alldoman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_auctioneer.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_bankman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_bigsmallmaster.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_bigsmallpet.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_blackmarket.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_bodylan.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_bus.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_charm.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_checkman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_dengon.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_door.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_doorman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_duelranking.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_eventaction.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_exchangeman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_familyman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_fmchallenge.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_fmdengon.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_fmhealer.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_fmletter.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_fmpkcallman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_fmpkman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_fmwarpman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_freepetskillshop.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_gamblebank.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_gamblemaster.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_gambleroulette.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_healer.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_itemchange.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_itemshop.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_janken.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_luckyman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_makepair.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_manorsman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_mic.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_msg.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_mtradenpcman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_newnpcman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_npcenemy.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_oldman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_pauctionman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_petfusion.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_petmaker.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_petracemaster.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_petracepet.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_petshop.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_petskillshop.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_pkpetshop.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_poolitemshop.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_quiz.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_raceman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_riderman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_roomadminnew.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_sample.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_savepoint.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_scheduleman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_sellsthman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_signboard.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_simpleshop.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_stoneserviceman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_storyteller.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_sysinfo.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_timeman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_townpeople.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_transerman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_transmigration.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_verywelfare.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_warp.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_warpman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_welfare.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_welfare2.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_windowhealer.c
# End Source File
# Begin Source File

SOURCE=.\npc\npc_windowman.c
# End Source File
# Begin Source File

SOURCE=.\npc\npccreate.c
# End Source File
# Begin Source File

SOURCE=.\npc\npcgen.c
# End Source File
# Begin Source File

SOURCE=.\npc\npctemplate.c
# End Source File
# Begin Source File

SOURCE=.\npc\npcutil.c
# End Source File
# Begin Source File

SOURCE=.\npc\readnpc.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\autil.c
# End Source File
# Begin Source File

SOURCE=.\buf.c
# End Source File
# Begin Source File

SOURCE=.\callfromac.c
# End Source File
# Begin Source File

SOURCE=.\callfromcli.c
# End Source File
# Begin Source File

SOURCE=.\configfile.c
# End Source File
# Begin Source File

SOURCE=.\function.c
# End Source File
# Begin Source File

SOURCE=.\genver.sh
# End Source File
# Begin Source File

SOURCE=.\init.c
# End Source File
# Begin Source File

SOURCE=.\link.c
# End Source File
# Begin Source File

SOURCE=.\log.c
# End Source File
# Begin Source File

SOURCE=.\lssproto_serv.c
# End Source File
# Begin Source File

SOURCE=.\lssproto_util.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=.\mclient.c
# End Source File
# Begin Source File

SOURCE=.\msignal.c
# End Source File
# Begin Source File

SOURCE=.\net.c
# End Source File
# Begin Source File

SOURCE=.\npcserver.c
# End Source File
# Begin Source File

SOURCE=.\npcshandle.c
# End Source File
# Begin Source File

SOURCE=.\object.c
# End Source File
# Begin Source File

SOURCE=.\saacproto_cli.c
# End Source File
# Begin Source File

SOURCE=.\saacproto_util.c
# End Source File
# Begin Source File

SOURCE=..\saac\saacproto_work.c
# End Source File
# Begin Source File

SOURCE=.\util.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\addressbook.h
# End Source File
# Begin Source File

SOURCE=.\include\anim_tbl.h
# End Source File
# Begin Source File

SOURCE=.\include\autil.h
# End Source File
# Begin Source File

SOURCE=.\include\battle.h
# End Source File
# Begin Source File

SOURCE=.\include\battle_ai.h
# End Source File
# Begin Source File

SOURCE=.\include\battle_command.h
# End Source File
# Begin Source File

SOURCE=.\include\battle_event.h
# End Source File
# Begin Source File

SOURCE=.\include\battle_item.h
# End Source File
# Begin Source File

SOURCE=.\include\battle_magic.h
# End Source File
# Begin Source File

SOURCE=.\include\buf.h
# End Source File
# Begin Source File

SOURCE=.\include\char.h
# End Source File
# Begin Source File

SOURCE=.\include\char_base.h
# End Source File
# Begin Source File

SOURCE=.\include\char_data.h
# End Source File
# Begin Source File

SOURCE=.\include\char_event.h
# End Source File
# Begin Source File

SOURCE=.\include\char_talk.h
# End Source File
# Begin Source File

SOURCE=.\include\chatmagic.h
# End Source File
# Begin Source File

SOURCE=.\include\chatroom.h
# End Source File
# Begin Source File

SOURCE=.\include\common.h
# End Source File
# Begin Source File

SOURCE=.\include\configfile.h
# End Source File
# Begin Source File

SOURCE=.\include\correct_bug.h
# End Source File
# Begin Source File

SOURCE=.\include\deathcontend.h
# End Source File
# Begin Source File

SOURCE=.\include\defend.h
# End Source File
# Begin Source File

SOURCE=.\include\encount.h
# End Source File
# Begin Source File

SOURCE=.\include\enemy.h
# End Source File
# Begin Source File

SOURCE=.\include\enemyexptbl.h
# End Source File
# Begin Source File

SOURCE=.\include\event.h
# End Source File
# Begin Source File

SOURCE=.\include\family.h
# End Source File
# Begin Source File

SOURCE=.\include\function.h
# End Source File
# Begin Source File

SOURCE=.\include\handletime.h
# End Source File
# Begin Source File

SOURCE=.\include\init.h
# End Source File
# Begin Source File

SOURCE=.\include\item.h
# End Source File
# Begin Source File

SOURCE=.\include\item_event.h
# End Source File
# Begin Source File

SOURCE=.\include\item_gen.h
# End Source File
# Begin Source File

SOURCE=.\include\item_trade.h
# End Source File
# Begin Source File

SOURCE=.\include\levelup.h
# End Source File
# Begin Source File

SOURCE=.\include\link.h
# End Source File
# Begin Source File

SOURCE=.\include\log.h
# End Source File
# Begin Source File

SOURCE=.\include\lssproto_serv.h
# End Source File
# Begin Source File

SOURCE=.\include\lssproto_util.h
# End Source File
# Begin Source File

SOURCE=.\include\magic.h
# End Source File
# Begin Source File

SOURCE=.\include\magic_base.h
# End Source File
# Begin Source File

SOURCE=.\include\magic_field.h
# End Source File
# Begin Source File

SOURCE=.\include\map_deal.h
# End Source File
# Begin Source File

SOURCE=.\include\map_util.h
# End Source File
# Begin Source File

SOURCE=.\include\map_warppoint.h
# End Source File
# Begin Source File

SOURCE=.\include\mclient.h
# End Source File
# Begin Source File

SOURCE=.\include\msignal.h
# End Source File
# Begin Source File

SOURCE=.\include\net.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_action.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_airplane.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_alldoman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_auctioneer.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_bankman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_bigsmallmaster.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_bigsmallpet.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_blackmarket.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_bodylan.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_bus.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_charm.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_checkman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_Dengon.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_door.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_doorman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_duelranking.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_eventaction.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_exchangeman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_familyman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_fmdengon.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_fmhealer.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_fmletter.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_fmpkcallman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_fmpkman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_fmwarpman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_freepetskillshop.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_gamblebank.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_gamblemaster.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_gambleroulette.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_healer.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_itemchange.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_itemshop.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_janken.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_luckyman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_makepair.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_manorsman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_mic.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_msg.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_mtradenpcman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_newnpcman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_npcenemy.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_oldman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_pauctionman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_petfusion.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_petmaker.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_petracemaster.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_petracepet.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_petshop.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_petskillshop.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_pettransman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_pkpetshop.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_poolitemshop.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_quiz.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_raceman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_riderman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_roomadminnew.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_sample.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_savepoint.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_scheduleman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_sellsthman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_signboard.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_simpleshop.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_stoneserviceman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_storyteller.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_sysinfo.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_timeman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_townpeople.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_transerman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_transmigration.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_verywelfare.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_warp.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_warpman.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_welfare.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_welfare2.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_windowhealer.h
# End Source File
# Begin Source File

SOURCE=.\include\npc_windowman.h
# End Source File
# Begin Source File

SOURCE=.\include\npccreate.h
# End Source File
# Begin Source File

SOURCE=.\include\npcgen.h
# End Source File
# Begin Source File

SOURCE=.\include\npcserver.h
# End Source File
# Begin Source File

SOURCE=.\include\npcshandle.h
# End Source File
# Begin Source File

SOURCE=.\include\npctemplate.h
# End Source File
# Begin Source File

SOURCE=.\include\npcutil.h
# End Source File
# Begin Source File

SOURCE=.\include\object.h
# End Source File
# Begin Source File

SOURCE=.\include\pet.h
# End Source File
# Begin Source File

SOURCE=.\include\pet_event.h
# End Source File
# Begin Source File

SOURCE=.\include\pet_skill.h
# End Source File
# Begin Source File

SOURCE=.\include\pet_skillinfo.h
# End Source File
# Begin Source File

SOURCE=.\include\petmail.h
# End Source File
# Begin Source File

SOURCE=.\include\profession_skill.h
# End Source File
# Begin Source File

SOURCE=.\include\readmap.h
# End Source File
# Begin Source File

SOURCE=.\include\readnpc.h
# End Source File
# Begin Source File

SOURCE=.\include\saacproto_cli.h
# End Source File
# Begin Source File

SOURCE=.\include\saacproto_util.h
# End Source File
# Begin Source File

SOURCE=.\include\skill.h
# End Source File
# Begin Source File

SOURCE=.\include\sndcnf.h
# End Source File
# Begin Source File

SOURCE=.\include\title.h
# End Source File
# Begin Source File

SOURCE=.\include\trade.h
# End Source File
# Begin Source File

SOURCE=.\include\util.h
# End Source File
# Begin Source File

SOURCE=.\include\version.h
# End Source File
# Begin Source File

SOURCE=.\include\version_pk.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\log\log.cf
# End Source File
# End Group
# End Target
# End Project
