#define __CONFIGFILE_C__
#include "version.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "util.h"
//#include "configfile.h"
//ttom
#include "lssproto_util.h"
#include "configfile.h"
#include "net.h"
//ttom end
#include "npcutil.h"
// Arminius 7.12 login announce
#include "char.h"

// CoolFish: add 
#include "lssproto_serv.h"

#include "npc_raceman.h"

/* ÀßÄê¤òÊÝ»ý¤¹¤ë¹½Â¤ÂÎ */
typedef struct tagConfig
{
    /*¥×¥í¥°¥é¥àÌ¾(¼«Æ°Åª¤Ëµá¤á¤¿¤¤¤±¤É¤Þ¤ÀÂÌÌÜ*/
    char    progname[8];

    char    configfilename[32]; /* config¥Õ¥¡¥¤¥ëÌ¾ */
    unsigned char debuglevel;   /* ¥Ç¥Ð¥Ã¥°¥ì¥Ù¥ë */

    unsigned int  usememoryunit;    /*¥á¥â¥ê¤Î¥æ¥Ë¥Ã¥È¥µ¥¤¥º */
    unsigned int  usememoryunitnum; /*¥á¥â¥ê¤Î¥æ¥Ë¥Ã¥È¿ô */

    char    asname[32];         /*¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤ÎÌ¾Á°*/
    unsigned short  acservport; /*¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤Î¥Ý¡¼¥ÈÌ¾*/
    char    acpasswd[32];       /*¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤Ø¤Î¥Ñ¥¹¥ï¡¼¥É*/
    char    gsnamefromas[32];   /*
                                 * ¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤«¤é¸«¤¨¤ë
                                 * ¥²¡¼¥à¥µ¡¼¥Ð¤È¤·¤Æ¤ÎÌ¾Á°
                                 */
                                 
    // Arminius 7.24 manor pk
    char gsid[32];	// game server chinese id
#ifdef _SERVICE    
    // Terry 2001/10/03
    char apid[32];      // service ap id
		unsigned short apport;	//service ap port
    int  looptime;	// ´X¬í«á³]©wÂ÷¶}
    int  enableservice;	// ¬O§_¨Ï¥Î¥Û¾¹ªA°È­û¥\¯à
#endif    
    unsigned short allowmanorpk;	// is this server allow manor pk
                                 
    unsigned short port;        /* ¥µ¡¼¥Ð¤ÎÂÔ¤Á¼õ¤±¥Ý¡¼¥È */
	
	int				servernumber;	/* ¥²¡¼¥à¥µ¡¼¥Ð¤ÎÈÖ¹æ */
	int				reuseaddr;	/* Address already used... ¤¬»ß¤Þ¤é¤Ê¤¤»þ¤Î¤¿¤á¤Ë */
    int             do_nodelay;     /* TCP_NODELAY ¤Ë¤¹¤ë¤«¤É¤¦¤« */
    int             log_write_time; /* ½ñ¤­¤³¤ß¥í¥°¤ò¤¹¤ë¤«¤É¤¦¤«¡£ */
    int             log_io_time;    /* I/OÁ´ÂÎ¤Î»þ´Ö¤Ï¤«¤ë¤«¤É¤¦¤«¡£ */
    int             log_game_time;  /* ¥²¡¼¥à¤Î½èÍýÁ´ÂÎ¤Î»þ´Ö¤ò¤Ï¤«¤ë */
    int             log_netloop_faster; /* netloop_faster ¤Î¥í¥° */
	int				saacwritenum;	/* ¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤Ø¤Î²¿¿ÍËè¤Ëwrite¤¹¤ë¤« */
	int				saacreadnum;	/* ¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤«¤é¤Îdispatch ¤ò²¿²ó¤¹¤ë¤« */

    unsigned short fdnum;           /*¤É¤ì¤À¤±¥³¥Í¥¯¥·¥ç¥ó¤òÊÝ»ý¤¹¤ë¤« */
    unsigned int   othercharnum;    /*  ¤½¤ÎÂ¾¤Î¥­¥ã¥é¤Î¿ô  */

    unsigned int objnum;            /* ¥ª¥Ö¥¸¥§¥¯¥È¤ÎºÇÂç¿ô*/
    unsigned int   petcharnum;   	/* ¥Ú¥Ã¥È¤Î¿ô    */
    unsigned int itemnum;           /* ¥¢¥¤¥Æ¥à¤ÎºÇÂç¿ô*/
    unsigned int battlenum;         /* ¥Ð¥È¥ë¤ÎºÇÂç*/

    char    topdir[64];         	/* ¥È¥Ã¥×¥Ç¥£¥ì¥¯¥È¥êÌ¾*/

    char    mapdir[64];         	/* ¥Þ¥Ã¥×¥Ç¥£¥ì¥¯¥È¥êÌ¾*/
    char    maptilefile[64];    	/* ¥Þ¥Ã¥×ÀßÄê¥Õ¥¡¥¤¥ëÌ¾*/
    char    battlemapfile[64];    	/* ¥Ð¥È¥ë¥Þ¥Ã¥×ÀßÄê¥Õ¥¡¥¤¥ëÌ¾*/
    char    itemfile[64];       	/* ¥¢¥¤¥Æ¥àÀßÄê¥Õ¥¡¥¤¥ëÌ¾*/
    char    invfile[64];        	/* ÌµÅ¨ÀßÄê¥Õ¥¡¥¤¥ëÌ¾*/
    char    appearfile[64];     	/* ½Ð¸½°ÌÃÖÀßÄê¥Õ¥¡¥¤¥ëÌ¾*/
	char	titlenamefile[64];		/* ¾Î¹æ¥Õ¥¡¥¤¥ëÌ¾ */
	char	titleconfigfile[64];	/* ¾Î¹æÀßÄê¥Õ¥¡¥¤¥ëÌ¾ */
	char	encountfile[64];		/* ¥¨¥ó¥«¥¦¥ó¥ÈÀßÄê¥Õ¥¡¥¤¥ëÌ¾ */
	char	enemybasefile[64];		/* Å¨´ðËÜÀßÄê¥Õ¥¡¥¤¥ëÌ¾ */
	char	enemyfile[64];			/* Å¨ÀßÄê¥Õ¥¡¥¤¥ëÌ¾ */
	char	groupfile[64];			/* ¥°¥ë¡¼¥×ÀßÄê¥Õ¥¡¥¤¥ëÌ¾ */
	char	magicfile[64];			/* ¼ö½ÑÀßÄê¥Õ¥¡¥¤¥ëÌ¾ */
	        #ifdef __ATTACK_MAGIC


        char    attmagicfile[64];       // §ðÀ»©Ê©G³N

        #endif

	char	petskillfile[64];		/* ¥Ú¥Ã¥Èµ»¼ö½ÑÀßÄê¥Õ¥¡¥¤¥ëÌ¾ */
    char    itematomfile[64];       /* ¥¢¥¤¥Æ¥à¤ÎºàÎÁÉ½¥Õ¥¡¥¤¥ë */
    char    effectfile[64];     	/* ±é½ÐÀßÄê¥Õ¥¡¥¤¥ëÌ¾*/
    char    quizfile[64];     		/* ¥¯¥¤¥ºÀßÄê¥Õ¥¡¥¤¥ëÌ¾*/


    char    lsgenlog[64];       /*¥µ¡¼¥Ð¤Îlsgen ¥¢¥¦¥È¥×¥Ã¥È¥Õ¥¡¥¤¥ëÌ¾*/

    char    storedir[64];       /*¥¹¥È¥¢¥Ç¥£¥ì¥¯¥È¥êÌ¾  */
    char    npcdir[64];         /*NPC¤ÎÀßÄê¥Õ¥¡¥¤¥ë¤òÃÖ¤¯¥Ç¥£¥ì¥¯¥È¥ê   */

    char    logdir[64];         /*
                                 * ¥í¥°¥Ç¥£¥ì¥¯¥È¥ê
                                 */
    char    logconfname[64];    /*
                                 * ¥í¥°ÀßÄê¥Õ¥¡¥¤¥ëÌ¾
                                 */
    char	chatmagicpasswd[64];	/* ¥Á¥ã¥Ã¥ÈËâË¡¥Ñ¥¹¥ï¡¼¥É */

#ifdef _STORECHAR
    char	storechar[64];
#endif
	
    unsigned int 	chatmagiccdkeycheck;	/* ¥Á¥ã¥Ã¥ÈËâË¡¤ÇCDKEY¤ò¥Á¥§¥Ã¥¯¤¹¤ë¤« */
    
    unsigned int    filesearchnum;     /*¥Õ¥¡¥¤¥ë¤ò¸¡º÷¤Ç¤­¤ë¥Õ¥¡¥¤¥ë¤Î¿ô*/
    unsigned int    npctemplatenum;     /*NPC¤Î¥Æ¥ó¥×¥ì¡¼¥È¥Õ¥¡¥¤¥ë¤Î¿ô*/
    unsigned int    npccreatenum;       /*NPC¤ÎÀ¸»º¥Õ¥¡¥¤¥ë¤Î¿ô*/
    unsigned int    walksendinterval;   /* Êâ¤¯¤Î¤òÁ÷¤ë´Ö³Ö */
    unsigned int    CAsendinterval_ms;     /* CA¤òÁ÷¤ë´Ö³Ö (ms)*/
    unsigned int    CDsendinterval_ms;     /* CD¤òÁ÷¤ë´Ö³Ö (ms)*/
    unsigned int    Onelooptime_ms;     	/* 1¥ë¡¼¥×¤Ë¤«¤±¤ë»þ´Ö */
	unsigned int	Petdeletetime;		/* ¥Ú¥Ã¥È¤¬¾ÃÌÇ¤¹¤ë»þ´Ö */
	unsigned int	Itemdeletetime;		/* ¥¢¥¤¥Æ¥à¤¬¾ÃÌÇ¤¹¤ë»þ´Ö */
    /* ¥í¥°¥¤¥óÃæ¤Î¥­¥ã¥é¤Î¥»¡¼¥Ö¤ò¤¹¤ë´Ö³Ö */
    unsigned int    CharSavesendinterval;
    unsigned int    addressbookoffmsgnum;  /*
                                            * ¥¢¥É¥ì¥¹¥Ö¥Ã¥¯¤Ë¥ª¥Õ¥é¥¤¥ó
                                            * ¥á¥Ã¥»¡¼¥¸¤ò
                                            * ²¿¥á¥Ã¥»¡¼¥¸»Ä¤»¤ë¤«
                                            */

    unsigned int    protocolreadfrequency;  /*
                                             * ¥×¥í¥È¥³¥ë¤ò²¿¥ß¥êÉÃËè
                                             * ¤ËÆÉ¤à¤«
                                             */
    unsigned int    allowerrornum;          /*
                                             * ¥¨¥é¡¼¤ò²¿¸Ä¤Þ¤Çµö¤¹¤«
                                             */
    unsigned int    loghour;          		/*
                                             * ¥í¥°¤òÊÝÂ¸¤¹¤ë»þ¹ï¡Ê¡©»þ¡Ë
                                             */
    unsigned int    battledebugmsg;    		/*
                                             * ¥Ð¥È¥ëÃæ¤Î¥Ç¥Ð¥Ã¥°¥á¥Ã¥»¡¼¥¸¤ò½Ð¤¹¤«¡££°¤Ê¤é½Ð¤µ¤Ê¤¤
                                             */
    //ttom add this because the second had this                                         
    unsigned int    encodekey;              
    unsigned int    acwbsize;             
    unsigned int    acwritesize;
    unsigned int    ErrUserDownFlg;
    //ttom end
#ifdef _GMRELOAD
	char	gmsetfile[64];				/* GM±b¸¹¡BÅv­­³]©wÀÉ */
#endif

#ifdef _AUCTIONEER
  char auctiondir[256];   // ©ç½æ¸ê®Æ¥Ø¿ý
#endif
#ifdef _BLACK_MARKET 
  char blackmarketfile[256];
#endif
#ifdef _M_SERVER
    char    msname[32];         
    unsigned short  msport;
#endif
#ifdef _NPCSERVER_NEW
	char    nsaddress[64];         
	unsigned short  nsport;
#endif

#ifdef _PROFESSION_SKILL			// WON ADD ¤Hª«Â¾·~§Þ¯à
	char profession[64];
#endif

#ifdef _ITEM_QUITPARTY
    char itemquitparty[64];
#endif

#ifdef _MUSEUM
	int museum;
#endif

#ifdef _DEL_DROP_GOLD	
	unsigned int	Golddeletetime;
#endif

}Config;
Config config;

