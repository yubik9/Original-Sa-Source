#include "version.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "object.h"
#include "char.h"
#include "char_base.h"
#include "char_data.h"
#include "net.h"
#include "npcutil.h"
#include "handletime.h"
#include "readmap.h"
#include "npc_roomadminnew.h"
#include "npc_door.h"


/* 
 * ÉÔÆ°»º¤ä¤µ¤ó By Kawata
 *
 * »ÅÍÍÂçÉý¤ËÊÑ¹¹¤Î°Ù½ñ¤­Ä¾¤·¤¿Êý¤¬Áá¤¤¤È»×¤¦¤Î¤Ç½ñ¤­¤Ê¤ª¤·
 */


enum{
	/* ¥ª¡¼¥¯¥·¥ç¥óÈÏ°Ï */
	CHAR_WORKINT_RANGE     = CHAR_NPCWORKINT1,
	/* ¼¡¤ÎÄùÀÚ»þ´Ö */
	CHAR_WORKINT_NEXTTIME  = CHAR_NPCWORKINT2,
	/* É½¼¨ÍÑ¡ÖÄùÀÚ¡©Ê¬Á°¡× */
	CHAR_WORKINT_PRINTCLOSETIME = CHAR_NPCWORKINT3,
	/* Éô²°¸¡º÷¤·¤¿ÆüÉÕ */
	CHAR_WORKINT_SEARCHROOMTIME = CHAR_NPCWORKINT4,
	
};
enum {
	/* ¼«Ê¬¤ÎÌ¾Á° */
	CHAR_WORKCHAR_MYNAME   = CHAR_NPCWORKCHAR1,
	/* ¼¡¤ÎÊª·ï */
	CHAR_WORKCHAR_NEXTROOM = CHAR_NPCWORKCHAR2,
	
};

/* ¥é¥ó¥­¥ó¥°É½¼¨£Î£Ð£ÃÍÑ¥ï¡¼¥¯*/
enum {
	/* ´ÉÍý¿Í¤ÎÌ¾Á° */
	CHAR_WORKCHAR_ADMNAME = CHAR_NPCWORKCHAR1,
};


/* ¥Ç¡¼¥¿¥Õ¥¡¥¤¥ë¤ò³ÊÇ¼¤¹¤ë¥Ç¥£¥ì¥¯¥È¥ê¡£ls2/src/lss ¤«¤é¤ÎÁêÂÐ»ØÄê */
#define     NPC_ROOMADMINNEW_DATADIR        "roomadmin/"
/* ¥Ç¡¼¥¿¥Õ¥¡¥¤¥ë¤Î³ÈÄ¥»Ò */
#define     NPC_ROOMADMINNEW_EXTENSION      ".room"
/* ¥é¥ó¥­¥ó¥°¥Ç¡¼¥¿¥Õ¥¡¥¤¥ë¤Î³ÈÄ¥»Ò¡Ê¥Æ¥ó¥Ý¥é¥ê¡Ë */
#define     NPC_ROOMADMINNEW_RANKING_EXTENSION      ".score"
#define     NPC_ROOMADMINNEW_RANKING_EXTENSION_TMP  ".score.tmp"
/* ºÇ¹âÍî»¥¼Ô¥é¥ó¥­¥ó¥°¥Õ¥¡¥¤¥ë */
#define     NPC_ROOMADMINNEW_SCOREFILE      ".score"
/* Äù¤áÀÚ¤ê¡©Ê¬Á°¤«¤é¥á¥Ã¥»¡¼¥¸ */
#define     NPC_ROOMADMINNEW_PRINTCLOSETIME_DEFAULT     5

#define     NPC_ROOMADMINNEW_MSG_CLOSE_BEFORE       \
"%s %s %sªº¬¡°Ê¡AºI¤î¤é%d¤À«e¡C"
#define     NPC_ROOMADMINNEW_MSG_CLOSE              \
"¡C¡C¡C¤wºI¤î¡C"
#define     NPC_ROOMADMINNEW_MSG_NONE               \
"«Ü¥i±¤¥»¦¸¨S¦³¤HÀò±o©Ð¶¡ªº¾Ö¦³Åv¡C"
#define     NPC_ROOMADMINNEW_MSG_CONGRATULATION     \
"®¥³ß±z¡I%sÀò±o©Ð¶¡ªº©Ò¦³Åv¡I%sªºª÷¿ú¥þ³¡³Q¼x¦¬¤F¡C%s½Ð¦VºÞ²z­û¸ß°Ý©Ð¶¡·t¸¹¡C"
#define     NPC_ROOMADMINNEW_MSG_PRINTPASSWD_NONE    \
"¥Ñ©ó§A¥¼«ù¦³©Ð¶¡¦]¦¹¨S¦³·t¸¹¡C"
#define     NPC_ROOMADMINNEW_MSG_DSPPASSWD1          \
"§Aªº©Ð¶¡¬O %s %s %s¡C"
#define     NPC_ROOMADMINNEW_MSG_DSPPASSWD2          \
"©Ð¶¡ªº·t¸¹¬O ¡u%s ¡v¡C¥u­n¦b¹ïµÛªù»¡¸Ü®É¦b»P§À¥[¤W¡u...¡v¡A§O¤H´N¬Ý¤£¨£¤F­ò¡I"
#define     NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION_NONE        \
"¥»¤éªº¬¡°Êµ²§ô¡C"
#define     NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION1     \
"±µ¤U¨Óªº©Ð¶¡¬O %s %s %s¡C"
#define     NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION2     \
"ºI¤î¤é¬O %d:%d ¡C¨ìºI¤î¤é¬°¤îÁÙ¦³%2d¤p®É%02d¤À¡C"
#define     NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION3     \
"³Ì§C±o¼Ðª÷ÃB¬O %s ¡C"

#define     NPC_ROOMADMINNEW_MSG_RANKING_INFO       \
"³o¬O°ªÃB±o¼ÐªÌ±Æ¦æº]¡C"
#define     NPC_ROOMADMINNEW_MSG_RANKING            \
"Rank %2d:%4d/%02d/%02d %8d GOLD %s%s "

/* ¤³¤ÎNPC¤Î¥á¥Ã¥»¡¼¥¸¿§ */
#define     NPC_ROOMADMINNEW_MSGCOLOR           CHAR_COLORWHITE
#define     NPC_RANKING_MSGCOLOR                CHAR_COLORWHITE
#define		NPC_ROOMADMINNEW_2WEEK				60*60*24*14

static void NPC_RoomAdminNew_DspNextAuction( int meindex, int talkerindex);
static void NPC_RoomAdminNew_DspRoomPasswd( int meindex, int adminindex, int talkerindex);
static BOOL NPC_RoomAdminNew_SetNextAuction( int meindex);
static BOOL NPC_RoomAdminNew_WriteDataFile( int meindex, int pindex, char *roomname,
											char *passwd, BOOL flg);
static void NPC_RoomAdminNew_CreatePasswd( char *passwd, int passlen);
static void NPC_RoomAdminNew_SendMsgThisFloor( int index, char *buff);
static int NPC_RoomAdminNew_SearchPlayerHasMostMoney( int meindex);
static void NPC_RoomAdminNew_CloseProc( int meindex);
static int NPC_RoomAdminNew_WriteScoreFile( int meindex, int pindex);
static void NPC_RoomAdminNew_ReadScoreFile( int meindex, int talkerindex, 
											char *admname);

