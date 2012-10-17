
#ifndef __ITEM_H__
#define __ITEM_H__

#include "char.h"

#define NULLITEM    "0"

typedef enum
{
    ITEM_FIST =0,
    ITEM_AXE,
    ITEM_CLUB,
    ITEM_SPEAR,
    ITEM_BOW,
    ITEM_SHIELD,
    ITEM_HELM,
    ITEM_ARMOUR,

	ITEM_BRACELET =8,
	ITEM_MUSIC,
	ITEM_NECKLACE,
	ITEM_RING,
	ITEM_BELT,
	ITEM_EARRING,
	ITEM_NOSERING,
	ITEM_AMULET,
    /* ****** */
    ITEM_OTHER =16,
    ITEM_BOOMERANG,		// °j±Û¼Ð
    ITEM_BOUNDTHROW,	// §ëÂY©òÀY
    ITEM_BREAKTHROW,	// §ëÂY¥Û
    ITEM_DISH =20,
#ifdef _ITEM_INSLAY
	ITEM_METAL,
	ITEM_JEWEL,
#endif
#ifdef _ITEM_CHECKWARES
	ITEM_WARES,			//³fª«
#endif

#ifdef _ITEM_EQUITSPACE
	ITEM_WBELT,			//¸y±a
	ITEM_WSHIELD,		//¬Þ
	ITEM_WSHOES,		//¾c¤l
#endif
#ifdef _EQUIT_NEWGLOVE 
	ITEM_WGLOVE,		//¤â®M
#endif

#ifdef _ALCHEMIST
	ITEM_ALCHEMIST =30,
#endif

#ifdef _ANGEL_SUMMON
	//ITEM_ANGELTOKEN,
	//ITEM_HEROTOKEN,
#endif

    ITEM_CATEGORYNUM,
    
}ITEM_CATEGORY;

typedef enum
{
	ITEM_FIELD_ALL,
	ITEM_FIELD_BATTLE,
	ITEM_FIELD_MAP,
}ITEM_FIELDTYPE;

typedef enum
{
	ITEM_TARGET_MYSELF,
	ITEM_TARGET_OTHER,
	ITEM_TARGET_ALLMYSIDE,
	ITEM_TARGET_ALLOTHERSIDE,
	ITEM_TARGET_ALL,
}ITEM_TARGETTYPE;