/*
 *  ¥³¥ó¥Õ¥£¥°¥Õ¥¡¥¤¥ë¤òÆÉ¤à»þ¤Ë»È¤¦¹½Â¤ÂÎ
 *  xxxx=yyyy ¤ß¤¿¤¤¤Ê¤Î¤òÆÉ¤à
 */

typedef struct tagReadConf
{
    char    name[32];       /*xxxx¤Ë¤¢¤¿¤ëÃÍ*/

    /*¼¡¤Î2¤Ä¤ÏNULL¤òÆþ¤ì¤ë¤È²¿¤â½èÍý¤·¤Ê¤¤*/
    char  *charvalue;      /*yyyy¤ò¤½¤Î¤Þ¤ÞÂåÆþ¤¹¤ë»þ¤ÎÂåÆþÀè*/
    size_t  charsize;       /*charvalue¤Î¥µ¥¤¥º*/

    /*
     * ÆÃÊÌ»ÅÍÍ¤È¤·¤Æ=¤Î¸å¤¬ "ON"¤À¤Ã¤¿¤é intvalue ¤Ë¤Ï1¤òÂåÆþ¤¹¤ë
     * ¤½¤ì°Ê³°¤Ï atoi ¤Î·ë²Ì
     */
    void*    value;       /*yyyy¤òÊÑ´¹¤¹¤ëÂåÆþ¤¹¤ë»þ¤ÎÂåÆþÀè*/
    CTYPE    valuetype;
}ReadConf;