/************************************************
 * ½é´ü½èÍý
 ************************************************/
BOOL NPC_RoomAdminNewInit( int meindex )
{
	int     i;
	char    buf[256];
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	
	/* ¼«Ê¬¤Î´É³í¤ÎÁ´Éô¤Î¥É¥¢¤¬½ÐÍè¾å¤¬¤ë¤Þ¤Ç¤³¤ÎNPC¤ÏºîÀ®¤µ¤ì¤Ê¤¤ */
	for( i = 3; ; i ++ ) {
		if( getStringFromIndexWithDelim(argstr, "|" , i , buf,sizeof(buf) )) {
			if( NPC_DoorSearchByName( buf) < 0 ) {
				print( "admin not created.¦b±K½Xªù°µ¦n¤§«e¥ýµ¥«Ý¡C\n");
				return FALSE;
			}
		}
		else{ 
			break;
		}
	}
	
	NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr));
	
	/* ½é´ü²½½èÍý */
	
	/* type¥»¥Ã¥È */
	CHAR_setInt( meindex, CHAR_WHICHTYPE, CHAR_TYPEROOMADMIN);
	/* Í­¸úÈÏ°Ï¼èÆÀ */
	getStringFromIndexWithDelim(argstr, "|" , 1 , buf,sizeof(buf) );
	CHAR_setWorkInt( meindex , CHAR_WORKINT_RANGE , atoi( buf ));
	/* ¼«Ê¬¤ÎÌ¾Á°¼èÆÀ */
	if( !getStringFromIndexWithDelim(argstr, "|" , 2 , buf,sizeof(buf) )) {
		print( "arg err.admin not created.\n");
		return( FALSE);
	}
	CHAR_setWorkChar( meindex , CHAR_WORKCHAR_MYNAME , buf);
	
	/* É½¼¨ÍÑ¡ÖÄùÀÚ¡©Ê¬Á°¡×½é´ü²½ */
	CHAR_setWorkInt( meindex, CHAR_WORKINT_PRINTCLOSETIME, 0);
	
	/* ¼¡¤ÎÊª·ï¤È¥ª¡¼¥¯¥·¥ç¥ó³«ºÅ»þ´Ö¤ò¥»¥Ã¥È */
	NPC_RoomAdminNew_SetNextAuction( meindex);
	
	return TRUE;
}
/************************************************
 * ¥ë¡¼¥×½èÍý
 * ¸Æ¤Ó½Ð¤·ÉÑÅÙ¤Ï¤½¤ó¤Ê¤ËÉÑÈË¤Ç¤Ê¤¯¤Æ¤â¤¤¤¤¡£
 * ¤Ç¤âÀäÂÐ¤Ë£±Ê¬Ì¤Ëþ¤Ë¤¹¤ë»ö
 ************************************************/
void NPC_RoomAdminNewLoop( int meindex )
{
	int     hun;
	/* ¤â¤·¤â³«ºÅ»þ´Ö¤¬·è¤Þ¤Ã¤Æ¤¤¤Ê¤«¤Ã¤¿¤é¼èÆÀ¤·¤Ê¤ª¤¹ */
	if( CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME) == 0 ) {
		/* Á°²ó¸¡º÷¤·¤¿Æü¤¬°ã¤¨¤Ð¸¡º÷¤·¤Ê¤ª¤¹ */
		struct  tm      tm1,tm2;
		time_t          t;
		memcpy( &tm1, localtime( (time_t *)&NowTime.tv_sec), sizeof( tm1));
		t = CHAR_getWorkInt( meindex, CHAR_WORKINT_SEARCHROOMTIME);
		memcpy( &tm2, localtime( &t), 
								sizeof( tm2));
		if( tm1.tm_year != tm2.tm_year ||
			tm1.tm_mday != tm2.tm_mday ||
			tm1.tm_mon != tm2.tm_mon )
		{
			NPC_RoomAdminNew_SetNextAuction( meindex);
		}
		return;
	}
	
	hun = ceil((double)(CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME) - 
		   NowTime.tv_sec) / 60.0);
	/* ¤â¤·¤â¤Î»þ¤Î°Ù¤Ëhun¤¬¥Þ¥¤¥Ê¥¹¤Î»þ¤Ï£°¤ËÌá¤¹ */
	if( hun < 0 ) hun = 0;
	
	if( hun < NPC_ROOMADMINNEW_PRINTCLOSETIME_DEFAULT ) {
		/* Äù¤áÀÚ¤ê¶á¤¯¤Ë¤Ê¤Ã¤¿¤é¡ÖÄù¤áÀÚ¤ê¡©Ê¬Á°¤Ç¤¹¡×¤È¸À¤ï¤¹ */
		if( hun != CHAR_getWorkInt( meindex, CHAR_WORKINT_PRINTCLOSETIME)) {
			char    buff[256];
			if( hun != 0 ) {
				char    buf2[1024];
				char	msgbuff[16];
				char    argstr2[NPC_UTIL_GETARGSTR_BUFSIZE];
				int		doorindex;

				/* Éô²°¾ðÊó¼èÆÀ */
				doorindex = NPC_DoorSearchByName( CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM));
				NPC_Util_GetArgStr( doorindex, argstr2, sizeof( argstr2));
				getStringFromIndexWithDelim( argstr2, "|" , 9 , 
											 buf2,sizeof(buf2) );
				/* Éô²°¾ðÊó*/
				if( strlen(CHAR_getChar( doorindex, CHAR_NAME) ) != 0 ) {
					strcpy( msgbuff, "¸¹«Ç");
				}
				else {
					msgbuff[0] = '\0';
				}
				/* ¡ÖÄù¤áÀÚ¤ê¡©Ê¬Á°¤Ç¤¹¡× */
				snprintf( buff, sizeof(buff), 
							NPC_ROOMADMINNEW_MSG_CLOSE_BEFORE, 
							buf2,
							CHAR_getChar( doorindex, CHAR_NAME),
							msgbuff,
							hun);
				
				CHAR_setWorkInt( meindex, CHAR_WORKINT_PRINTCLOSETIME, hun);
			}
			else {
				/* ¡ÖÄù¤áÀÚ¤ê¤Þ¤·¤¿¡× */
				strcpy( buff, NPC_ROOMADMINNEW_MSG_CLOSE);
				CHAR_setWorkInt( meindex, CHAR_WORKINT_PRINTCLOSETIME, 0);
			}
			/* ¤³¤Î¥Õ¥í¥¢¤Ë¤¤¤ë¿ÍÁ´°÷¤ËÁ÷¿® */
			NPC_RoomAdminNew_SendMsgThisFloor( meindex, buff);
		}
	}
	/* Äù¤áÀÚ¤Ã¤¿»þ¤Î½èÍý */
	if( hun == 0 ) {
		NPC_RoomAdminNew_CloseProc( meindex);
	}
}

/************************************************
 * ÏÃ¤·¤«¤±¤é¤ì¤¿»þ¤Î½èÍý
 ************************************************/