typedef enum
{
    ITEM_ID,
    ITEM_BASEIMAGENUMBER,
    ITEM_COST,
    ITEM_TYPE,
	ITEM_ABLEUSEFIELD,
	ITEM_TARGET,
    ITEM_LEVEL,                     /*  LEVEL  */
#ifdef _ITEM_MAXUSERNUM
	ITEM_DAMAGEBREAK,				//ª««~¨Ï¥Î¦¸¼Æ
#endif

#ifdef _ITEMSET4_TXT
	ITEM_USEPILENUMS,				//ª««~°ïÅ|¦¸¼Æ
	ITEM_CANBEPILE,					//¬O§_¥i°ïÅ|

	ITEM_NEEDSTR,
	ITEM_NEEDDEX,
	ITEM_NEEDTRANS,
	ITEM_NEEDPROFESSION,
#endif

#ifdef _TAKE_ITEMDAMAGE
	ITEM_DAMAGECRUSHE,
	ITEM_MAXDAMAGECRUSHE,
#endif

#ifdef _ADD_DEAMGEDEFC
	ITEM_OTHERDAMAGE,
	ITEM_OTHERDEFC,
#endif

#ifdef _SUIT_ITEM
	ITEM_SUITCODE,
#endif

    ITEM_ATTACKNUM_MIN,             /*  ºÇÄã¹¶·â²ó¿ô  */
    ITEM_ATTACKNUM_MAX,             /*  ºÇ¹â¹¶·â²ó¿ô  */
    ITEM_MODIFYATTACK,              /*  ¹¶·âÎÏÊÑ²½ÎÌ  */
    ITEM_MODIFYDEFENCE,             /*  ËÉ¸æÎÏÊÑ²½ÎÌ  */
    ITEM_MODIFYQUICK,               /*  QUICKÊÑ²½ÎÌ  */

    ITEM_MODIFYHP,                  /*  HPÊÑ²½ÎÌ    */
    ITEM_MODIFYMP,                  /*  MPÊÑ²½ÎÌ    */
    ITEM_MODIFYLUCK,                /*  LUCKÊÑ²½ÎÌ    */
    ITEM_MODIFYCHARM,               /*  CHARMÊÑ²½ÎÌ    */
    ITEM_MODIFYAVOID,               /*  ²óÈòÎ¨½¤Àµ    */
	ITEM_MODIFYATTRIB,				/*  Â°À­½¤Àµ */
	ITEM_MODIFYATTRIBVALUE,			/*  Â°À­½¤ÀµÃÍ */
	ITEM_MAGICID,					/*  ¼ö½ÑÈÖ¹æ */
	ITEM_MAGICPROB,					/*  ¼ö½ÑÈ¯Æ°Î¨ */
	ITEM_MAGICUSEMP,				/*  ¾ÃÈñMP */

#ifdef _ITEMSET5_TXT
	ITEM_MODIFYARRANGE,
	ITEM_MODIFYSEQUENCE,

	ITEM_ATTACHPILE,
	ITEM_HITRIGHT,	//ÃB¥~©R¤¤
#endif
#ifdef _ITEMSET6_TXT
	ITEM_NEGLECTGUARD,
//	ITEM_BEMERGE,
#endif
    /*  ¥¹¥Æ¡¼¥¿¥¹½¤ÀµÃÍ¡£*/
    ITEM_POISON,					/* ÆÇÊâ¤¯¤¿¤Ó¤Ë¥À¥á¡¼¥¸          */
    ITEM_PARALYSIS,          		/* ¤·¤Ó¤ì¡¢1Éô¤Î¹ÔÆ°¤¬¤Ç¤­¤Ê¤¤¡£ */
    ITEM_SLEEP,              		/* Ì²¤ê¡£¹ÔÆ°¤Ç¤­¤Ê¤¤            */
    ITEM_STONE,              		/* ÀÐ¡£¹ÔÆ°¤Ç¤­¤Ê¤¤              */
    ITEM_DRUNK,              		/* ¿ì¤¦¡£Ì¿ÃæÎ¨¤¬²¼¤¬¤ë     */
    ITEM_CONFUSION,          		/* º®Íð¡£¹¶·âÌÜÉ¸¤ò¸í¤ë     */

	ITEM_CRITICAL,					/* ¥¯¥ê¥Æ¥£¥«¥ëÎ¨½¤Àµ */

	ITEM_USEACTION,					/* »È¤Ã¤¿»þ¤Î¥¢¥¯¥·¥ç¥ó */
    ITEM_DROPATLOGOUT,              /* ¥í¥°¥¢¥¦¥È¤¹¤ë»þ¤ËÍî¤¹¤«¤É¤¦¤«  */
    ITEM_VANISHATDROP,              /* Íî¤·¤¿»þ¤Ë¾Ã¤¨¤ë¤«¤É¤¦¤« */
    ITEM_ISOVERED,                  /*  ¾å¤Ë¾è¤Ã¤«¤é¤é¤ì¤ë¤«¤É¤¦¤«¡£*/
	ITEM_CANPETMAIL,				/* ¥Ú¥Ã¥È¥á¡¼¥ë¤ÇÁ÷¤ì¤ë¤« */
	ITEM_CANMERGEFROM,				/* ¹çÀ®¸µ¤Ë¤Ê¤ì¤ë¤« */
	ITEM_CANMERGETO,				/* ¹çÀ®Àè¤Ë¤Ê¤ì¤ë¤« */

    ITEM_INGVALUE0,                 /* À®Ê¬(5¸ÄÊ¬) */
    ITEM_INGVALUE1,
    ITEM_INGVALUE2,
    ITEM_INGVALUE3,
    ITEM_INGVALUE4,
    
	ITEM_PUTTIME,					/*  ¥¢¥¤¥Æ¥à¤¬ÃÖ¤«¤ì¤¿»þ´Ö */
    ITEM_LEAKLEVEL,                 /*  ÈëÌ©¤¬¤É¤ì¤À¤±¤Ð¤ì¤¿¤«  */
	ITEM_MERGEFLG,					/*  ¹çÀ®¤µ¤ì¤¿¥¢¥¤¥Æ¥à¤«¤É¤¦¤« */
	ITEM_CRUSHLEVEL,				/*  ²õ¤ìÅÙ¹ç¤¤ 0¡Á2 £°¤Ï²õ¤ì¤Æ¤Ê¤¤ 2¤ÏÁ´²õ */

    ITEM_VAR1,              	/*  ÈÆÍÑºî¶ÈÎÎ°è   */
    ITEM_VAR2,              	/*  ÈÆÍÑºî¶ÈÎÎ°è   */
    ITEM_VAR3,              	/*  ÈÆÍÑºî¶ÈÎÎ°è   */
    ITEM_VAR4,              	/*  ÈÆÍÑºî¶ÈÎÎ°è   */

	ITEM_DATAINTNUM,

}ITEM_DATAINT;