ReadConf readconf[]=
{
    { "debuglevel"      , NULL ,0 , (void*)&config.debuglevel      ,CHAR},

    { "usememoryunit"   , NULL ,0 , (void*)&config.usememoryunit   ,INT},
    { "usememoryunitnum", NULL ,0 , (void*)&config.usememoryunitnum,INT},

    { "acserv",			config.asname,sizeof(config.asname) ,NULL , 0},
    { "acservport",		NULL ,0 , (void*)&config.acservport     ,SHORT},
    { "acpasswd",		config.acpasswd,sizeof( config.acpasswd),NULL,0},
    { "gameservname",	config.gsnamefromas,sizeof(config.gsnamefromas),
     NULL,0},

    // Arminius 7.24 manor pk
    { "gameservid", config.gsid, sizeof(config.gsid), NULL, 0},
#ifdef _SERVICE    
    // Terry 2001/10/03 service ap
    { "apid", config.apid, sizeof(config.apid), NULL, 0},
		{ "apport",	NULL ,0 ,(void*)&config.apport ,SHORT},
    { "looptime",NULL,0,(void*)&config.looptime,INT},
    { "enableservice",NULL,0,(void*)&config.enableservice,INT},
#endif    
    { "allowmanorpk", NULL, 0, (void*)&config.allowmanorpk, SHORT},

    { "port",			NULL ,0 , (void*)&config.port           ,SHORT},
    { "servernumber",	NULL ,0 , (void*)&config.servernumber           ,INT},

    { "reuseaddr",			NULL ,0 , (void*)&config.reuseaddr  ,		INT},
    { "nodelay",			NULL , 0 , (void*)&config.do_nodelay ,		INT},
    { "log_write_time", 	NULL, 0 , (void*)&config.log_write_time,	INT},
    { "log_io_time", 		NULL, 0 , (void*)&config.log_io_time, 		INT},
    { "log_game_time",		NULL, 0 , (void*)&config.log_game_time,		INT},
    { "log_netloop_faster", NULL,0,(void*)&config.log_netloop_faster,	INT},
    { "saacwritenum",		NULL,0,(void*)&config.saacwritenum, 		INT},
    { "saacreadnum",		NULL,0,(void*)&config.saacreadnum, 			INT},
    { "fdnum",				NULL ,0 , (void*)&config.fdnum,				SHORT},
    { "petnum",				NULL ,0 , (void*)&config.petcharnum,		INT},
    { "othercharnum",		NULL ,0 , (void*)&config.othercharnum,		INT},

    { "objnum",			NULL ,0 , (void*)&config.objnum,				INT},
    { "itemnum",		NULL ,0 , (void*)&config.itemnum,				INT},
    { "battlenum",		NULL ,0 , (void*)&config.battlenum,				INT},

    { "topdir"          , config.topdir,sizeof(config.topdir),NULL,0},
    { "mapdir"          , config.mapdir,sizeof(config.mapdir),NULL,0},
    { "maptilefile"     , config.maptilefile,sizeof(config.maptilefile),NULL,0},
    { "battlemapfile"   , config.battlemapfile,sizeof(config.battlemapfile),NULL,0},

#ifdef _ITEMSET6_TXT
	{ "itemset6file",	config.itemfile,	sizeof(config.invfile),	NULL,	0},
#else
#ifdef _ITEMSET5_TXT
	{ "itemset5file",	config.itemfile,	sizeof(config.invfile),	NULL,	0},
#else
#ifdef _ITEMSET4_TXT
	{ "itemset4file"  , config.itemfile,sizeof(config.invfile),NULL,0},
#else
#ifdef _ITEMSET3_ITEM
	{ "itemset3file"  , config.itemfile,sizeof(config.invfile),NULL,0},
#endif
#endif
#endif
#endif
    { "invinciblefile"  , config.invfile,sizeof(config.invfile),NULL,0},
    { "appearpositionfile"  , config.appearfile,sizeof(config.appearfile),NULL,0},
	{ "titlenamefile", config.titlenamefile, sizeof( config.titlenamefile),NULL,0},
	{ "titleconfigfile", config.titleconfigfile, sizeof( config.titleconfigfile),NULL,0},
	{ "encountfile", config.encountfile, sizeof( config.encountfile),NULL,0},
	{ "enemyfile", config.enemyfile, sizeof( config.enemyfile),NULL,0},
	{ "enemybasefile", config.enemybasefile, sizeof( config.enemybasefile),NULL,0},
	{ "groupfile", config.groupfile, sizeof( config.groupfile),NULL,0},
	{ "magicfile", config.magicfile, sizeof( config.magicfile),NULL,0},
#ifdef __ATTACK_MAGIC
	{ "attmagicfile" , config.attmagicfile , sizeof( config.attmagicfile )  , NULL , 0 },
#endif

#ifdef _PETSKILL2_TXT
	{ "petskillfile2", config.petskillfile, sizeof( config.petskillfile),NULL,0},
#else
	{ "petskillfile1", config.petskillfile, sizeof( config.petskillfile),NULL,0},
#endif

    { "itematomfile" , config.itematomfile, sizeof( config.itematomfile),NULL,0},
    { "effectfile"  , config.effectfile,sizeof(config.effectfile),NULL,0},
    { "quizfile"  , config.quizfile,sizeof(config.quizfile),NULL,0},

    { "lsgenlogfilename", config.lsgenlog,sizeof(config.lsgenlog),NULL,0},
#ifdef _GMRELOAD
	{ "gmsetfile", config.gmsetfile, sizeof( config.gmsetfile),NULL,0},
#endif

    { "storedir"        ,config.storedir,sizeof(config.storedir),NULL,0},
    { "npcdir"          ,config.npcdir,sizeof(config.npcdir),NULL,0},
    { "logdir"          ,config.logdir,sizeof(config.logdir),NULL,0},
    { "logconfname"     ,config.logconfname,sizeof(config.logconfname),NULL,0},
    { "chatmagicpasswd", config.chatmagicpasswd, sizeof( config.chatmagicpasswd),NULL,0},
#ifdef _STORECHAR
    { "storechar", config.storechar, sizeof( config.storechar),NULL,0},
#endif
    { "chatmagiccdkeycheck",  NULL,0, &config.chatmagiccdkeycheck,INT},
    { "filesearchnum",  NULL,0, &config.filesearchnum,INT},
    { "npctemplatenum",  NULL,0, &config.npctemplatenum,INT},
    { "npccreatenum",    NULL,0, &config.npccreatenum,INT},
    { "walkinterval" ,NULL,0,(void*)&config.walksendinterval,INT},
    { "CAinterval" ,NULL,0,(void*)&config.CAsendinterval_ms,INT},
    { "CDinterval" ,NULL,0,(void*)&config.CDsendinterval_ms,INT},
    { "CharSaveinterval" ,NULL,0,(void*)&config.CharSavesendinterval,INT},
    { "Onelooptime" ,NULL,0,(void*)&config.Onelooptime_ms,INT},
    { "Petdeletetime" ,NULL,0,(void*)&config.Petdeletetime,INT},
    { "Itemdeletetime" ,NULL,0,(void*)&config.Itemdeletetime,INT},
	 { "addressbookoffmesgnum" ,NULL,0,
      (void*)&config.addressbookoffmsgnum,INT},

    { "protocolreadfrequency" ,NULL,0,
      (void*)&config.protocolreadfrequency,INT},

    { "allowerrornum" ,NULL,0,(void*)&config.allowerrornum,INT},
    { "loghour" ,NULL,0,(void*)&config.loghour,INT},
    { "battledebugmsg" ,NULL,0,(void*)&config.battledebugmsg,INT},
    //ttom add because the second had
    { "encodekey" ,NULL,0,(void*)&config.encodekey,INT},
    { "acwritesize" ,NULL,0,(void*)&config.acwritesize,INT},
    { "acwbsize" ,NULL,0,(void*)&config.acwbsize,INT},
    { "erruser_down" ,NULL,0,(void*)&config.ErrUserDownFlg,INT},    
    //ttom end
#ifdef _AUCTIONEER
    { "auctiondir" , config.auctiondir, sizeof(config.auctiondir),NULL,0},
#endif
#ifdef _BLACK_MARKET
	{ "blackmarketfile", config.blackmarketfile, sizeof(config.blackmarketfile), NULL, 0},
#endif
#ifdef _M_SERVER
    { "msname",		config.msname,sizeof(config.msname) ,NULL , 0},
    { "msport",		NULL ,0 , (void*)&config.msport     ,SHORT},
#endif
#ifdef _NPCSERVER_NEW
    { "npcaddress",		config.nsaddress, sizeof(config.nsaddress) ,NULL , 0},
    { "nsport",		NULL, 0 , (void*)&config.nsport     ,SHORT},
#endif

#ifdef _PROFESSION_SKILL			// WON ADD ¤Hª«Â¾·~§Þ¯à
    { "profession",		config.profession, sizeof(config.profession) ,NULL , 0},
#endif

#ifdef _ITEM_QUITPARTY
    { "itemquitparty",	config.itemquitparty, sizeof(config.itemquitparty) ,NULL , 0},
#endif

#ifdef _MUSEUM
    { "museum",			NULL , 0 , (void*)&config.museum ,		INT},
#endif

#ifdef _DEL_DROP_GOLD
	{ "Golddeletetime" ,NULL,0,(void*)&config.Golddeletetime,	INT},
#endif

};

// Arminius 7.12 login announce
char announcetext[8192];
void AnnounceToPlayer(int charaindex)
{
  char *ptr,*qtr;
  
  ptr=announcetext;
  while ((qtr=strstr(ptr,"\n"))!=NULL) {
    qtr[0]='\0';
//    printf("ptr=%s\n",ptr);
    CHAR_talkToCli(charaindex, -1, ptr, CHAR_COLORYELLOW);
    qtr[0]='\n';
    ptr=qtr+1;
  }
  CHAR_talkToCli(charaindex, -1, ptr, CHAR_COLORYELLOW);
  
}

// Robin 0720
void AnnounceToPlayerWN(int fd)
{
	char buf[8192];
	lssproto_WN_send( fd , WINDOW_MESSAGETYPE_LOGINMESSAGE,
		WINDOW_BUTTONTYPE_OK,
		-1, -1,
		makeEscapeString( announcetext, buf, sizeof(buf))
		);
}


void LoadAnnounce(void)
{
    FILE *f;

    memset(announcetext, 0, sizeof(announcetext));
    if ((f=fopen("./announce.txt","r"))!=NULL) {
      fread(announcetext, sizeof(announcetext), 1, f);
      announcetext[sizeof(announcetext)-1]='\0';
      fclose(f);
    }
}
#ifdef _PET_TALKPRO
PTALK pettalktext[PETTALK_MAXID];

void LoadPetTalk(void)
{
	FILE *fp;
	char fn[256];
	char line[ 4096];
	char talkmem[4096];
	int maxid=0;
	char buf1[256], buf2[256], buf3[256];
	int talkNO=-1, mark=-1, i;
	int len = sizeof( talkmem);
	
	memset(talkmem, 0, sizeof(talkmem));
	sprintf(fn, "%s/pettalk/pettalk.menu", getNpcdir());

	for( i=0;i<PETTALK_MAXID;i++)	{
		pettalktext[i].ID = -1;
		strcpy( pettalktext[i].DATA, "\0");
	}

	print("\n LoadPetTalk file:%s", fn);
	fp = fopen( fn, "r");
    if( fp != NULL ) {
		while( fgets( line, sizeof( line), fp)) {
			if( strlen( talkmem) != 0 ) {
				if( talkmem[strlen( talkmem) -1] != '|' ) {
					strcatsafe( talkmem, len, "|");		
				}
			}
			chompex( line);
			strcatsafe( talkmem,len,  line);
		}
		fclose( fp);
    }else	{
		print("...err:not found !");
	}

	talkNO=1;
	while( getStringFromIndexWithDelim( talkmem,"END",talkNO, buf1, sizeof( buf1)) != FALSE	){
		talkNO++;
		if( NPC_Util_GetStrFromStrWithDelim( buf1, "PETTEMPNO", buf2, sizeof( buf2)) == NULL  )
			continue;
		mark=1;
		strcpy( fn,"\0");

		if( getStringFromIndexWithDelim( buf2,",", mark+1,buf3,sizeof( buf3)) != FALSE )	{
			pettalktext[maxid].ID = atoi( buf3);
			if( getStringFromIndexWithDelim( buf2,",", mark,buf3,sizeof( buf3)) != FALSE )	{
				sprintf(fn, "%s/pettalk/%s", getNpcdir(), buf3);
				//print("\n ...file:%s", fn);
				fp = fopen( fn, "r");
				if( fp != NULL )	{
					char line[4096];
					while( fgets( line, sizeof( line), fp ) ) {
						if( strlen( pettalktext[maxid].DATA) != 0 ) {
							if( pettalktext[maxid].DATA[strlen( pettalktext[maxid].DATA) -1] != '|' ) {
								strcatsafe( pettalktext[maxid].DATA, sizeof( pettalktext[maxid].DATA), "|");
							}
						}
						chompex( line);
						strcatsafe( pettalktext[maxid].DATA, sizeof( pettalktext[maxid].DATA), line);
					}
					maxid++;
					fclose( fp);
				}else	{
					print("... err:[%s] not found!", fn);
					pettalktext[maxid].ID=-1;
				}
			}else	{
				pettalktext[maxid].ID=-1;
			}
		}
		print(".");
		if( maxid >= PETTALK_MAXID )
			break;
	}
	print("\n.......maxid=%d", maxid);
	{
		int haveid=0;
		for( i=0;i<PETTALK_MAXID;i++)	{
			if( pettalktext[i].ID >= 0 )	{
				haveid++;
			}
		}
		print("...haveid=%d", haveid);
	}

}