void NPC_RoomAdminNewTalked( int meindex , int talkerindex , char *msg ,
							 int color )
{
	
	/* ¥×¥ì¥¤¥ä¡¼¤¬¥É¥¢¥Þ¥ó¤Î1¥°¥ê¥Ã¥É°ÊÆâ¤Ê¤éÈ¿±þ¤¹¤ë¡£ */
	if(NPC_Util_CharDistance( talkerindex, meindex ) > 1)return;
	
	/* ÏÃ¤·¤«¤±¤é¤ì¤¿¿Í¤¬Éô²°¤ò¼Ú¤ê¤Æ¤¤¤¿¤é¥Ñ¥¹¥ï¡¼¥É¤òÉ½¼¨¤¹¤ë */
	/*NPC_RoomAdminNew_DspRoomPasswd( meindex, talkerindex);*/
	
	/* ¼¡²ó³«ºÅ»þ´Ö¡¤Éô²°¤ò¸À¤¦ */
	NPC_RoomAdminNew_DspNextAuction( meindex, talkerindex);
}
/*-----------------------------------------------
 * Äù¤áÀÚ¤é¤ì¤¿»þ¤Î½èÍý
 *----------------------------------------------*/
static void NPC_RoomAdminNew_CloseProc( int meindex)
{
	int     pindex;
	char    buff[256];
	char    pname[32];
	int     failed = TRUE;
	
	/* °ìÈÖ¤ª¶â¤òÂ¿¤¯»ý¤Ã¤Æ¤¤¤ë¥×¥ì¥¤¥ä¡¼¤òÃµ¤¹ */
	pindex = NPC_RoomAdminNew_SearchPlayerHasMostMoney( meindex);
	/* ³ºÅö¼Ô¥¢¥ê */
	if( pindex != -1) {
		/* £±²ó¤ÇÈ´¤±¤ë¥ë¡¼¥×¤Ç¤¹ */
		while( 1) {
			char    passwd[9];
			int		doorindex;
			doorindex = NPC_DoorSearchByName( 
							CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM));
			/* ¥Ñ¥¹¥ï¡¼¥É¤òÀ¸À®¤¹¤ë */
			NPC_RoomAdminNew_CreatePasswd( passwd, sizeof( passwd));
			/* ¥Õ¥¡¥¤¥ë¤Ë¤³¤Î¿Í¤Î£Ã£Ä¥­¡¼¡¤¥­¥ã¥éÌ¾¡¤Éô²°Ì¾¡¤¥Ñ¥¹¤ò½ñ¤­¹þ¤à */
			if( NPC_RoomAdminNew_WriteDataFile( meindex, pindex,
							CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM),
												passwd, 1)
				== FALSE) 
			{
				break;
			}
			/* ¥É¥¢¤Ë¥Ñ¥¹¤ò¥»¥Ã¥È */
			if( NPC_DoorSetPasswd( doorindex, passwd) == -1 ) {
				break;
			}
			/* Í­¸ú´ü¸Â¤ò¥»¥Ã¥È */
			CHAR_setWorkInt( doorindex, CHAR_WORKDOOREXPIRETIME, 
							CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME)
							+ NPC_ROOMADMINNEW_2WEEK);
			/* Íî»¥¼Ô¥é¥ó¥­¥ó¥°¥Á¥§¥Ã¥¯¡õ¥Õ¥¡¥¤¥ë½ñ¤­¹þ¤ß */
			NPC_RoomAdminNew_WriteScoreFile( meindex, pindex);

			CHAR_setInt( pindex, CHAR_GOLD, 0);

			CHAR_send_P_StatusString( pindex , CHAR_P_STRING_GOLD);
			
			/* ¤ª¤á¤Ç¤È¤¦¡ª¥á¥Ã¥»¡¼¥¸ */
			strcpy( pname, CHAR_getChar( pindex, CHAR_NAME));
			snprintf( buff, sizeof(buff), 
						NPC_ROOMADMINNEW_MSG_CONGRATULATION, pname, pname, pname);
			failed = FALSE;
			break;
		}
	}
	
	/* ³ºÅö¼Ô¥Ê¥·¤Þ¤¿¤Ï½èÍý¼ºÇÔ¤Î»þ */
	if( failed) {
		/* ¥Õ¥¡¥¤¥ë¤«¤é¤³¤ÎÉô²°¤Î¾ðÊó¤ò¾Ã¤¹ */
		NPC_RoomAdminNew_WriteDataFile( meindex, -1,
					CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM),
										NULL, 0);
		/* ³ºÅö¼Ô¤Ï¤¤¤Þ¤»¤ó¥á¥Ã¥»¡¼¥¸ */
		snprintf( buff, sizeof(buff), 
				NPC_ROOMADMINNEW_MSG_NONE);
	}
	/* ¤³¤Î¥Õ¥í¥¢¤Ë¤¤¤ë¿ÍÁ´°÷¤ËÁ÷¿® */
	NPC_RoomAdminNew_SendMsgThisFloor( meindex, buff);
	
	/* ¼¡¤ÎÊª·ï¤È¥ª¡¼¥¯¥·¥ç¥ó³«ºÅ»þ´Ö¤ò¥»¥Ã¥È */
	NPC_RoomAdminNew_SetNextAuction( meindex);
	
}
/*-----------------------------------------------
 * °ìÈÖ¤ª¶â¤òÂ¿¤¯»ý¤Ã¤Æ¤¤¤ë¥×¥ì¥¤¥ä¡¼¤òÃµ¤¹
 *----------------------------------------------*/