typedef enum
{
    ITEM_NAME,                      /*  Ì¾Á° ¡ÊËÜÅö¤ÎÌ¾Á°¡Ë   */
    ITEM_SECRETNAME,                /*  Ì¾Á°¡ÊÊÑ¹¹¤µ¤ì¤ë²ÄÇ½À­Í­¤ê¡Ë  */
    ITEM_EFFECTSTRING,              /*  ¸ú²ÌÊ¸»úÎó  */
    ITEM_ARGUMENT,                  /*  ¥¢¥¤¥Æ¥à¤Î°ú¿ô  */
#ifdef _ITEM_INSLAY
	ITEM_TYPECODE,
	ITEM_INLAYCODE,
#endif
	ITEM_CDKEY,						/*  ¥¢¥¤¥Æ¥à¤ÎÌ¾Á°¤òºÇ½é¤ËÊÑ¹¹¤·¤¿¿Í¤Î£Ã£Ä£Ë£Å£Ù */
#ifdef _ITEM_FORUSERNAMES
	ITEM_FORUSERNAME,
	ITEM_FORUSERCDKEY,
#endif
// CoolFish: 2001/10/11
#ifdef _UNIQUE_P_I
    ITEM_UNIQUECODE,		  /* ª««~½s½X */
#endif

    ITEM_INGNAME0,                  /*  À®Ê¬¤ÎÌ¾Á°(5¸ÄÊ¬) */
    ITEM_INGNAME1,
    ITEM_INGNAME2,
    ITEM_INGNAME3,
    ITEM_INGNAME4,


    ITEM_INITFUNC,                  /* °ú¿ô
                                     * ITEM_Item*
                                     * ÊÖ¤êÃÍ BOOL
                                     * ÊÖ¤êÃÍ¤Î°ÕÌ£¤Ï CHAR_INITFUNC
                                     * ¤ÈÆ±¤¸  */
    ITEM_FIRSTFUNCTION = ITEM_INITFUNC,
    ITEM_PREOVERFUNC,               /* CHAR_PREOVERFUNC ¤ò»²¾È */
    ITEM_POSTOVERFUNC,              /* CHAR_POSTOVERFUNC ¤ò»²¾È*/
    ITEM_WATCHFUNC,                 /* CHAR_WATCHFUNC ¤ò»²¾È */
    ITEM_USEFUNC,                   /* °ú¿ô¤Ï¡¢
                                     * int charaindex ¥­¥ã¥é¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     * int charitemindex ¼«Ê¬¤Î
                                     *              ¥¢¥¤¥Æ¥àÇÛÎó¤Î²¿ÈÖÌÜ
                                     *              ¤ò»È¤Ã¤¿¤«
                                     */
    ITEM_ATTACHFUNC,                /* °ú¿ô¤Ï¡¢
                                     * int charaindex ¥­¥ã¥é¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     * int itemindex  ¥¢¥¤¥Æ¥à¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     *      ¥­¥ã¥é¥¯¥¿¤Î»ý¤Ã¤Æ¤¤¤ë¥¢¥¤¥Æ¥à
                                     *      ¤Î¥¢¥¤¥Æ¥àÍó¤Ç¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     *      ¤Ç¤Ï¤Ê¤¤»ö¤ËÃí°Õ¡£
                                     */
    ITEM_DETACHFUNC,                /* °ú¿ô¤Ï¡¢
                                     * int charaindex ¥­¥ã¥é¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     * int itemindex  ¥¢¥¤¥Æ¥à¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     *      ¥­¥ã¥é¥¯¥¿¤Î»ý¤Ã¤Æ¤¤¤ë¥¢¥¤¥Æ¥à
                                     *      ¤Î¥¢¥¤¥Æ¥àÍó¤Ç¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     *      ¤Ç¤Ï¤Ê¤¤»ö¤ËÃí°Õ¡£
                                     */
    ITEM_DROPFUNC, 		            /* Íî¤È¤·¤¿¤È¤­
                                     * °ú¿ô¤Ï
                                     *  int charaindex Íî¤È¤·¤¿¥­¥ã¥é
                                     *  int itemindex ¥¢¥¤¥Æ¥à¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     */
    ITEM_PICKUPFUNC,              /* ¥¢¥¤¥Æ¥à¤ò½¦¤Ã¤¿»þ
                                     * °ú¿ô¤Ï
                                     *  int charaindex  ½¦¤Ã¤¿¥­¥ã¥éindex
                                     *  int itemindex ¥¢¥¤¥Æ¥à¥¤¥ó¥Ç¥Ã¥¯¥¹
                                     */
#ifdef _Item_ReLifeAct
	ITEM_DIERELIFEFUNC,					/*ANDY_ADD
										´_¬¡¹D¨ã	
									 */
#endif

#ifdef _CONTRACT
	ITEM_CONTRACTTIME,
	ITEM_CONTRACTARG,
#endif

    ITEM_LASTFUNCTION,

    ITEM_DATACHARNUM = ITEM_LASTFUNCTION,

#ifdef _ANGEL_SUMMON
	ITEM_ANGELMISSION = ITEM_INGNAME0,
	ITEM_ANGELINFO = ITEM_INGNAME1,
	ITEM_HEROINFO = ITEM_INGNAME2,
#endif

}ITEM_DATACHAR;