#else
char pettalktext[4096];
void LoadPetTalk(void)
{
  FILE *fp;
  char fn[256];
  char	line[ 4096];
  int len = sizeof( pettalktext);
  
  memset(pettalktext, 0, sizeof(pettalktext));
  sprintf(fn, "%s/pettalk/pettalk.mem", getNpcdir());
  
  fp = fopen( fn, "r");
    if( fp != NULL ) {
		print("\n\n READ pettalk.mem");
		while( fgets( line, sizeof( line), fp)) {
			if( strlen( pettalktext) != 0 ) {
				if( pettalktext[strlen( pettalktext) -1] != '|' ) {
					strcatsafe( pettalktext, len, "|");		
				}
			}
			chompex( line);
			strcatsafe( pettalktext,len,  line);
		}
		fclose( fp);
		print("\n %s", pettalktext);
    }else	{
		print("\n Could't Found pettalk.mem");
	}
}
#endif

#ifdef _GAMBLE_BANK
GAMBLEBANK_ITEMS GB_ITEMS[GAMBLEBANK_ITEMSMAX];

void Load_GambleBankItems( void)
{

	FILE *fp;
	char filename[256];
	char buf1[256];
	char name[128];
	int num,ID,type;
	int i=0;
	sprintf(filename, "./data/gambleitems.txt" );
	print("\n Load GambleItems file:%s ...", filename);
	fp = fopen( filename, "r");
    if( fp != NULL ) {
		while( fgets( buf1, sizeof( buf1), fp) != NULL )	{
			if( strstr( buf1, "#") != 0 ) continue;
			sscanf( buf1,"%s %d %d %d", name, &ID, &num , &type);
			strcpy( GB_ITEMS[i].name, name);
			GB_ITEMS[i].Gnum = num;
			GB_ITEMS[i].ItemId = ID;
			GB_ITEMS[i].type = type;
			i++;
		}
		print("..maxID: %d ", i);
		fclose( fp);
    }else	{
		print("err not found %s", filename);
	}

}
#endif


#ifdef _CFREE_petskill
PETSKILL_CODES Code_skill[PETSKILL_CODE];
void Load_PetSkillCodes( void)
{
	FILE *fp;
	char filename[256];
	char buf1[256];
	char name[128];
	char type[256];
	int num,ID;
	int i=0;
	sprintf(filename, "./data/skillcode.txt" );
	print("\n Load PetSKill Code file:%s", filename);
	fp = fopen( filename, "r");
    if( fp != NULL ) {
		while( fgets( buf1, sizeof( buf1), fp) != NULL )	{
			sscanf( buf1,"%s %d %d %s", name, &num, &ID, type);
			strcpy( Code_skill[i].name, name);
			Code_skill[i].TempNo = num;
			Code_skill[i].PetId = ID;
			strcpy( Code_skill[i].Code, type);
			//print("\n %s|%d|%d|%s|", Code_skill[i].name, Code_skill[i].TempNo, 
			//	Code_skill[i].PetId, Code_skill[i].Code);
			i++;
			if( i >= PETSKILL_CODE ) break;
		}
		fclose( fp);
    }else	{
		print("...not found %s", filename);
	}
	print("\n");
}
#endif

#ifdef _BLACK_MARKET
BOOL LoadBMItem( char* filename)
{
	FILE *fp;	
	int  i, j, k;
	char line[512]="", cTmp[256]="";
	char *ip=NULL, *gp=NULL;	
    
	for(i=0; i<BMIMAX; i++){		
		BMItem[i].iGraphicsNum      = 0;
		BMItem[i].GCondition        = 0;
		for(j=0; j<4; j++){			
		    BMItem[i].iCondition[j] = 0;
			for(k=0; k<3; k++){			
				BMItem[i].iId[j][k] = 0;
			}
		}
		strcpy( BMItem[i].iName, "");
	}	
	for(i=0; i<12; i++) BMSellList[i] = -1;

	fp = fopen( filename, "r");

	if(fp==NULL){
		print("\nFault!! Can't Open File:%s ...\n", filename);
		return FALSE;
	}else{
		while(fgets( line, sizeof(line), fp)!=NULL){			
			char cTmp1[256]="", cTmp2[256]="", cTmp3[256]="";
			char iTmp1[128]="", iTmp2[128]="", iTmp3[128]="", iTmp4[128]="", iTmp5[128]="";

			if(BMINum>=BMIMAX){
				print("\nWarning!! To beyond the scope of the itemnum(%d).", BMIMAX);
				break;
			}
			sscanf( line, "%s %d %s %s %s %s %s", 
				    BMItem[BMINum].iName,
					&BMItem[BMINum].iGraphicsNum,
				    iTmp1, iTmp2, iTmp3, iTmp4,	cTmp);
			
            for(i=0; i<3; i++){
				if(getStringFromIndexWithDelim( iTmp1, ",", i+1, iTmp5, sizeof( iTmp5))!=FALSE)
					BMItem[BMINum].iId[0][i] = atoi(iTmp5);
				if(getStringFromIndexWithDelim( iTmp2, ",", i+1, iTmp5, sizeof( iTmp5))!=FALSE)
					BMItem[BMINum].iId[1][i] = atoi(iTmp5);
				if(getStringFromIndexWithDelim( iTmp3, ",", i+1, iTmp5, sizeof( iTmp5))!=FALSE)
					BMItem[BMINum].iId[2][i] = atoi(iTmp5);
				if(getStringFromIndexWithDelim( iTmp4, ",", i+1, iTmp5, sizeof( iTmp5))!=FALSE)
					BMItem[BMINum].iId[3][i] = atoi(iTmp5);
			}

			ip = strstr( cTmp, "I");
			gp = strstr( cTmp, "G");	

			if( ip && gp && gp>ip){
				strncpy( cTmp1, ip+1, gp-ip-1);				
				for(i=0; i<4; i++)
					if(getStringFromIndexWithDelim( cTmp1, ",", i+1, cTmp3, sizeof( cTmp3))!=FALSE)
						BMItem[BMINum].iCondition[i] = atoi(cTmp3);			
				strcpy( cTmp2, gp+1);
				BMItem[BMINum].GCondition = atoi(cTmp2);	
			}else if( ip && gp && gp<ip){
				strcpy( cTmp1, ip+1);				
				for(i=0; i<4; i++)
					if(getStringFromIndexWithDelim( cTmp1, ",", i+1, cTmp3, sizeof( cTmp3))!=FALSE)
						BMItem[BMINum].iCondition[i] = atoi(cTmp3);				
				strncpy( cTmp2, gp+1, ip-gp-1);
				BMItem[BMINum].GCondition = atoi(cTmp2);
			}else if( gp && !ip){
				strcpy( cTmp2, gp+1);
				BMItem[BMINum].GCondition = atoi(cTmp2);
			}else if( !gp && ip){
				strcpy( cTmp1, ip+1);
				for(i=0; i<4; i++)
					if(getStringFromIndexWithDelim( cTmp1, ",", i+1, cTmp3, sizeof( cTmp3))!=FALSE)
						BMItem[BMINum].iCondition[i] = atoi(cTmp3);
			}else{
				print("\nWarning!! There is not item or gold condition..");
				continue;
			}
			BMINum++;
		}
	}
	for(i=0; i<12; i++)	BMSellList[i] = RAND(0, BMINum-1);		
	fclose(fp);
	return TRUE;
}
#endif

#ifdef _GMRELOAD
BOOL LoadGMSet( char* filename )
{
	FILE* fp;
	int i = 0, gm_num = 0;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		print("File Open Error\n");
		return FALSE;
	}
	for (i = 0; i < GMMAXNUM; i++)
	{
		strcpy(gminfo[i].cdkey, "");
		gminfo[i].level = 0;
	}
	while(1){
		char	line[64], cdkey[64], level[64];
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		chop(line);
		//change ¨Ïgmset.txt¥i¥H¼W¥[µù¸Ñ*******
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		//*************************************
		gm_num = gm_num + 1;
		if (gm_num > GMMAXNUM)	break;
		easyGetTokenFromString(line, 1, cdkey, sizeof(cdkey));
		if (strcmp(cdkey, "") == 0)	break;
		strncpy(gminfo[gm_num].cdkey, cdkey, sizeof(gminfo[gm_num].cdkey));
		easyGetTokenFromString(line, 2, level, sizeof(level));
		if (strcmp(level, "") == 0)	break;
		gminfo[gm_num].level = atoi(level);
//		print("\ncdkey:%s, level:%d", gminfo[gm_num].cdkey, gminfo[gm_num].level);
	}
	fclose(fp);
	return TRUE;
}
#endif