static int NPC_RoomAdminNew_SearchPlayerHasMostMoney( int meindex)
{
	int     i,j, range;
	int     fl,x,y, gold;
	int     doorindex;
	int     topindex;
	
	fl = CHAR_getInt( meindex, CHAR_FLOOR);
	x  = CHAR_getInt( meindex, CHAR_X);
	y  = CHAR_getInt( meindex, CHAR_Y);
	range = CHAR_getWorkInt( meindex , CHAR_WORKINT_RANGE);
	topindex = -1;
	
	/* Éô²°¤ÎÌ¾Á°¤«¤éºÇÄãÍî»¥¶â³Û¤òÄ´¤Ù¤ë */
	doorindex = NPC_DoorSearchByName( CHAR_getWorkChar( meindex, 
														CHAR_WORKCHAR_NEXTROOM));
	gold = atoi( CHAR_getWorkChar( doorindex, CHAR_WORKDOORGOLDLIMIT));
	/* ÈÏ°ÏÆâ¤Ë¤¤¤ë¥­¥ã¥é¤Î¿ô¤òÄ´¤Ù¤ë */
	for( i = x-range ; i <= x+range ; i++ ){
		for( j = y-range ; j <= y+range ; j ++ ){
			OBJECT  object;
			for( object = MAP_getTopObj( fl, i,j); object; 
				 object = NEXT_OBJECT(object) ) 
			{
				int objindex = GET_OBJINDEX( object);
				if( OBJECT_getType(objindex) == OBJTYPE_CHARA ){
					int     index;
					index = OBJECT_getIndex(objindex);
					if( CHAR_getInt( index,
								 CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER )
					{
						if( CHAR_getInt( index, CHAR_GOLD) >= gold ) {
							if( topindex == -1 ) {
								topindex =  index;
							}
							else {
								/* ¥È¥Ã¥×¤Î¿Í¤È¤ª¶âÈæ³Ó */
								int nowgold,topgold;
								nowgold = CHAR_getInt( index, CHAR_GOLD );
								topgold = CHAR_getInt( topindex, CHAR_GOLD);
								if( nowgold > topgold ) {
									topindex = index;
								}
								else if( nowgold == topgold ) {
									/* °ì½ï¤Ê¤é¥ì¥Ù¥ë¤¬Äã¤¤¿Í */
									int nowlv, toplv;
									nowlv = CHAR_getInt( index, CHAR_LV );
									toplv = CHAR_getInt( topindex, CHAR_LV);
									if( nowlv < toplv ) {
										topindex = index;
									}
									/* ¤½¤ì¤Ç¤â²ò·è¤·¤Ê¤¤¤ó¤Ê¤é¥é¥ó¥À¥à¤¸¤ã*/
									/* index¤¬Àè¤Î¿Í¤¬¤Á¤ç¤Ã¤ÈÉÔÍø¤À¤±¤É */
									else if( nowlv == toplv ) {
										if( RAND( 0,1)) {
											topindex = index;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return( topindex);
}
/*-----------------------------------------------
 * »ØÄê¤µ¤ì¤¿index¤Î¥Õ¥í¥¢¤Ë¤¤¤ë¥×¥ì¥¤¥ä¡¼Á´°÷¤Ë
 * ¥á¥Ã¥»¡¼¥¸¤òÁ÷¿®¤¹¤ë¡£
 *----------------------------------------------*/
static void NPC_RoomAdminNew_SendMsgThisFloor( int index, char *buff)
{
	int     i, fl;
	int     playernum = CHAR_getPlayerMaxNum();
	
	fl = CHAR_getInt( index, CHAR_FLOOR);
	for( i = 0; i < playernum; i ++ ) {
		if( CHAR_getInt( i, CHAR_FLOOR) == fl ) {
			/* Ç°¤Î¤¿¤á¥×¥ì¥¤¥ä¡¼¤«¥Á¥§¥Ã¥¯ */
			if( CHAR_getInt( i, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER ) {
				CHAR_talkToCli( i, index, buff, NPC_ROOMADMINNEW_MSGCOLOR);
			}
		}
	}
}
/*-----------------------------------------------
 * ¥Ñ¥¹¥ï¡¼¥É¤òÀ¸À®¤¹¤ë¡£passlen¤Î-1Ê¸»ú¤¬ºî¤é¤ì¤ë¡£
 * makecdn.c¤Î¤Ñ¤¯¤ê¡£
 *----------------------------------------------*/
static void NPC_RoomAdminNew_CreatePasswd( char *passwd, int passlen)
{
#define NPC_ROOMADMINNEW_RAND()    ( (rand() /256) % 32768 )       /* 256¤Ç³ä¤ë¤Î¤ÏÍð¿ô¤ÎÉÊ¼Á¤Î¤¿¤á */
	int     i;
	char *candidates[]= {
		"A","B","D",   "E","F","G","H",   "J","K","L","M",   "N","P","Q","R",
		"T","W","X","Y",   "a","b","d","e",   "f","g","m","n",   "q","r","t",
		"0","1","2",   "3","4","5","6",   "7","8","9"
	};
	strcpy( passwd , "");
	for(i = 0;i < passlen -1 ; i++){
		strcat( passwd , candidates[ NPC_ROOMADMINNEW_RAND() % (sizeof( candidates)/sizeof(candidates[0])) ] );
	}
#undef NPC_ROOMADMINNEW_RAND
}
/*-----------------------------------------------
 * ´ÉÍý¿ÍÍÑ¤Î¥Õ¥¡¥¤¥ë¤Ë¥Ç¡¼¥¿¤ò½ñ¤­¹þ¤à
 *
 * ¥Õ¥¡¥¤¥ëÌ¾¤Ï"©Ð¶¡ªº¦W¦r.room"
 * ¤ò»È¤¦¡£
 *
 * ¥Õ¥¡¥¤¥ë¤Î¥Õ¥©¡¼¥Þ¥Ã¥È¤Ï¡¤
 * cdkey|charaname|passwd|expiretime
 *
 *
 * °ú¿ô¤Îflg ¤¬£±¤À¤Ã¤¿¤é½ñ¤­¹þ¤ß¡¤£°¤À¤Ã¤¿¤é¥Õ¥¡¥¤¥ëºï½ü
 * Ìá¤êÃÍ¡§TRUE¡§À®¸ù
 *         FALSE:²¿¤é¤«¤Î¼ºÇÔ
 *----------------------------------------------*/
static BOOL NPC_RoomAdminNew_WriteDataFile( int meindex, int pindex, 
											char *roomname,
											char *passwd, int flg)
{
	char    *cdkey;
	char    charaname[32*2];
	char    expiretime[64];
	char    filename[128];
	char    outbuf[1024];
	FILE    *fp_src;
	
	/* ¥Õ¥¡¥¤¥ëÌ¾¤òÀ¸À® */
	strcpy( filename, NPC_ROOMADMINNEW_DATADIR);
	/*strcat( filename, CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM));*/
	strcat( filename, roomname);
	strcat( filename, NPC_ROOMADMINNEW_EXTENSION);

	if( flg) {
		/* Íî»¥¤·¤¿¿Í¤Î£Ã£Ä¥­¡¼ */
        //getcdkeyFromCharaIndex( pindex, cdkey, sizeof(cdkey ));
		cdkey = CHAR_getChar( pindex, CHAR_CDKEY);
		/* Íî»¥¤·¤¿¿Í¤Î¥­¥ã¥éÌ¾ */
		makeEscapeString(  CHAR_getChar( pindex, CHAR_NAME),
							charaname, sizeof( charaname));
		/* ´ü¸ÂÀÚ¤ì»þ´Ö */
		snprintf( expiretime, sizeof( expiretime), "%d", 
					CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME)
					+ NPC_ROOMADMINNEW_2WEEK   /* £²½µ´Ö¸å */
					);

		/* ¥Ç¥ê¥ß¥¿¤Ç¤¯¤¯¤ë */
		snprintf( outbuf, sizeof( outbuf), "%s|%s|%s|%s|",
					cdkey, charaname, passwd, expiretime);
		fp_src = fopen( filename, "w");
		if( !fp_src) {
			print( "[%s:%d] File Open Error\n", __FILE__, __LINE__);
			return FALSE;
		}
		fputs( outbuf, fp_src);
		fclose( fp_src);
	}
	else {
		unlink( filename);
	}
	
	return TRUE;
	
}
/*-----------------------------------------------
 * ¥Ç¡¼¥¿¥Õ¥¡¥¤¥ë¤òÆÉ¤ó¤Ç³ºÅö¤¹¤ëÉô²°¤¬¤¢¤ì¤Ð
 * ¥Ç¡¼¥¿¤ò³ÊÇ¼¤·¤ÆÊÖ¤¹¡£
 *----------------------------------------------*/
BOOL NPC_RoomAdminNew_ReadFile( char *roomname, NPC_ROOMINFO *data)
{
	FILE    *fp;
	char    line[1024];
	char    buf2[128];
	char    filename[128];
	
	/* ¥Õ¥¡¥¤¥ëÌ¾¤òÀ¸À® */
	strcpy( filename, NPC_ROOMADMINNEW_DATADIR);
	strcat( filename, roomname);
	strcat( filename, NPC_ROOMADMINNEW_EXTENSION);
	
	fp = fopen( filename, "r");
	if( !fp) return FALSE;
	fgets( line, sizeof( line), fp);
	/* ¥Ç¡¼¥¿¥»¥Ã¥È */
	getStringFromIndexWithDelim( line, "|" , 1 , buf2,sizeof(buf2) );
	strcpy( data->cdkey, buf2);
	getStringFromIndexWithDelim( line, "|" , 2 , buf2,sizeof(buf2) );
	strcpy( data->charaname, makeStringFromEscaped( buf2));
	getStringFromIndexWithDelim( line, "|" , 3 , buf2,sizeof(buf2) );
	strcpy( data->passwd, buf2);
	getStringFromIndexWithDelim( line, "|" , 4 , buf2,sizeof(buf2) );
	if( strcmp( buf2, "max") == 0 ) {
		data->expire = 0xffffffff;
	}
	else {
		data->expire = atoi( buf2);
	}
	fclose( fp);
	return TRUE;
}
/*-----------------------------------------------
 * ¼¡¤Î¥ª¡¼¥¯¥·¥ç¥ó¤Î»þ´Ö¤ÈÉô²°Ì¾¤ò¥ï¡¼¥¯¤Ë¥»¥Ã¥È¤¹¤ë
 *----------------------------------------------*/
static BOOL NPC_RoomAdminNew_SetNextAuction( int meindex)
{
	int     i, j;
	int     hitindex = -1;
	int     charnum = CHAR_getCharNum();
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	struct  tm  tmwk;
	
	/* ÍËÆü¤Ê¤É¤ò¼èÆÀ¤¹¤ë*/
	memcpy( &tmwk, localtime( ( time_t *)&NowTime.tv_sec), sizeof( tmwk));
	
	NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr));

	/* ¥É¥¢¤ò¸¡º÷¤¹¤ë¡£NPC_DoorSearchByName()¤ò»È¤Ã¤¿¤é³Ú¤À¤±¤É
	 *  ÌµÂÌ¤¬Â¿¤¯¤Ê¤ê¤½¤¦¤Ê¤Î¤Ç¤³¤³¥ª¥ê¥¸¥Ê¥ë
	 */
	for( i = CHAR_getPlayerMaxNum()+ CHAR_getPetMaxNum(); 
		 i < charnum;
		 i ++ )
	{
		if( CHAR_getInt( i, CHAR_WHICHTYPE) != CHAR_TYPEDOOR) {
			continue;
		}
		if( CHAR_getWorkInt( i, CHAR_WORKDOORPASSFLG) == 0 ) {
			continue;
		}
		/* Ì¾Á°¤¬´É³í¤Î¤â¤Î¤«¥Á¥§¥Ã¥¯¤¹¤ë */
		for( j = 3; ; j ++ ) {
			int rc;
			char    *doorname;
			char    buf[256];
			int     dataexist;
			NPC_ROOMINFO data;
			
			rc = getStringFromIndexWithDelim( argstr, "|" , j,
												buf,sizeof(buf) );
			if( rc != TRUE ) {
				break;
			}
			doorname = CHAR_getWorkChar( i, CHAR_WORKDOORNAME);
			/* ¥É¥¢Ì¾¤¬°ìÃ×¤·¤¿ */
			if( strcmp( buf, doorname ) != 0 ) {
				continue;
			}
			/* ÍËÆü¤¬°ìÃ×¤¹¤ë */
			if( atoi( CHAR_getWorkChar( i, CHAR_WORKDOORWEEK)) != tmwk.tm_wday) {
				break;
			}
			/* °ì±þ½é´ü²½ */
			memset( &data, 0, sizeof( data));
			/* ¥Ç¡¼¥¿¥Õ¥¡¥¤¥ë¤«¤é¥Ç¡¼¥¿¤òÆÉ¤à */
			if( NPC_RoomAdminNew_ReadFile( doorname, &data) ) {
				struct tm   tmdoor;
				/* ¤³¤³¤Ç´ü¸ÂÀÚ¤ì¥Á¥§¥Ã¥¯¤ò¤ä¤Ã¤Æ¤ä¤ë¡£
				 * ´ü¸Â¤¬ÀÚ¤ì¤Æ¤¤¤¿¤é¥Õ¥¡¥¤¥ë¤òºï½ü¤¹¤ë
				 * »ÅÍÍÅª¤Ë¤¢¤Þ¤ê¤³¤Î¥ë¡¼¥Á¥óÄÌ¤ë»ö¤Ï¤Ê¤¤¤È»×¤¦¡£
				 */
				if( data.expire < NowTime.tv_sec) {
					NPC_RoomAdminNew_WriteDataFile( meindex, -1,doorname, NULL, 0);
					/* ¥É¥¢¤â¤­¤ì¤¤¤Ë*/
					NPC_DoorSetPasswd( i, "8hda8iauia90494jasd9asodfasdf89");
					CHAR_setWorkInt( i, CHAR_WORKDOOREXPIRETIME, 0xffffffff);
					
					break;
				}
				/* º£Æü¤ÇÀÚ¤ì¤ëÊª·ï¤Ê¤é£Ï£Ë */
				memcpy( &tmdoor, localtime( ( time_t *)&data.expire), sizeof( tmdoor));
				if( tmdoor.tm_mon != tmwk.tm_mon ||
					tmdoor.tm_mday != tmwk.tm_mday)
				{
					break;
				}
				dataexist = TRUE;
			}
			else {
				dataexist = FALSE;
			}
			/* ¸½ºß¤Î»þ´Ö¤è¤ê¸å¤Ç¤¢¤ë»ö¡£ */
			if( tmwk.tm_hour > atoi( CHAR_getWorkChar( i, CHAR_WORKDOORHOUR))){
				break;
			}
			if( tmwk.tm_hour == atoi( CHAR_getWorkChar( i, CHAR_WORKDOORHOUR))) {
				if( tmwk.tm_min >= atoi( CHAR_getWorkChar( i, CHAR_WORKDOORMINUTE))) {
					break;
				}
			}
			/* ¤³¤³¤Þ¤Ç¤­¤¿¤é¤È¤ê¤¢¤¨¤º¤³¤Î¥É¥¢¤Ï¼¡³«ºÅ¤Ë½Ð¤ë»ñ³Ê¤¬
			 * ¤¢¤ë¤È¤¤¤¦»ö¡£¤³¤ì¤«¤éÂ¾¤Î¥É¥¢¤È¥Á¥§¥Ã¥¯¤¹¤ë¡£
			 */
			if( hitindex == -1 ) {
				hitindex = i;
			}
			else {
				/* ³«ºÅ»þ´Ö¤¬Áá¤¯¡¤¸½ºß¼Ú¤ê¤é¤ì¤Æ¤¤¤ë¤È¤¤¤¦¤Î¤¬Í¥Àè½ç°Ì */
				if( atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORHOUR)) 
					> atoi( CHAR_getWorkChar( i, CHAR_WORKDOORHOUR)))
				{
					hitindex = i;
				}
				else if( atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORHOUR)) 
						== atoi( CHAR_getWorkChar( i, CHAR_WORKDOORHOUR)))
				{
					if( atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORMINUTE)) 
						> atoi( CHAR_getWorkChar( i, CHAR_WORKDOORMINUTE)))
					{
						hitindex = i;
					}
					else if( atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORMINUTE)) 
						== atoi( CHAR_getWorkChar( i, CHAR_WORKDOORMINUTE)))
					{
						NPC_ROOMINFO    datawk;
						int     hitindexdataexist;
						hitindexdataexist = NPC_RoomAdminNew_ReadFile( 
									CHAR_getWorkChar( hitindex, CHAR_WORKDOORNAME),
									&datawk);
						if( hitindexdataexist < dataexist ) {
							hitindex = i;
						}
						/* ¤³¤³¤Þ¤ÇÍè¤ë¤è¤¦¤Ê¤éÀè¤Îindex¤Î¤Þ¤Þ¤È¤¹¤ë¡£*/
						
					}
				}
			}
		}
	}
	if( hitindex > 0 ) {
		/* ³«ºÅ»þ´Ö¤ò¥»¥Ã¥È */
		tmwk.tm_sec = 0;
		tmwk.tm_min = atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORMINUTE));
		tmwk.tm_hour = atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORHOUR));
		CHAR_setWorkInt( meindex, CHAR_WORKINT_NEXTTIME, mktime( &tmwk));
		/* Éô²°Ì¾¤ò¥»¥Ã¥È */
		CHAR_setWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM,
						 CHAR_getWorkChar( hitindex, CHAR_WORKDOORNAME));
		
	}
	else {
		CHAR_setWorkInt( meindex, CHAR_WORKINT_NEXTTIME, 0);
		/* Éô²°Ì¾¤ò¥»¥Ã¥È */
		CHAR_setWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM,"" );
	
	}
	/* ¸¡º÷»þ´Ö¤ò¥»¥Ã¥È */
	CHAR_setWorkInt( meindex, CHAR_WORKINT_SEARCHROOMTIME, NowTime.tv_sec);
	return( TRUE);
}
/*-----------------------------------------------
 * ÏÃ¤·¤«¤±¤é¤ì¤¿¿Í¤¬Éô²°¤ò¼Ú¤ê¤Æ¤¤¤¿¤é¥Ñ¥¹¥ï¡¼¥É¤òÉ½¼¨¤¹¤ë
 *----------------------------------------------*/