typedef enum
{
    ITEM_WORKOBJINDEX,
    ITEM_WORKCHARAINDEX,
#ifdef _MARKET_TRADE
	ITEM_WORKTRADEINDEX,
	ITEM_WORKTRADETYPE,
	ITEM_WORKTRADESELLINDEX,
#endif
#ifdef _ITEM_ORNAMENTS
	ITEM_CANPICKUP,
#endif
#ifdef _ITEM_TIME_LIMIT
	ITEM_WORKTIMELIMIT,
#endif
    ITEM_WORKDATAINTNUM,
}ITEM_WORKDATAINT;



typedef struct tagItem
{
    int         data[ITEM_DATAINTNUM];
    STRING64    string[ITEM_DATACHARNUM];
    int         workint[ITEM_WORKDATAINTNUM];

    void*       functable[ITEM_LASTFUNCTION-ITEM_FIRSTFUNCTION];
}ITEM_Item;

typedef struct tagITEM_table
{
    int         use; 
    ITEM_Item   itm;
    int         randomdata[ITEM_DATAINTNUM];
}ITEM_table;


typedef struct tagITEM_exists
{
    BOOL        use;
    ITEM_Item   itm;
}ITEM_exists;

#ifdef _CONTRACT
#define MAX_CONTRACTTABLE	10
typedef struct tagITEM_contract
{
    int         used;
    char		detail[2048];
    int			argnum;
}ITEM_contractTable;
#endif


#define		ITEM_CHECKINDEX(index)		\
	_ITEM_CHECKINDEX( __FILE__, __LINE__, index)
INLINE BOOL _ITEM_CHECKINDEX( char *file, int line, int index);


BOOL ITEM_initExistItemsArray( int num );
BOOL ITEM_endExistItemsArray( void );
#define		ITEM_initExistItemsOne( itm) \
	_ITEM_initExistItemsOne( __FILE__, __LINE__, itm)
int _ITEM_initExistItemsOne( char *file, int line, ITEM_Item* itm );

#define		ITEM_endExistItemsOne( index ) \
			_ITEM_endExistItemsOne( index, __FILE__, __LINE__)

void _ITEM_endExistItemsOne( int index , char *file, int line);

#define ITEM_getInt( Index, element) _ITEM_getInt( __FILE__, __LINE__, Index, element )
INLINE int _ITEM_getInt( char *file, int line, int index ,ITEM_DATAINT element);