/*------------------------------------------------------------
 * ¥×¥í¥°¥é¥àÌ¾¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  cahr*
 ------------------------------------------------------------*/
char* getProgname( void )
{
    return config.progname;
}
/*------------------------------------------------------------
 * configfilename ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getConfigfilename( void )
{
    return config.configfilename;
}
/*------------------------------------------------------------
 * configfilename ¤òÀßÄê¤¹¤ë¡£
 * °ú¿ô
 *  newv    char*   ¿·¤·¤¤ÃÍ
 * ÊÖ¤êÃÍ
 *  ¤Ê¤·
 ------------------------------------------------------------*/
void setConfigfilename( char* newv )
{
    strcpysafe( config.configfilename, sizeof( config.configfilename ),
                newv );
}

/*------------------------------------------------------------
 * ¥Ç¥Ð¥Ã¥°¥ì¥Ù¥ë¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getDebuglevel( void )
{
    return config.debuglevel;
}
/*------------------------------------------------------------
 * ¥Ç¥Ð¥Ã¥°¥ì¥Ù¥ë¤òÀßÄê¤¹¤ë
 * °ú¿ô
 *  newv    int     ¿·¤·¤¤ÃÍ
 * ÊÖ¤êÃÍ
 *  unsigned int    ÀÎ¤ÎÃÍ
 ------------------------------------------------------------*/
unsigned int setDebuglevel( unsigned int newv )
{
    int old;
    old = config.debuglevel;
    config.debuglevel = newv;
    return old;
}
/*------------------------------------------------------------
 * memoryunit ¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getMemoryunit( void )
{
    return config.usememoryunit;
}
/*------------------------------------------------------------
 * memoryunitnum ¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getMemoryunitnum( void )
{
    return config.usememoryunitnum;
}

/*------------------------------------------------------------
 * ¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤Î¥¢¥É¥ì¥¹¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char*   getAccountservername( void )
{
    return config.asname;
}
/*------------------------------------------------------------
 * ¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤Î¥Ý¡¼¥È¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned short
 ------------------------------------------------------------*/
unsigned short   getAccountserverport( void )
{
    return config.acservport;
}
/*------------------------------------------------------------
 * ¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤Ø¤Î¥Ñ¥¹¥ï¡¼¥É¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned short
 ------------------------------------------------------------*/
char*   getAccountserverpasswd( void )
{
    return config.acpasswd;
}
/*------------------------------------------------------------
 * ¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤«¤é¸«¤¨¤ë¥²¡¼¥à¥µ¡¼¥Ð¤È¤·¤Æ¤ÎÌ¾Á°¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned short
 ------------------------------------------------------------*/
char*   getGameservername( void )
{
    return config.gsnamefromas;
}

// Arminius 7.24 manor pk
char* getGameserverID( void )
{
    if (config.gsid[strlen(config.gsid)-1]=='\n')
      config.gsid[strlen(config.gsid)-1]='\0';
      
    return config.gsid;
}

#ifdef _SERVICE
// Terry 2001/10/03
char* getApID(void)
{
  return config.apid;
}

unsigned short getApPort(void)
{
  return config.apport; 
}

int getLoopTime(void)
{
  return config.looptime;
}

int getEnableService(void)
{
  return config.enableservice;
}
#endif
  
unsigned short getAllowManorPK( void )
{
    return config.allowmanorpk;
}

unsigned short getPortnumber( void )
{
    return config.port;
}
/*------------------------------------------------------------
 * ¥²¡¼¥à¥µ¡¼¥Ð¤ÎÈÖ¹æ ÈÖ¹æ¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned short
 ------------------------------------------------------------*/
int getServernumber( void )
{
    return config.servernumber;
}
/*------------------------------------------------------------
 * reuseaddr ¤ÎÃÍ¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned short
 ------------------------------------------------------------*/
int getReuseaddr( void )
{
    return config.reuseaddr;
}

int getNodelay( void )
{
    return config.do_nodelay;
}
int getLogWriteTime(void)
{
    return config.log_write_time;
}
int getLogIOTime( void)
{
    return config.log_io_time;
}
int getLogGameTime(void)
{
    return config.log_game_time;
}
int getLogNetloopFaster(void)
{
    return config.log_netloop_faster;
}

/*------------------------------------------------------------
 * saacwritenum ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *   int
 ------------------------------------------------------------*/
int getSaacwritenum( void )
{
    return config.saacwritenum;
}
/*------------------------------------------------------------
 * saacwritenum ¤òÀßÄê¤¹¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *   int
 ------------------------------------------------------------*/
void setSaacwritenum( int num )
{
    config.saacwritenum = num;
}
/*------------------------------------------------------------
 * saacreadnum ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *   int
 ------------------------------------------------------------*/
int getSaacreadnum( void )
{
    return config.saacreadnum;
}
/*------------------------------------------------------------
 * saacreadnum ¤òÀßÄê¤¹¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *   int
 ------------------------------------------------------------*/
void setSaacreadnum( int num )
{
    config.saacreadnum = num;
}
/*------------------------------------------------------------
 * fdnum ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getFdnum( void )
{
    return config.fdnum;
}
/*------------------------------------------------------------
 * petcharanum ¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getPetcharnum( void )
{
    return config.petcharnum;
}


/*------------------------------------------------------------
 * othercharnum ¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getOtherscharnum( void )
{
    return config.othercharnum;
}

/*------------------------------------------------------------
 * objnum ¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getObjnum( void )
{
    return config.objnum;
}

/*------------------------------------------------------------
 * itemnum ¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getItemnum( void )
{
    return config.itemnum;
}


/*------------------------------------------------------------
 * battlenum ¤òÆÀ¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getBattlenum( void )
{
    return config.battlenum;
}


/*------------------------------------------------------------
 * topdir ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getTopdir( void )
{
    return config.topdir;
}
/*------------------------------------------------------------
 * mapdir ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getMapdir( void )
{
    return config.mapdir;
}
/*------------------------------------------------------------
 * maptilefile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getMaptilefile( void )
{
    return config.maptilefile;
}
/*------------------------------------------------------------
 * battlemapfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getBattleMapfile( void )
{
    return config.battlemapfile;
}
/*------------------------------------------------------------
 * itemfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getItemfile( void )
{
    return config.itemfile;
}
/*------------------------------------------------------------
 * invfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getInvfile( void )
{
    return config.invfile;
}
/*------------------------------------------------------------
 * appearfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getAppearfile( void )
{
    return config.appearfile;
}
/*------------------------------------------------------------
 * effectfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getEffectfile( void )
{
    return config.effectfile;
}
/*------------------------------------------------------------
 * titlenamefile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getTitleNamefile( void )
{
    return config.titlenamefile;
}
/*------------------------------------------------------------
 * titleconfigfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getTitleConfigfile( void )
{
    return config.titleconfigfile;
}
/*------------------------------------------------------------
 * encountfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getEncountfile( void )
{
    return config.encountfile;
}
/*------------------------------------------------------------
 * enemyfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getEnemyfile( void )
{
    return config.enemyfile;
}
/*------------------------------------------------------------
 * enemybasefile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getEnemyBasefile( void )
{
    return config.enemybasefile;
}
/*------------------------------------------------------------
 * groupfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getGroupfile( void )
{
    return config.groupfile;
}
/*------------------------------------------------------------
 * magicfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getMagicfile( void )
{
    return config.magicfile;
}

#ifdef __ATTACK_MAGIC

/*------------------------------------------------------------
 * ¨ú±o§ðÀ»©Êªº©G³N
 * °Ñ¼Æ
 * None
 * ªð¦^­È
 * char*
 ------------------------------------------------------------*/
char* getAttMagicfileName( void )
{
    return config.attmagicfile;
}

#endif


char* getPetskillfile( void )
{
    return config.petskillfile;
}

#ifdef _PROFESSION_SKILL			// WON ADD ¤Hª«Â¾·~§Þ¯à
char* getProfession( void )
{
    return config.profession;
}
#endif

#ifdef _ITEM_QUITPARTY
char* getitemquitparty( void )
{
    return config.itemquitparty;
}
#endif

char *getItematomfile( void )
{
    return config.itematomfile;
}


char* getQuizfile( void )
{
    return config.quizfile;
}

/*------------------------------------------------------------
 * lsgenlogfile ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getLsgenlogfilename( void )
{
    return config.lsgenlog;
}

#ifdef _BLACK_MARKET
char* getBMItemFile(void){
	return config.blackmarketfile;
}
#endif

#ifdef _GMRELOAD
char* getGMSetfile( void )
{
    return config.gmsetfile;
}
#endif

/*------------------------------------------------------------
 * storedir ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getStoredir( void )
{
    return config.storedir;
}
#ifdef _STORECHAR
/*------------------------------------------------------------
 ------------------------------------------------------------*/
char* getStorechar( void )
{
    return config.storechar;
}
#endif

#ifdef _AUCTIONEER
char* getAuctiondir(void)
{
  return config.auctiondir;
}
#endif