static void NPC_RoomAdminNew_DspRoomPasswd( int index, int adminindex, int talkerindex)
{
	int     i, rc, flg = FALSE;
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char    argstr2[NPC_UTIL_GETARGSTR_BUFSIZE];
	char    *cdkey;
	char    buf[128];
	char	buff[16];
	char    buf2[1024];

	NPC_ROOMINFO    info;

    //getcdkeyFromCharaIndex( talkerindex, cdkey, sizeof(cdkey));
	cdkey = CHAR_getChar( talkerindex, CHAR_CDKEY);
	
	NPC_Util_GetArgStr( adminindex, argstr, sizeof( argstr));
	for( i = 3; ; i ++ ) {
		if( getStringFromIndexWithDelim(argstr, "|" , i , buf,sizeof(buf) )) {
			rc = NPC_RoomAdminNew_ReadFile( buf, &info);
			if( rc == TRUE ) {
				/* CDKEY ¤È¥­¥ã¥é¤ÎÌ¾Á°¤¬°ìÃ×¤·¤¿¤é */
				if( strcmp( info.cdkey, cdkey) == 0 &&
					strcmp( info.charaname, 
							CHAR_getChar( talkerindex, CHAR_NAME)) == 0 )
				{
					int doorindex;
					char    msgbuf[256];
					/* Éô²°¾ðÊó¼èÆÀ */
					doorindex = NPC_DoorSearchByName( buf);
					NPC_Util_GetArgStr( doorindex, argstr2, sizeof( argstr2));
					getStringFromIndexWithDelim( argstr2, "|" , 9 , 
												 buf2,sizeof(buf2) );
					/* Éô²°¾ðÊó*/
					if( strlen(CHAR_getChar( doorindex, CHAR_NAME) ) != 0 ) {
						strcpy( buff, "¸¹«Ç");
					}
					else {
						buff[0] = '\0';
					}
					
					/* ¥á¥Ã¥»¡¼¥¸¥»¥Ã¥È */
					snprintf( msgbuf, sizeof( msgbuf), 
								NPC_ROOMADMINNEW_MSG_DSPPASSWD1,
								buf2,
								CHAR_getChar( doorindex, CHAR_NAME),
								buff
							);
					/* ¥á¥Ã¥»¡¼¥¸Á÷¿® */
					CHAR_talkToCli( talkerindex, index,msgbuf, 
									NPC_ROOMADMINNEW_MSGCOLOR);

					/* ¥á¥Ã¥»¡¼¥¸¥»¥Ã¥È */
					snprintf( msgbuf, sizeof( msgbuf), 
								NPC_ROOMADMINNEW_MSG_DSPPASSWD2,
								CHAR_getWorkChar(doorindex, CHAR_WORKDOORPASSWD)
							);
					/* ¥á¥Ã¥»¡¼¥¸Á÷¿® */
					CHAR_talkToCli( talkerindex, index,msgbuf, 
									NPC_ROOMADMINNEW_MSGCOLOR);
					flg = TRUE;
				}
			}
		}
		else {
			break;
		}
	}
	if( !flg) {
		CHAR_talkToCli( talkerindex, index,
						NPC_ROOMADMINNEW_MSG_PRINTPASSWD_NONE,
						NPC_ROOMADMINNEW_MSGCOLOR);
	}
}
/*-----------------------------------------------
 * ¼¡²ó³«ºÅ»þ´Ö¡¤Éô²°¤ò¸À¤¦
 *----------------------------------------------*/