#define ITEM_setInt( Index, element, data) _ITEM_setInt( __FILE__, __LINE__, Index, element, data)
INLINE int _ITEM_setInt( char *file, int line, int index ,ITEM_DATAINT element, int data);


INLINE char* ITEM_getChar( int index ,ITEM_DATACHAR element );
INLINE BOOL ITEM_setChar( int index ,ITEM_DATACHAR element , char* new);

INLINE int ITEM_getWorkInt( int index ,ITEM_WORKDATAINT element);
INLINE int ITEM_setWorkInt( int index ,ITEM_WORKDATAINT element, int data);
INLINE int ITEM_getITEM_itemnum( void );
INLINE int ITEM_getITEM_UseItemnum( void );
INLINE BOOL ITEM_getITEM_use( int index );
void ITEM_constructFunctable( int itemindex );
void* ITEM_getFunctionPointer( int itemindex, int functype );
INLINE ITEM_Item *ITEM_getItemPointer( int index );
int ITEM_getItemMaxIdNum( void);


char* ITEM_makeStringFromItemData( ITEM_Item* one, int mode );
char* ITEM_makeStringFromItemIndex( int index, int mode );

BOOL ITEM_makeExistItemsFromStringToArg( char* src , ITEM_Item* item, int mode );
void ITEM_getDefaultItemSetting( ITEM_Item* itm);


INLINE BOOL ITEM_CHECKITEMTABLE( int number );
BOOL    ITEM_readItemConfFile( char* filename );


CHAR_EquipPlace ITEM_getEquipPlace( int charaindex, int itmid );


char*  ITEM_makeItemStatusString( int haveitemindex, int itemindex );
char*   ITEM_makeItemFalseString( void );
char*   ITEM_makeItemFalseStringWithNum( int haveitemindex );


BOOL ITEM_makeItem( ITEM_Item* itm, int number );
int ITEM_makeItemAndRegist( int number );


void ITEM_equipEffect( int index );

void Other_DefcharWorkInt( int index);

char* ITEM_getAppropriateName(int itemindex);
char* ITEM_getEffectString( int itemindex );


int ITEM_getcostFromITEMtabl( int itemid );

#define ITEM_getNameFromNumber( id) _ITEM_getNameFromNumber( __FILE__, __LINE__, id)
INLINE char* _ITEM_getNameFromNumber( char *file, int line, int itemid );


int ITEM_getlevelFromITEMtabl( int itemid );
int ITEM_getgraNoFromITEMtabl( int itemid );
char *ITEM_getItemInfoFromNumber( int itemid );

int ITEM_getdropatlogoutFromITEMtabl( int itemid );
int ITEM_getvanishatdropFromITEMtabl( int itemid );
int ITEM_getcanpetmailFromITEMtabl( int itemid );
int ITEM_getmergeItemFromFromITEMtabl( int itemid );

#ifdef _ITEM_CHECKWARES
BOOL CHAR_CheckInItemForWares( int charaindex, int flg);
#endif

BOOL ITEM_canuseMagic( int itemindex);
// Nuke +1 08/23 : For checking the validity of item target
int ITEM_isTargetValid( int charaindex, int itemindex, int toindex);


#ifdef _IMPOROVE_ITEMTABLE
BOOL ITEMTBL_CHECKINDEX( int ItemID);
int ITEM_getSIndexFromTransList( int ItemID);
int ITEM_getMaxitemtblsFromTransList( void);
int ITEM_getTotalitemtblsFromTransList( void);
#endif

int ITEMTBL_getInt( int ItemID, ITEM_DATAINT datatype);
char *ITEMTBL_getChar( int ItemID, ITEM_DATACHAR datatype);

int ITEM_getItemDamageCrusheED( int itemindex);
void ITEM_RsetEquit( int charaindex);//¦Û°Ê¨ø°£¸Ë³Æ¦ì¸m¿ù»~¤§ª««~
void ITEM_reChangeItemToPile( int itemindex);
void ITEM_reChangeItemName( int itemindex);


#ifdef _SIMPLIFY_ITEMSTRING
void ITEM_getDefaultItemData( int itemID, ITEM_Item* itm);
#endif

#ifdef _CONTRACT
BOOL ITEM_initContractTable( );
#endif

#endif