/*------------------------------------------------------------
 * NPC ÍÑ¤Î¥Ç¥£¥ì¥¯¥È¥ê¤òÆÀ¤Æ¤¯¤ë´Ø¿ô
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getNpcdir( void )
{
    return config.npcdir;
}
/*------------------------------------------------------------
 * ¥í¥°¥Ç¥£¥ì¥¯¥È¥ê¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getLogdir( void )
{
    return config.logdir;
}

/*------------------------------------------------------------
 * ¥í¥°ÀßÄê¥Õ¥¡¥¤¥ëÌ¾¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getLogconffile( void )
{
    return config.logconfname;
}
/*------------------------------------------------------------
 * ¥Á¥ã¥Ã¥ÈËâË¡¥Ñ¥¹¥ï¡¼¥É ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
char* getChatMagicPasswd( void )
{
    return config.chatmagicpasswd;
}
/*------------------------------------------------------------
 * ¥Ç¥Ð¥Ã¥°¥Á¥ã¥Ã¥ÈËâË¡¤Ç¤ÎCDKEY¥Á¥§¥Ã¥¯¤ò¤¹¤ë¤«¤É¤¦¤«¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  char*
 ------------------------------------------------------------*/
unsigned getChatMagicCDKeyCheck( void )
{
    return config.chatmagiccdkeycheck;
}

/*------------------------------------------------------------
 * filesearchnum¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getFilesearchnum( void )
{
    return config.filesearchnum;
}
/*------------------------------------------------------------
 * npctemplatenum¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getNpctemplatenum( void )
{
    return config.npctemplatenum;
}
/*------------------------------------------------------------
 * npccreatenum¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getNpccreatenum( void )
{
    return config.npccreatenum;
}

/*------------------------------------------------------------
 * walksendinterval¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getWalksendinterval( void )
{
    return config.walksendinterval;
}
/*------------------------------------------------------------
 * walksendinterval¤ò¥»¥Ã¥È¤¹¤ë¡£
 * °ú¿ô
 *  unsigned int 	interval	»þ´Ö¡Ê¥ß¥êÉÃ¡Ë
 * ÊÖ¤êÃÍ
 *  void
 ------------------------------------------------------------*/
void setWalksendinterval( unsigned int interval )
{
    config.walksendinterval = interval;
}
/*------------------------------------------------------------
 * CAsendinterval¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getCAsendinterval_ms( void )
{
    return config.CAsendinterval_ms;
}
/*------------------------------------------------------------
 * CAsendinterval¤ò¥»¥Ã¥È¤¹¤ë¡£
 * °ú¿ô
 *  unsigned int interval
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
void setCAsendinterval_ms( unsigned int interval_ms )
{
    config.CAsendinterval_ms = interval_ms;
}
/*------------------------------------------------------------
 * CDsendinterval¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getCDsendinterval_ms( void )
{
    return config.CDsendinterval_ms;
}
/*------------------------------------------------------------
 * CDsendinterval¤ò¥»¥Ã¥È¤¹¤ë¡£
 * °ú¿ô
 *  interval		unsigned int
 * ÊÖ¤êÃÍ
 * void
 ------------------------------------------------------------*/
void setCDsendinterval_ms( unsigned int interval_ms )
{
	config.CDsendinterval_ms = interval_ms;
}
/*------------------------------------------------------------
 * Onelooptime¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getOnelooptime_ms( void )
{
    return config.Onelooptime_ms;
}
/*------------------------------------------------------------
 * Onelooptime¤ò¥»¥Ã¥È¤¹¤ë¡£
 * °ú¿ô
 *  interval		unsigned int
 * ÊÖ¤êÃÍ
 * void
 ------------------------------------------------------------*/
void setOnelooptime_ms( unsigned int interval_ms )
{
	config.Onelooptime_ms = interval_ms;
}
/*------------------------------------------------------------
 * Petdeletetime¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getPetdeletetime( void )
{
    return config.Petdeletetime;
}
/*------------------------------------------------------------
 * Petdeletetime¤ò¥»¥Ã¥È¤¹¤ë¡£
 * °ú¿ô
 *  interval		unsigned int
 * ÊÖ¤êÃÍ
 * void
 ------------------------------------------------------------*/
void setPetdeletetime( unsigned int interval )
{
	config.Petdeletetime = interval;
}
/*------------------------------------------------------------
 * Itemdeletetime¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getItemdeletetime( void )
{
    return config.Itemdeletetime;
}
/*------------------------------------------------------------
 * Itemdeletetime¤ò¥»¥Ã¥È¤¹¤ë¡£
 * °ú¿ô
 *  interval		unsigned int
 * ÊÖ¤êÃÍ
 * void
 ------------------------------------------------------------*/
void setItemdeletetime( unsigned int interval )
{
	config.Itemdeletetime = interval;
}

/*------------------------------------------------------------
 * CharSavesendinterval¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getCharSavesendinterval( void )
{
    return config.CharSavesendinterval;
}
/*------------------------------------------------------------
 * CharSavesendinterval¤ò¥»¥Ã¥È¤¹¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
void setCharSavesendinterval( unsigned int interval)
{
	config.CharSavesendinterval = interval;
}

/*------------------------------------------------------------
 * Addressbookoffmsgnum ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getAddressbookoffmsgnum( void )
{
    return config.addressbookoffmsgnum;
}
/*------------------------------------------------------------
 * Protocolreadfrequency ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getProtocolreadfrequency( void )
{
    return config.protocolreadfrequency;
}

/*------------------------------------------------------------
 * Allowerrornum ¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getAllowerrornum( void )
{
    return config.allowerrornum;
}

/*------------------------------------------------------------
 * ¥í¥°¤ò¼èÆÀ¤¹¤ë»þ¹ï¤òÆÀ¤ë¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getLogHour( void )
{
    return config.loghour;
}

/*------------------------------------------------------------
 * ¥Ð¥È¥ëÃæ¤Î¥Ç¥Ð¥Ã¥°¥á¥Ã¥»¡¼¥¸¤ò½Ð¤¹¤«¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int 1¤Ê¤é½Ð¤¹
 ------------------------------------------------------------*/
unsigned int getBattleDebugMsg( void )
{
    return config.battledebugmsg;
}
/*------------------------------------------------------------
 * ¥Ð¥È¥ëÃæ¤Î¥Ç¥Ð¥Ã¥°¥á¥Ã¥»¡¼¥¸¤ò½Ð¤¹¤«¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int 1¤Ê¤é½Ð¤¹
 ------------------------------------------------------------*/
void setBattleDebugMsg( unsigned int num )
{
    config.battledebugmsg = num;
}



/*
 * Config¤Î¥Ç¥Õ¥©¥ë¥ÈÃÍ¤ò·è¤á¤ë´Ø¿ô
 * °ú¿ô
 *  argv0   char*   ¥³¥Þ¥ó¥É¥é¥¤¥ó°ú¿ô¤ÎºÇ½é
 */
void  defaultConfig( char* argv0 )
{
    char* program;                  /* programÌ¾¤òµá¤á¤ë¤Î¤Ë»È¤¦ */

    /* ¥Ç¥Õ¥©¥ë¥ÈÃÍ¤òÆþ¤ì¤ë */

    /*¥×¥í¥°¥é¥àÌ¾*/
    program = rindex(argv0, '/');
    if (program == NULL)
        program = argv0;
    else
        program++;   /* "/"¤Î¼¡¤«¤é¤Ë¤·¤¿¤¤¤Î¤Ç++¤¹¤ë*/
    strcpysafe( config.progname , sizeof( config.progname ) ,program );

    /*ÀßÄê¥Õ¥¡¥¤¥ëÌ¾*/
    strcpysafe( config.configfilename,
                sizeof( config.configfilename ),"setup.cf" );

}

/*
 * ÀßÄê¥Õ¥¡¥¤¥ëÌ¾¤òÆÉ¤ó¤À¸å¤Ç¤Î½èÍý¤ò¹Ô¤¦¡£
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  ¤Ê¤·
 */