static void NPC_RoomAdminNew_DspNextAuction( int meindex, int talkerindex)
{
	char    msgbuf[2048];
	time_t  closetime;
	
	closetime = CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME);
	/* ¼¡¤Î³«ºÅ»þ´Ö¤¬·è¤Þ¤Ã¤Æ¤¤¤Ê¤±¤ì¤Ð¡Ö·è¤Þ¤Ã¤Æ¤Þ¤»¤ó¡£¡×¥á¥Ã¥»¡¼¥¸*/
	if( closetime == 0 ) {
		snprintf( msgbuf, sizeof( msgbuf),
					NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION_NONE
				);
		CHAR_talkToCli( talkerindex, meindex,msgbuf, NPC_ROOMADMINNEW_MSGCOLOR);
	}
	else {
		int     doorindex;
		char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
		char    explain[1024];      /*ÀâÌÀ*/
		char	buff[16];
		struct  tm tmwk;
		int     difftime;
		/* Éô²°¾ðÊó¤ò¼èÆÀ */
		doorindex = NPC_DoorSearchByName( 
					CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM));
		NPC_Util_GetArgStr( doorindex, argstr, sizeof( argstr));
		getStringFromIndexWithDelim(argstr, "|" , 9 , explain,sizeof(explain));
		/* Éô²°¾ðÊó*/
		if( strlen(CHAR_getChar( doorindex, CHAR_NAME) ) != 0 ) {
			strcpy( buff, "¸¹«Ç");
		}
		else {
			buff[0] = '\0';
		}
		snprintf( msgbuf, sizeof( msgbuf),
					NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION1,
					explain,
					CHAR_getChar( doorindex, CHAR_NAME),
					buff
				);
		CHAR_talkToCli( talkerindex, meindex,msgbuf, NPC_ROOMADMINNEW_MSGCOLOR);
		/* Äù¤áÀÚ¤ê»þ´Ö */
		memcpy( &tmwk, localtime( &closetime), sizeof( tmwk));
		difftime = closetime - NowTime.tv_sec;
		snprintf( msgbuf, sizeof( msgbuf),
					NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION2,
					tmwk.tm_hour,
					tmwk.tm_min,
					difftime / 3600,
					(difftime % 3600) / 60
				);
		CHAR_talkToCli( talkerindex, meindex,msgbuf, NPC_ROOMADMINNEW_MSGCOLOR);
		
		/* ²¼¸Â¶â³Û */
		snprintf( msgbuf, sizeof( msgbuf),
					NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION3,
					CHAR_getWorkChar( doorindex, CHAR_WORKDOORGOLDLIMIT)
				);
		CHAR_talkToCli( talkerindex, meindex,msgbuf, NPC_ROOMADMINNEW_MSGCOLOR);
	}
	
}
/*-----------------------------------------------
 * Íî»¥¼Ô¥é¥ó¥­¥ó¥°¥Á¥§¥Ã¥¯¡õ¥Õ¥¡¥¤¥ë½ñ¤­¹þ¤ß
 *
 * ¥Õ¥¡¥¤¥ëÌ¾¤Ï"ºÞ²zªÌªº¦W¦r.score"
 * ¥Æ¥ó¥Ý¥é¥ê¥Õ¥¡¥¤¥ëÌ¾¤Ï"ºÞ²zªÌªº¦W¦r.score.tmp"
 *
 * ¥Õ¥¡¥¤¥ë¥Õ¥©¡¼¥Þ¥Ã¥È¤Ï¡¤
 * gold|time|cdkey|charaname|owntitle
 * ¤Ç£±£°¹Ô¤Þ¤Ç¡£
 *
 * Ìá¤êÃÍ¡§ À®¸ù¡§¥é¥ó¥­¥ó¥°²¿°Ì¤«ÊÖ¤¹¡£¥é¥ó¥­¥ó¥°¤ËÆþ¤é¤Ê¤±¤ì¤Ð£°¡£
 *          ¼ºÇÔ¡§-1
 *----------------------------------------------*/