void lastConfig( void )
{
    char    entry[256];
    /*  ¥Þ¥Ã¥×¥Ç¥£¥ì¥¯¥È¥ê¤ÎÀßÄê    */
    snprintf(entry, sizeof(entry), "%s/%s", config.topdir, config.mapdir);
    strcpysafe(config.mapdir, sizeof(config.mapdir), entry);

    /*  ¥Þ¥Ã¥×ÀßÄê¥Õ¥¡¥¤¥ëÌ¾¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry),"%s/%s",
             config.topdir,config.maptilefile);
    strcpysafe(config.maptilefile, sizeof(config.maptilefile), entry);

    /*  ¥Ð¥È¥ë¥Þ¥Ã¥×ÀßÄê¥Õ¥¡¥¤¥ëÌ¾¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry),"%s/%s",
             config.topdir,config.battlemapfile);
    strcpysafe(config.battlemapfile, sizeof(config.battlemapfile), entry);

    /*  ¥¢¥¤¥Æ¥àÀßÄê¥Õ¥¡¥¤¥ëÌ¾¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.itemfile);
    strcpysafe(config.itemfile, sizeof(config.itemfile), entry);

    /*  ÌµÅ¨ÀßÄê¥Õ¥¡¥¤¥ëÌ¾¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.invfile);
    strcpysafe(config.invfile, sizeof(config.invfile), entry);

    /*  ½Ð¸½°ÌÃÖÀßÄê¥Õ¥¡¥¤¥ëÌ¾¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.appearfile);
    strcpysafe(config.appearfile, sizeof(config.appearfile), entry);

    /*  ±é½ÐÀßÄê¥Õ¥¡¥¤¥ëÌ¾¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.effectfile);
    strcpysafe(config.effectfile, sizeof(config.effectfile), entry);

    /*  ¥¯¥¤¥ºÀßÄê¥Õ¥¡¥¤¥ëÌ¾¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.quizfile);
    strcpysafe(config.quizfile, sizeof(config.quizfile), entry);

    /*  ¾Î¹æÌ¾¥Õ¥¡¥¤¥ëÌ¾¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.titlenamefile);
    strcpysafe(config.titlenamefile, sizeof(config.titlenamefile), entry);

    /*  lsgen ¥¢¥¦¥È¥×¥Ã¥È¥Õ¥¡¥¤¥ëÌ¾    */
    snprintf(entry,sizeof(entry),"%s/%s", config.topdir,config.lsgenlog);
    strcpysafe(config.lsgenlog, sizeof(config.lsgenlog), entry);

    /*  ¥¹¥È¥¢¥Ç¥£¥ì¥¯¥È¥ê¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry), "%s/%s",config.topdir,config.storedir);
    strcpysafe(config.storedir, sizeof(config.storedir), entry);

    /*  NPCÀßÄê¥È¥Ã¥×¥Ç¥£¥ì¥¯¥È¥ê¤ÎÀßÄê    */
    snprintf(entry,sizeof(entry), "%s/%s",config.topdir,config.npcdir);
    strcpysafe(config.npcdir, sizeof(config.npcdir), entry);

#ifdef _STORECHAR
    /*   */
    snprintf(entry,sizeof(entry), "%s/%s",config.topdir,config.storechar);
    strcpysafe(config.storechar, sizeof(config.storechar), entry);
#endif

}


/*
 * ¥Ý¥¤¥ó¥¿¡¼¤ÈÅ¬ÀÚ¤Ê·¿¾ðÊó¤òÌã¤Ã¤ÆÂåÆþ¤ò¤¹¤ë´Ø¿ô
 * °ú¿ô
 *  to      void*   ÃÍ¤òÂåÆþ¤¹¤ë¥Ý¥¤¥ó¥¿
 *  type    CTYPE   to¤Î·¿¤ò·è¤á¤ë
 *  value   double  to¤ËÂåÆþ¤¹¤ëÃÍ
 * ÊÖ¤êÃÍ
 *  ¤Ê¤·
 */
void substitutePointerFromType( void* to , CTYPE type ,double value)
{
    switch( type  ){
    case CHAR:
        *(char*)to = (char)value;
        break;
    case SHORT:
        *(short*)to = (short)value;
        break;
    case INT:
        *(int*)to = (int)value;
        break;
    case DOUBLE:
        *(double*)to = (double)value;
        break;
    }
}


/*------------------------------------------------------------
 * ÀßÄê¥Õ¥¡¥¤¥ë¤òÆÉ¤à
 * °ú¿ô
 *      filename            ¥Õ¥¡¥¤¥ëÌ¾
 * ÊÖ¤êÃÍ
 *      TRUE(1)     À®¸ù
 *      FALSE(0)    ¼ºÇÔ    -> ¥Õ¥¡¥¤¥ë¤Î¥ª¡¼¥×¥ó¤Ë¼ºÇÔ¤·¤¿
 ------------------------------------------------------------*/
BOOL readconfigfile( char* filename )
{
    FILE* f=NULL;
    char linebuf[256];                  /* °ì¹ÔÆÉ¤ß¹þ¤à¥Ð¥Ã¥Õ¥¡ */
    int linenum=0;                      /* ¹Ô¿ô¤ò¿ô¤¨¤ë */
    char    realopenfilename[256];      /*  ¼ÂºÝ¤Ëopen ¤¹¤ë¥Õ¥¡¥¤¥ëÌ¾*/

    char    hostname[128];

    /*  ¥Û¥¹¥ÈÌ¾¤òµá¤á¤ë    */
    if( gethostname( hostname, sizeof(hostname) ) != -1 ){
        char*   initdot;
        initdot = index( hostname, '.' );
        if( initdot != NULL )
            *initdot = '\0';
        snprintf( realopenfilename, sizeof(realopenfilename),
                  "%s.%s" , filename, hostname);

        /* ¥Õ¥¡¥¤¥ë¤Î¥ª¡¼¥×¥ó */
        f=fopen( realopenfilename, "r" );
        if( f == NULL )
            print( "Can't open %s.  use %s instead\n", realopenfilename,
                   filename );
    }
    if( f == NULL ){
        f=fopen( filename , "r" );          /* ¥Õ¥¡¥¤¥ë¤Î¥ª¡¼¥×¥ó */
        if( f == NULL ){
            print( "Can't open %s\n", filename );
            return FALSE;
        }
    }

    /* °ì¹Ô¤Å¤ÄÆÉ¤ß¹þ¤à */
    while( fgets( linebuf , sizeof( linebuf ), f ) ){
        char firstToken[256];       /*1ÈÖÌÜ¤ÎÊ¸»úÎó*/
        int i;                      /*¥ë¡¼¥×ÊÑ¿ô*/
        int ret;                    /*¥ê¥¿¡¼¥ó¥³¡¼¥É*/

        linenum ++;

        deleteWhiteSpace(linebuf);          /* remove whitespace    */

        if( linebuf[0] == '#' )continue;        /* comment */
        if( linebuf[0] == '\n' )continue;       /* none    */

        chomp( linebuf );                    /* remove tail newline  */

        /* delim "=" ¤ÇºÇ½é(1)¤Î¥È¡¼¥¯¥ó¤òÆÀ¤ë*/
        ret = getStringFromIndexWithDelim( linebuf , "=",  1, firstToken,
                                           sizeof(firstToken) );
        if( ret == FALSE ){
            print( "Find error at %s in line %d. Ignore\n",
                     filename , linenum);
            continue;
        }

        /* readconf ¤Î¥µ¥¤¥º¤Ç¥ë¡¼¥× */
        for( i = 0 ; i < arraysizeof( readconf ) ; i ++ ){
            if( strcmp( readconf[i].name ,firstToken ) == 0 ){
                /* match */
                char secondToken[256];      /*2ÈÖÌÜ¤ÎÊ¸»úÎó*/
                /* delim "=" ¤Ç2ÈÖÌÜ¤Î¥È¡¼¥¯¥ó¤òÆÀ¤ë*/
                ret = getStringFromIndexWithDelim( linebuf , "=" , 2
                                                   , secondToken ,
                                                   sizeof(secondToken) );

                /* NULLÊ¸»ú¤«¤É¤¦¤«¤òÄ´¤Ù¤ë */
                if( ret == FALSE ){
                    print( "Find error at %s in line %d. Ignore",
                           filename , linenum);
                    break;
                }


                /*NULL¤À¤Ã¤¿¤éÂåÆþ¤·¤Ê¤¤*/
                if( readconf[i].charvalue != NULL )
                    strcpysafe( readconf[i].charvalue
                                ,readconf[i].charsize, secondToken);

                /*NULL¤À¤Ã¤¿¤éÂåÆþ¤·¤Ê¤¤*/
                if( readconf[i].value != NULL ) {
                    if( strcmp( "ON" ,secondToken ) == 0 ) {
                        /*ON¤À¤Ã¤¿¤é1¤òÆþ¤ì¤ë*/
                        substitutePointerFromType( readconf[i].value,
                                                   readconf[i].valuetype,
                                                   1.0);

                    }else if( strcmp( "OFF" ,secondToken ) == 0 ) {
                        /*OFF¤À¤Ã¤¿¤é1¤òÆþ¤ì¤ë*/
                        substitutePointerFromType( readconf[i].value,
                                                   readconf[i].valuetype,
                                                   1.0);
                    }else {
                        strtolchecknum(secondToken,
                                       (int*)readconf[i].value,
                                       10, readconf[i].valuetype);
					}
				}
                break;
            }
        }
    }
    fclose( f );
    lastConfig();
    return TRUE;
}
//ttom add this becaus the second had this function 
/*------------------------------------------------------------
 * ¥¨¥ó¥³¡¼¥É¥­¡¼¤òÀßÄê¤¹¤ë
 * °ú¿ô
 *  ¤Ê¤·
 * ÊÖ¤êÃÍ
 *  unsigned int ¥­¡¼¤òÊÖ¤¹
------------------------------------------------------------*/
unsigned int setEncodeKey( void )
{
   JENCODE_KEY = config.encodekey;
   return JENCODE_KEY;
}
/*------------------------------------------------------------
* ¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¡¼¤Ë½ñ¤­¹þ¤à¥Ð¥Ã¥Õ¥¡¤Î¥µ¥¤¥º¤òÀßÄê¤¹¤ë
* °ú¿ô
*  ¤Ê¤·
* ÊÖ¤êÃÍ
*  unsigned int ¥­¡¼¤òÊÖ¤¹
------------------------------------------------------------*/
unsigned int setAcWBSize( void )
{
    AC_WBSIZE = config.acwbsize;
    return AC_WBSIZE;
}
unsigned int getAcwriteSize( void )
{
    return config.acwritesize;
}
unsigned int getErrUserDownFlg( void )
{
    return config.ErrUserDownFlg;
}
    
    
//tom end


#ifdef _M_SERVER
char* getmservername(void)
{
	return config.msname;
}
void setmservername( char *msname)
{
	sprintf( config.msname, "%s", msname);
}
unsigned int getmserverport(void)
{
	return config.msport;
}
void setmserverport( int port)
{
	config.msport = port;
}
#endif

#ifdef _NPCSERVER_NEW
char *getnpcserveraddr(void)
{
	return config.nsaddress;
}

unsigned int getnpcserverport(void)
{
	return config.nsport;
}
void NS_setAddressAndPort( char *address, int nport)
{
	sprintf( config.nsaddress, "%s", address);
	config.nsport = nport;
}
#endif

#ifdef _MUSEUM
int   getMuseum( void )
{
    return config.museum;
}
#endif

#ifdef _DEL_DROP_GOLD
unsigned int getGolddeletetime( void )
{
    if( config.Golddeletetime > 0 )
		return config.Golddeletetime;
	else
		return config.Itemdeletetime;
}
void setIGolddeletetime( unsigned int interval )
{
	config.Golddeletetime = interval;
}
#endif

#ifdef _ANGEL_SUMMON

extern int mission_num;

BOOL LoadMissionList( )
{
	FILE* fp;
	int i = 0;

	mission_num = 0;

	fp = fopen(".//data//mission.txt", "r");
	if (fp == NULL)
	{
		print("Mission File Open Error\n");
		return FALSE;
	}
	
	memset( missionlist, 0, sizeof(missionlist));

	while(1){
		char	line[1024], level[64];
		char	token[1024];
		int		mindex;
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		print("\n %s ", line);
		chop(line);
		// ¥H#¬°µù¸Ñ*******
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		//*************************************

		//®æ¦¡ #¥ô°È½s¸¹,¥²­nµ¥¯Å,¥ô°È»¡©ú,¼ú«~ID,­­¨î®É¶¡(¤p®É)
		
		getStringFromIndexWithDelim(line, ",", 1, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		mindex = atoi( token);

		if( mindex <= 0 || mindex >= MAXMISSION) 
			break;

		missionlist[mindex].id = mindex;

		getStringFromIndexWithDelim(line, ",", 2, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missionlist[mindex].level = atoi( token);

		getStringFromIndexWithDelim(line, ",", 3, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missionlist[mindex].eventflag, token);

		getStringFromIndexWithDelim(line, ",", 4, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missionlist[mindex].detail, token);

		//getStringFromIndexWithDelim(line, ",", 4, token, sizeof(token));
		//if (strcmp(token, "") == 0)	break;
		//strcpy( missionlist[mindex].bonus, token);

		getStringFromIndexWithDelim(line, ",", 5, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missionlist[mindex].limittime = atoi( token);

		/*print("\nMISSION[%d] lv:%d ef:%s detail:%s limit:%d ", mindex,
			missionlist[mindex].level, missionlist[mindex].eventflag,
			missionlist[mindex].detail, missionlist[mindex].limittime );
		*/
		mission_num++;
		//if (mission_num > MAXMISSION)	break;
	}
	fclose(fp);
	return TRUE;
}



BOOL LoadMissionCleanList( )
{
	// ®æ¦¡... ¨ÏªÌ,«iªÌ,¥ô°È,¼ú½à
	FILE* fp;
	int	listindex =0;
	int i = 0;

	memset( missiontable, 0, sizeof(missiontable));
	fp = fopen(".//data//missionclean.txt", "r");
	if (fp == NULL)
	{
		print("MissionClean File Open Error\n");
		return FALSE;
	}

	while(1){
		char	line[1024], angelinfo[128], heroinfo[128];
		char	token[1024];
		
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		print("\n %s ", line);
		chop(line);
		// ¥H#¬°µù¸Ñ*******
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		
		getStringFromIndexWithDelim(line, ",", 1, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missiontable[listindex].angelinfo, token);

		getStringFromIndexWithDelim(line, ",", 2, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missiontable[listindex].heroinfo, token);

		getStringFromIndexWithDelim(line, ",", 3, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missiontable[listindex].mission = atoi( token);
		
		getStringFromIndexWithDelim(line, ",", 4, token, sizeof(token));
		//if (strcmp(token, "") == 0)	break;
		missiontable[listindex].flag = atoi( token);

		getStringFromIndexWithDelim(line, ",", 5, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missiontable[listindex].time = atoi( token);

		print("\nMISSIONCLEAN[%d] %s %s %d %d %d", listindex,
			missiontable[listindex].angelinfo,
			missiontable[listindex].heroinfo,
			missiontable[listindex].mission,
			missiontable[listindex].flag,
			missiontable[listindex].time );

		listindex++;
		if ( listindex >= MAXMISSIONTABLE)	break;
	}
	fclose(fp);
	return TRUE;
}


#endif

#ifdef _JOBDAILY
extern  DailyFileType dailyfile[MAXDAILYLIST];
BOOL LoadJobdailyfile(void)
{
	char	line[20000];
	char	token[16384];
	int		listindex =0;
	int     i;
	FILE* fp;

	fp = fopen(".//data//jobdaily.txt", "r");
	if (fp == NULL)
	{
		print("Jobdaily File Open Error\n");
		return FALSE;
	}

	memset( dailyfile, 0, sizeof(dailyfile));

	while(1){		
		line[0]='\0';	
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		//print("\n %s ", line);
		chop(line);

		// #¬°µù¸Ñ
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		
		getStringFromIndexWithDelim(line, "|", 1, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( dailyfile[listindex].jobid, token);

		getStringFromIndexWithDelim(line, "|", 2, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( dailyfile[listindex].rule, token);

		getStringFromIndexWithDelim(line, "|", 3, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		if(strlen(token)>96){
			print("¥ô°È»¡©ú¹Lªø:%d",strlen(token));
			return FALSE;
		}
		strcpy( dailyfile[listindex].explain, token);
		
		getStringFromIndexWithDelim(line, "|", 4, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( dailyfile[listindex].state, token);

		/*print("\ndailyfile[%d] %s %s %s %s", listindex,
			dailyfile[listindex].jobid,
			dailyfile[listindex].rule,
			dailyfile[listindex].explain,
			dailyfile[listindex].state);	
		*/
		listindex++;
		if ( listindex >= MAXDAILYLIST)	break;
	}
	fclose(fp);
	return TRUE;
}
#endif

#ifdef _RACEMAN
//extern int petflgtable[640];
//extern struct ASKTABLE asktable[400];
extern int asktotal;
BOOL LoadRacepetfile(void)
{
	char line[1000];
	char token[64];
	int  i;
	FILE* fp;

	asktotal = 0;
	memset(asktable,0,sizeof(asktable));

	fp = fopen(".//data//raceman.txt", "r");
	if (fp == NULL)
	{
		print("Racepet File Open Error\n");
		return FALSE;
	}
	
	while(1){		
		line[0]='\0';	
		if (fgets(line, sizeof(line), fp) == NULL)	break;		
		chop(line);

		// #¬°µù¸Ñ
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		getStringFromIndexWithDelim(line, "|", 1 , token, sizeof(token));
		if( atoi(token) <= 0 )	return FALSE;
		asktable[asktotal].no = atoi(token);

		getStringFromIndexWithDelim(line, "|", 2 , token, sizeof(token));
		if( strlen(token) == 0 ) return FALSE;
		strcpy( asktable[asktotal].petname , token );

		getStringFromIndexWithDelim(line, "|", 3 , token, sizeof(token));
		if( atoi(token) <= 0 )	return FALSE;
		asktable[asktotal].bbi = atoi(token);

		getStringFromIndexWithDelim(line, "|", 4 , token, sizeof(token));
		if( atoi(token) <= 0 )	return FALSE;
		asktable[asktotal].lowlv = atoi(token);

		getStringFromIndexWithDelim(line, "|", 5 , token, sizeof(token));
		if( atoi(token) < 0 || atoi(token) > 140 )	return FALSE;
		asktable[asktotal].highlv = atoi(token);
		asktotal++;
	}
	fclose(fp);
	return TRUE;

	/*
	for( k=0,j=0;j<ENEMYTEMP_enemynum;j++ ) {
		if( ENEMYTEMP_getInt( j, E_T_PETFLG ) == 1 ){
			petflgtable[k] = ENEMYTEMP_getInt(j,E_T_IMGNUMBER) ;
			k++;
			if(k>=640) {
				print( "Valid petflag Num is %d...", k );
				break;
			}
		}
		//if( CHAR_getInt( petindex, CHAR_PETID) == ENEMYTEMP_getInt( j, E_T_TEMPNO ) ) {//ENEMY_getInt( j, ENEMY_TEMPNO)
	}
	*/
}

#endif