static int NPC_RoomAdminNew_WriteScoreFile( int meindex, int pindex)
{
#define NPC_RANKING_DEFAULT     10
	int     ret = 0;
	char    *cdkey;
	char    charaname[32*2],owntitle[32*2];
	char    filename_dest[128],filename[128];
	char    outbuf[1024], line[1024];
	FILE    *fp_src, *fp_dest;

	/* ¥Õ¥¡¥¤¥ëÌ¾¤òÀ¸À® */
	strcpy( filename, NPC_ROOMADMINNEW_DATADIR);
	strcat( filename, CHAR_getWorkChar( meindex, CHAR_WORKCHAR_MYNAME));
	strcpy( filename_dest, filename);
	strcat( filename, NPC_ROOMADMINNEW_RANKING_EXTENSION);
	strcat( filename_dest, NPC_ROOMADMINNEW_RANKING_EXTENSION_TMP);
	
	/* Íî»¥¤·¤¿¿Í¤Î£Ã£Ä¥­¡¼ */
	//getcdkeyFromCharaIndex( pindex , cdkey , sizeof(cdkey ));
	cdkey = CHAR_getChar( pindex, CHAR_CDKEY);
	
	/* Íî»¥¤·¤¿¿Í¤Î¥­¥ã¥éÌ¾ */
	makeEscapeString(  CHAR_getChar( pindex, CHAR_NAME),
						charaname, sizeof( charaname));
	
	/* Íî»¥¤·¤¿¿Í¤Î¾Î¹æ */
	makeEscapeString(  CHAR_getChar( pindex, CHAR_OWNTITLE),
						owntitle, sizeof( owntitle));

	/* ¥Ç¥ê¥ß¥¿¤Ç¤¯¤¯¤ë */
	snprintf( outbuf, sizeof( outbuf), "%d|%d|%s|%s|%s|\n",
				CHAR_getInt( pindex, CHAR_GOLD),
				CHAR_getWorkInt( meindex,CHAR_WORKINT_NEXTTIME),
				cdkey, charaname, owntitle );


	fp_dest = fopen( filename_dest, "w");
	if( !fp_dest) return -1;
	
	fp_src = fopen( filename, "r");
	if( !fp_src ) {
		/* ºÇ½é¤Î°ì·ï¤Ê¤Î¤Ç¤½¤Î¤Þ¤Þ½ñ¤¤¤Æ½ª¤ï¤ê */
		fclose( fp_dest);
		fp_src = fopen( filename, "w");
		if( !fp_src) return -1;
		fputs( outbuf, fp_src);
		fclose( fp_src);
		return 1;
	}
	else {
		int cnt = 1;
		char buf[128];
		while( fgets( line, sizeof( line), fp_src)) {
			getStringFromIndexWithDelim( line, "|" , 1 , buf,sizeof(buf) );
			if( ret == 0 ) {
				/* Íî»¥¶â³ÛÈæ³Ó */
				if( atoi( buf) < CHAR_getInt( pindex, CHAR_GOLD) ) {
					/* ¶â³Û¤¬¾¡¤Ã¤Æ¤¤¤ë¤Î¤ÇÁÞÆþ */
					fputs( outbuf, fp_dest);
					fputs( line, fp_dest);
					ret = cnt;
					cnt ++;
				}
				else {
					/* ¤½¤ì°Ê³°¤Ï¤½¤Î¤Þ¤Þ½ñ¤¯ */
					fputs( line, fp_dest);
				}
			}
			else {
				/* ¤½¤ì°Ê³°¤Ï¤½¤Î¤Þ¤Þ½ñ¤¯ */
				fputs( line, fp_dest);
			}
			cnt ++;
			if( cnt > NPC_RANKING_DEFAULT ) break;
		}
		/* ¥é¥ó¥­¥ó¥°¤¬£±£°¸Ä¤½¤í¤Ã¤Æ¤¤¤Ê¤¤»þ */
		if( ret == 0 && cnt <= NPC_RANKING_DEFAULT ) {
			fputs( outbuf, fp_dest);
			ret = cnt;
		}
	}
	fclose( fp_src);
	fclose( fp_dest);
	/* Ãæ¿ÈÆþ¤ì´¹¤¨ */
	/* Ã¯¤«¥Õ¥¡¥¤¥ë¥³¥Ô¡¼¤¹¤ë´ÊÃ±¤ÊÊýË¡ÃÎ¤Ã¤Æ¤¿¤éÊÑ¤¨¤Æ²¼¤µ¤¤¡£ */
	fp_src = fopen( filename_dest, "r");
	if( !fp_src) return -1;
	fp_dest = fopen( filename, "w");
	if( !fp_dest) {
		fclose( fp_src);
		return -1;
	}
	while( fgets( line, sizeof( line), fp_src)) {
		fputs( line, fp_dest);
	}
	fclose( fp_dest);
	fclose( fp_src);
	return( ret);
#undef  NPC_RANKING_DEFAULT
}
/*-----------------------------------------------
 * Íî»¥¼Ô¥é¥ó¥­¥ó¥°¥Õ¥¡¥¤¥ëÆÉ¤ß¹þ¤ß¡õÉ½¼¨
 *
 * °ú¿ô meindex     ¥­¥ã¥é¥¤¥ó¥Ç¥Ã¥¯¥¹¡£¥é¥ó¥­¥ó¥°NPC¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹¡£
 *      talkerindex ÏÃ¤·¤«¤±¤é¤ì¤¿¿Í¤Îindex¡£¤³¤ì¤Ëtalk¤òÁ÷¿®¤¹¤ë¡£
 *      admname     ÉÔÆ°»º²°¤µ¤ó¤ÎÌ¾Á°
 *      
 *----------------------------------------------*/
static void NPC_RoomAdminNew_ReadScoreFile( int meindex, int talkerindex, 
											char *admname)
{
	int     cnt;
	char    filename[128];
	char    line[1024];
	FILE    *fp;
	
	/* ¥Õ¥¡¥¤¥ëÌ¾¤òÀ¸À® */
	strcpy( filename, NPC_ROOMADMINNEW_DATADIR);
	strcat( filename, admname);
	strcat( filename, NPC_ROOMADMINNEW_RANKING_EXTENSION);
	
	fp = fopen( filename, "r");
	if( !fp) return;
	cnt = 1;
	while( fgets( line, sizeof( line), fp)) {
		int     gold,bidtime;
		char    charaname[32],owntitle[32];
		char    tmp[64];
		char    outbuf[1024], buf[256];
		struct  tm tmwk;
		
		getStringFromIndexWithDelim( line, "|" , 1 , buf,sizeof(buf) );
		gold = atoi(buf);
		getStringFromIndexWithDelim( line, "|" , 2 , buf,sizeof(buf) );
		bidtime = atoi(buf);
		getStringFromIndexWithDelim( line, "|" , 4 , buf,sizeof(buf) );
		strcpy( charaname, makeStringFromEscaped( buf));
		getStringFromIndexWithDelim( line, "|" , 5 , buf,sizeof(buf) );
		strcpy( owntitle, makeStringFromEscaped( buf));
		if( strlen( owntitle) != 0 ) {
			strcpy( tmp, "[");
			strcat( tmp, owntitle);
			strcat( tmp, "] ");
			strcpy( owntitle, tmp);
		}
		else {
			strcpy( owntitle, " ");
		}
		memcpy( &tmwk, localtime( (time_t *)&bidtime), sizeof( tmwk));
		
		snprintf( outbuf, sizeof( outbuf),
				NPC_ROOMADMINNEW_MSG_RANKING,
				cnt, 
				tmwk.tm_year + 1900,
				tmwk.tm_mon+1,
				tmwk.tm_mday,
				gold,charaname,owntitle
				);
		CHAR_talkToCli( talkerindex, meindex,outbuf, 
						NPC_RANKING_MSGCOLOR);
		cnt ++;
	}
	fclose( fp);
	
}
/************************************************
 * ½é´ü½èÍý
 ************************************************/
BOOL NPC_RankingInit( int meindex )
{
	char    buf[256];
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	
	NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr));
	
	/* ½é´ü²½½èÍý */
	
	/* type¥»¥Ã¥È */
	CHAR_setInt( meindex, CHAR_WHICHTYPE, CHAR_TYPERANKING);
	/* ´ÉÍý¿Í¤ÎÌ¾Á°¼èÆÀ */
	if( !getStringFromIndexWithDelim(argstr, "|" , 1 , buf,sizeof(buf) )) {
		print( "arg err.rankingNPC not created.\n");
		return( FALSE);
	}
	CHAR_setWorkChar( meindex , CHAR_WORKCHAR_ADMNAME , buf);
	
	return TRUE;
}
/************************************************
 * ÏÃ¤·¤«¤±¤é¤ì¤¿»þ¤Î½èÍý
 ************************************************/
void NPC_RankingTalked( int meindex , int talkerindex , char *msg ,
							 int color )
{
	/* ¥×¥ì¥¤¥ä¡¼¤¬¥É¥¢¥Þ¥ó¤Î1¥°¥ê¥Ã¥É°ÊÆâ¤Ê¤éÈ¿±þ¤¹¤ë¡£ */
	if(NPC_Util_CharDistance( talkerindex, meindex ) > 1)return;
	
	CHAR_talkToCli( talkerindex, meindex,
					NPC_ROOMADMINNEW_MSG_RANKING_INFO, 
					NPC_RANKING_MSGCOLOR);
	
	NPC_RoomAdminNew_ReadScoreFile( meindex, talkerindex, 
						CHAR_getWorkChar( meindex, CHAR_WORKCHAR_ADMNAME));
	
}
/************************************************
 * ½é´ü½èÍý
 ************************************************/
BOOL NPC_PrintpassmanInit( int meindex )
{
	char    buf[256];
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	
	NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr));
	
	/* ½é´ü²½½èÍý */
	
	/* type¥»¥Ã¥È */
	CHAR_setInt( meindex, CHAR_WHICHTYPE, CHAR_TYPEPRINTPASSMAN);
	/* ´ÉÍý¿Í¤ÎÌ¾Á°¼èÆÀ */
	if( !getStringFromIndexWithDelim(argstr, "|" , 1 , buf,sizeof(buf) )) {
		print( "arg err.passmanNPC not created.\n");
		return( FALSE);
	}
	CHAR_setWorkChar( meindex , CHAR_WORKCHAR_ADMNAME , buf);
	
	return TRUE;
}
/************************************************
 * ÏÃ¤·¤«¤±¤é¤ì¤¿»þ¤Î½èÍý
 ************************************************/
void NPC_PrintpassmanTalked( int meindex , int talkerindex , char *msg ,
							 int color )
{
	int i;
	int charnum;
	/* ¥×¥ì¥¤¥ä¡¼¤¬¥É¥¢¥Þ¥ó¤Î2¥°¥ê¥Ã¥É°ÊÆâ¤Ê¤éÈ¿±þ¤¹¤ë¡£ */
	if(NPC_Util_CharDistance( talkerindex, meindex ) > 2)return;
	
	/* ¼«Ê¬ÍÑ¤Î´ÉÍý¿Í¤ÎÌ¾Á°¤ò¸¡º÷¤¹¤ë*/
	charnum  = CHAR_getCharNum();
	for(i= CHAR_getPlayerMaxNum() +CHAR_getPetMaxNum();
		i<charnum;
		i++){

		if( CHAR_getCharUse(i) &&
			CHAR_getInt( i ,CHAR_WHICHTYPE) == CHAR_TYPEROOMADMIN ){
			if( strcmp( CHAR_getWorkChar(i,CHAR_WORKCHAR_MYNAME),
						CHAR_getWorkChar( meindex, CHAR_WORKCHAR_ADMNAME))
				==0 )
			{
				break;
			}
		}
	}
	if( i < charnum ) {
		NPC_RoomAdminNew_DspRoomPasswd( meindex, i, talkerindex);
	}
	else {
		CHAR_talkToCli( talkerindex, meindex, "¡C¡C¡C", NPC_RANKING_MSGCOLOR);
	}
}
/* end of roomadminnew */
