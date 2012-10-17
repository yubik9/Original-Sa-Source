#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "pet_skill.h"
#include "readmap.h"
#include "battle.h"
#include "log.h"
#ifdef _CHECK_ENEMY_PET
#include "enemy.h"
#endif
/* ¥á¥Ã¥»¡¼¥¸¤Îenum */
enum {
	NPC_PETSHOP_MSG_OVER,
	NPC_PETSHOP_MSG_MAIN,
	NPC_PETSHOP_MSG_REALY,
	NPC_PETSHOP_MSG_THANKS,
	NPC_PETSHOP_MSG_COST,
	NPC_PETSHOP_MSG_POOLTHANKS,
	NPC_PETSHOP_MSG_POOLFULL,
	NPC_PETSHOP_MSG_GETFULL,
};
typedef struct {
	char	option[32];
	char	defaultmsg[128];
}NPC_PETSHOP_MSG;

NPC_PETSHOP_MSG		shopmsg[] = {
	{ "over_msg",		"¿ú¥Î§¹Åo!¨S°ÝÃD¶Ü?"},
	{ "main_msg",		"Åwªï"},
	{ "realy_msg",		"¯uªº¥i¥H¶Ü?"},
	{ "thanks_msg",		"ÁÂÁÂ!"},
	{ "cost_msg",		"¿ú¤£°÷³á!"},
	{ "pooltanks_msg",	"ÁÂÁÂ!"},
	{ "poolfull_msg",	"¶W¹LªºµLªk³B²z³á"},
	{ "getfull_msg",	"§A¤£¬O¦³«Ü¦hÃdª«¶Ü"}
	
};

#define PETCOST	10

void NPC_PetDate(int meindex,int toindex,int select,char *token);
void NPC_PetDate2(int meindex,int talker,int select,char *token);
void NPC_PetDate3(int meindex,int talker,int select,char *token);
void NPC_PetDel2( int meindex, int talker, int select, char *token);
void NPC_PetDel3( int meindex, int talker, int select, char *token);
void NPC_getDrawPet( int meindex, int toindex, char *token, int *buttontype, int select);

static void NPC_PetShop_selectWindow( int meindex, int toindex, int num,int select);
int NPC_GetCost(int meindex,int talker,int petindex);
void NPC_PetDel(int meindex, int toindex,int select,char* token);
void NPC_MaxGoldOver(int meindex,int toindex,int select,char *token);

#ifdef _NPC_DEPOTPET
void NPC_PoolPetShop_DepotPet_Menu( int meindex, int talkerindex);
void NPC_DepotPet_List( int meindex, int toindex, char *token, int *buttontype, int select);
void NPC_DepotPet_Add( int meindex, int talker, int select, char *token);
void NPC_DepotPet_Get( int meindex, int talker, int select, char *token);
void NPC_DepotPet_AskGet(int meindex,int talker,int select,char *token);
//void NPC_DepotPet_Pet_printWindow( int meindex, int talkerindex);
//void NPC_DepotPet_Depot_printWindow( int meindex, int talkerindex);
//BOOL NPC_DepotPet_InsertPet( int meindex, int talkerindex, int num);
//BOOL NPC_DepotPet_getPet( int meindex, int talkerindex, int num);
#endif


// ÍÂ¤±Æþ¤ì¶â³Û·×»»¼°
#define		NPC_GETPOOLCOST( talker)		( 50 + CHAR_getInt( talker, CHAR_LV)*4)

// ÍÂ¤±Æþ¤ì¶â¥Ç¥Õ¥©¥ë¥È
#define		NPC_PETSHOP_POOLCOST_DEFAULT	200

/*--¥ï¡¼¥¯ÎÎ°è¤Î³ÎÊÝ--*/
enum {
	CHAR_WORK_NOMALRATE	= CHAR_NPCWORKINT1,
	CHAR_WORK_SPECIALRATE	= CHAR_NPCWORKINT2,
	CHAR_WORK_POOLCOST	= CHAR_NPCWORKINT3,
	CHAR_WORK_POOLFLG	= CHAR_NPCWORKINT4,
};


/*********************************
* ½é´ü½èÍý
*********************************/
BOOL NPC_PetShopInit( int meindex )
{

	char npcarg[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buf[256];
	double rate;
	int intrate;
//	int cost;
	int tmp;

	/*--¥¿¥¤¥×ÀßÄê--*/
   	CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPESTONESHOP );


	if( NPC_Util_GetArgStr( meindex, npcarg, sizeof( npcarg)) == NULL){
		print("GetArgStrErr");
		return FALSE;
	}

	/*--¥ì¡¼¥È¤ò»Ï¤á¤Ëµ­²±¤µ¤»¤Æ¤ª¤¯*/
	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "nomal_rate", buf, sizeof( buf)) != NULL){
		rate = atof( buf);		
		intrate = ( int)( rate *1000);		
   		CHAR_setWorkInt( meindex , CHAR_WORK_NOMALRATE ,intrate );
	}else{
		CHAR_setWorkInt( meindex , CHAR_WORK_NOMALRATE ,1000 );
	}

		
	/*--¥¹¥Ú¥·¥ã¥ë¥ì¡¼¥È¤âµ­²±¤µ¤»¤Æ¤ª¤¯--*/
	if(NPC_Util_GetStrFromStrWithDelim( npcarg, "special_rate", buf, sizeof( buf))
	!=NULL)
	{
		rate = atof( buf);		
		intrate = ( int)( rate *1000);		
   		CHAR_setWorkInt( meindex , CHAR_WORK_SPECIALRATE ,intrate );
	}else{
		CHAR_setWorkInt( meindex , CHAR_WORK_SPECIALRATE ,1200 );
	}
	
	// ÍÂ¤±Æþ¤ì¶âµ­²±
	//cost = NPC_Util_GetNumFromStrWithDelim( npcarg, "pool_cost");
	//if( cost == -1 ) cost = NPC_PETSHOP_POOLCOST_DEFAULT;
	//CHAR_setWorkInt( meindex, CHAR_WORK_POOLCOST, cost);
	
	
	// ÍÂ¤±Æþ¤ì¤¬½ÐÍè¤ë¤«¤É¤¦¤«¤òµ­²±
	tmp = NPC_Util_GetNumFromStrWithDelim( npcarg, "pool_flg");
	if( tmp != 1 ) tmp = 0;
	CHAR_setWorkInt( meindex, CHAR_WORK_POOLFLG, tmp);
    
    return TRUE;

}




/*********************************
* ÏÃ¤·¤«¤±¤é¤ì¤¿»þ¤Î½èÍý
*********************************/
void NPC_PetShopTalked( int meindex , int talkerindex , char *szMes ,int color )
{

    /* ¥×¥ì¥¤¥ä¡¼¤ËÂÐ¤·¤Æ¤À¤±È¿±þ¤¹¤ë */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }
	

	/*--ÌÜ¤ÎÁ°¤Ë¤¤¤ë¤«¤É¤¦¤«¡©--*/
	if(NPC_Util_isFaceToFace(talkerindex,meindex,2 )==FALSE){
		/* £±¥°¥ê¥Ã¥É°ÊÆâ¤Î¤ß */
		if( NPC_Util_CharDistance( talkerindex, meindex ) > 1) return;
	}

	/*--¥ï¡¼¥¯¤Î½é´ü²½--*/
	CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANT,0);

	/*-¤Ï¤¸¤á¤ÎÁªÂò²èÌÌ--*/
	NPC_PetShop_selectWindow( meindex, talkerindex,0,-1);

}

/*********************************
* ¸«¤é¤ì¤¿»þ¤Î½èÍý
*********************************/
void NPC_PetShopLooked( int meindex , int lookedindex)
{
    /* ¥×¥ì¥¤¥ä¡¼¤ËÂÐ¤·¤Æ¤À¤±È¿±þ¤¹¤ë */
    if( CHAR_getInt( lookedindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }

	/* £±¥°¥ê¥Ã¥É°ÊÆâ¤Î¤ß */
	if( NPC_Util_CharDistance( lookedindex, meindex ) > 2) return;

	/*--¥ï¡¼¥¯¤Î½é´ü²½--*/
//	CHAR_setWorkInt(lookedindex,CHAR_WORKSHOPRELEVANT,0);


//	NPC_PetShop_selectWindow( meindex, lookedindex,0,-1);

}

static void NPC_PetShop_selectWindow( int meindex, int toindex, int num,int select)
{

	char token[1024];
	int buttontype=0,windowtype=0,windowno=0;
	char buf[1024];
	int fd = getfdFromCharaIndex( toindex);
	
	//print("\n NPC_PetShop_selectWindow num:%d select:%d  ", num, select);

	if( fd == -1 ) {
		fprint( "getfd err\n");
		return;
	}
	
	token[0] = '\0';

	switch(num){
      // ºÇ½é¤Î¥¦¥£¥ó¥É¥¦
	  case 0:
		NPC_MaxGoldOver( meindex, toindex, NPC_PETSHOP_MSG_MAIN, buf);
#ifdef _NPC_DEPOTPET
		if( !CHAR_getWorkInt( meindex, CHAR_WORK_POOLFLG)) {
			sprintf(token, "4\n¡@¡@¡@¡@¡@¡@¡¸Ãdª«°Ó©±¡¸"
							"%s"
							"\n"
							"\n¡@¡@¡@¡@¡@¡@ ¡m ½æÃdª« ¡n"
							"\n"
							"\n"
							"\n             ¡m Â÷¶} ¡n",
							buf);
		}else {
			sprintf(token, "4\n¡@¡@¡@¡@¡@¡@¡¸Ãdª«°Ó©±¡¸"
							"%s"
							"\n            ¡m ±H©ñÃdª« ¡n"
							"\n            ¡m »â¨úÃdª« ¡n"
							"\n¡@¡@¡@¡@¡@¡@ ¡m ½æÃdª« ¡n"
							"\n            ¡m ¨Ï¥Î­Ü®w ¡n"
							"\n              ¡m Â÷¶} ¡n",
							buf);
		}
#else
		if( !CHAR_getWorkInt( meindex, CHAR_WORK_POOLFLG)) {
			sprintf(token, "4\n¡@¡@¡@¡@¡@¡@¡¸Ãdª«°Ó©±¡¸"
							"%s"
							"\n"
							"\n¡@¡@¡@¡@¡@¡@ ¡m ½æÃdª« ¡n"
							"\n"
							"\n             ¡m Â÷¶} ¡n",
							buf);
		}else {
			sprintf(token, "4\n¡@¡@¡@¡@¡@¡@¡¸Ãdª«°Ó©±¡¸"
							"%s"
							"\n            ¡m ±H©ñÃdª« ¡n"
							"\n            ¡m »â¨úÃdª« ¡n"
							"\n¡@¡@¡@¡@¡@¡@ ¡m ½æÃdª« ¡n"
							"\n              ¡m Â÷¶} ¡n",
							buf);
		}
#endif

		buttontype=WINDOW_BUTTONTYPE_NONE;
		windowtype=WINDOW_MESSAGETYPE_SELECT;
		windowno=CHAR_WINDOWTYPE_WINDOWPETSHOP_START; 
		break;
	  case 1:
	  	buttontype=WINDOW_BUTTONTYPE_CANCEL;
	  	windowtype=WINDOW_MESSAGETYPE_PETSELECT;
		windowno=CHAR_WINDOWTYPE_WINDOWPETSHOP_PETSELECT; 
		break;
	  case 2:
		NPC_PetDate(meindex,toindex,select,token);
		buttontype=WINDOW_BUTTONTYPE_YESNO;
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
		windowno=CHAR_WINDOWTYPE_WINDOWPETSHOP_MAIN; 
		break;
	  case 3:
		if( (CHAR_getInt(toindex,CHAR_GOLD)+CHAR_getWorkInt(toindex,CHAR_WORKSHOPRELEVANTSEC))
			> CHAR_getMaxHaveGold( toindex) ){

			NPC_MaxGoldOver( meindex, toindex, NPC_PETSHOP_MSG_OVER, token);
			windowno=CHAR_WINDOWTYPE_WINDOWPETSHOP_GOLDOVER; 
			buttontype=WINDOW_BUTTONTYPE_YESNO;
		}else{
			NPC_PetDel(meindex,toindex,select,token);
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowno=CHAR_WINDOWTYPE_WINDOWPETSHOP_END; 
		}

	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
		break;
	  case 4:
	 	NPC_PetDel(meindex,toindex,select,token);
		buttontype=WINDOW_BUTTONTYPE_OK;
		windowno=CHAR_WINDOWTYPE_WINDOWPETSHOP_END; 
		windowtype=WINDOW_MESSAGETYPE_MESSAGE;
		break;
	  case 5: // »â¨ú±H©ñÃd¦Cªí
	  	if( CHAR_getWorkInt( meindex, CHAR_WORK_POOLFLG ) != 1 )
	  		return;
	  	buttontype=WINDOW_BUTTONTYPE_CANCEL;
	  	windowtype=WINDOW_MESSAGETYPE_PETSELECT;
		windowno=CHAR_WINDOWTYPE_WINDOWPETSHOP_PETSELECT2; 
		break;
	  case 6:
		if( CHAR_getInt( toindex, CHAR_GOLD) 
			< NPC_GETPOOLCOST( toindex)) 
		{
		  	NPC_MaxGoldOver(meindex, toindex, NPC_PETSHOP_MSG_COST, token);
			buttontype = WINDOW_BUTTONTYPE_OK;
		}
		else {
			NPC_PetDate2( meindex, toindex, select, token);
			buttontype = WINDOW_BUTTONTYPE_YESNO;
		}
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_MAIN2; 
	    break;
	  
	  case 7:
	  	if( CHAR_getWorkInt( meindex, CHAR_WORK_POOLFLG ) != 1 )
	  		return;	    
		if( CHAR_getInt( toindex, CHAR_GOLD) < NPC_GETPOOLCOST( toindex)){
		  	NPC_MaxGoldOver(meindex, toindex, NPC_PETSHOP_MSG_COST, token);
		}else {
			NPC_PetDel2( meindex, toindex, select, token);
		}
		buttontype = WINDOW_BUTTONTYPE_OK;
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_END; 
	    break;
	  // ÍÂ¤±Æþ¤ìÍó¤¬°ìÇÕ
	  case 8:
	  	NPC_MaxGoldOver( meindex, toindex, NPC_PETSHOP_MSG_POOLFULL, token);
		buttontype = WINDOW_BUTTONTYPE_OK;
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_END; 
	    break;
	  // °ú¤­¼è¤ê¥Ú¥Ã¥È°ìÍ÷¥¦¥£¥ó¥É¥¦
	  case 9: // ¨ú¥X±H©ñÃd¦Cªí
	  	if( CHAR_getWorkInt( meindex, CHAR_WORK_POOLFLG) != 1 )
	  		return;
	  	NPC_getDrawPet( meindex, toindex, token, &buttontype, select);
	  	windowtype = WINDOW_MESSAGETYPE_SELECT;
		windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_DRAWSELECT; 
		break;
	  // °ú¤­¼è¤ê£Ï£Ë¡©
	  case 10:
		NPC_PetDate3( meindex, toindex, select, token);
		buttontype = WINDOW_BUTTONTYPE_YESNO;
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_ASKDRAW;
	    break;
	  // ÊÖ¤¹
	  case 11:
	  	if( CHAR_getWorkInt( meindex, CHAR_WORK_POOLFLG ) != 1 )
	  		return;	  
		NPC_PetDel3( meindex, toindex, select, token);
		buttontype = WINDOW_BUTTONTYPE_OK;
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_END; 
	    break;
	  // ¥Ú¥Ã¥ÈÍó¤¬°ìÇÕ
	  case 12:
	  	NPC_MaxGoldOver( meindex, toindex, NPC_PETSHOP_MSG_GETFULL, token);
		buttontype = WINDOW_BUTTONTYPE_OK;
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_END; 
	    break;
#ifdef _NPC_DEPOTPET
	  case 13: // ¦s¤J­Ü®w¦Cªí
	  	if( !CHAR_CheckDepotPet( toindex) )
	  		return;
	  	buttontype=WINDOW_BUTTONTYPE_CANCEL;
	  	windowtype=WINDOW_MESSAGETYPE_PETSELECT;
		windowno=CHAR_WINDOWTYPE_DEPOTPETSHOP_ADD;
	    break;
	  case 14: // ¨ú¥X­Ü®w¦Cªí
	  	if( !CHAR_CheckDepotPet( toindex) )
	  		return;
		NPC_DepotPet_List( meindex, toindex, token, &buttontype, select);
	  	//buttontype=WINDOW_BUTTONTYPE_CANCEL;
		windowtype = WINDOW_MESSAGETYPE_SELECT;
		windowno = CHAR_WINDOWTYPE_DEPOTPETSHOP_GET;
		break;

	  case 15: // ¸ß°Ý½T»{¦s¤J­Ü®w
		if( CHAR_getInt( toindex, CHAR_GOLD) 
			< NPC_GETPOOLCOST( toindex)) 
		{
		  	NPC_MaxGoldOver(meindex, toindex, NPC_PETSHOP_MSG_COST, token);
			buttontype = WINDOW_BUTTONTYPE_OK;
		}
		else {
			NPC_PetDate2( meindex, toindex, select, token);
			buttontype = WINDOW_BUTTONTYPE_YESNO;
		}
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		windowno = CHAR_WINDOWTYPE_DEPOTPETSHOP_ASKADD;
	    break;

	  case 16: // ¸ß°Ý½T»{¨ú¥X­Ü®w
		NPC_DepotPet_AskGet( meindex, toindex, select, token);
		buttontype = WINDOW_BUTTONTYPE_YESNO;
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		windowno = CHAR_WINDOWTYPE_DEPOTPETSHOP_ASKGET;
	    break;

	  case 17: // ½T»{¦s¤J­Ü®w
	  	if( !CHAR_CheckDepotPet(toindex) )
	  		return;
		if( CHAR_getInt( toindex, CHAR_GOLD) < NPC_GETPOOLCOST( toindex)){
		  	NPC_MaxGoldOver(meindex, toindex, NPC_PETSHOP_MSG_COST, token);
		}else {
			NPC_DepotPet_Add( meindex, toindex, select, token);
		}
		buttontype = WINDOW_BUTTONTYPE_OK;
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_END; 
	    break;

	  case 18: // ½T»{¨ú¥X­Ü®w
	  	if( CHAR_getWorkInt( meindex, CHAR_WORK_POOLFLG ) != 1 )
	  		return;	  
		if( NPC_DepotPet_CheckLevel( meindex, toindex, select, token) == TRUE) {
			NPC_DepotPet_Get( meindex, toindex, select, token);
			buttontype = WINDOW_BUTTONTYPE_OK;
	  		windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_END; 
		}
		else {
			sprintf( token, "\n§AµLªk·ÓÅU³o°¦Ãdª«¡C");
			buttontype = WINDOW_BUTTONTYPE_OK;
			windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			windowno = CHAR_WINDOWTYPE_WINDOWPETSHOP_END;
		}
	    break;
#endif
	  // CoolFish: Add 2001/7/4
	  default:
	    break;
	
	}
	
	/*--¥¨¥¹¥±¡¼¥×--*/
	//makeEscapeString( token, escapedname, sizeof(escapedname));
		
		
	/*--Á÷¿®--*/
	lssproto_WN_send( fd, windowtype, 
				buttontype, 
				windowno,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

}

void NPC_PetShopWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	int datanum = -1;
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) {
		return;
	}
	
	//print("\n NPC_PetShopWindowTalked: data:%s seq:%d sel:%d ", data, seqno, select);

	makeStringFromEscaped( data);
	
	datanum = atoi( data);

	switch( seqno){
	case CHAR_WINDOWTYPE_WINDOWPETSHOP_START:
		{
			int poolflg = CHAR_getWorkInt( meindex, CHAR_WORK_POOLFLG);
			
			if( ( datanum == 3 && poolflg) || // ½æÃdª«
				( datanum == 2 && !poolflg ) ){	// »â¨úÃdª«
				NPC_PetShop_selectWindow( meindex, talkerindex, 1, -1 );
			}else if( datanum == 1 && poolflg) { // ±H©ñÃdª« 
				if( CHAR_getCharPoolPetElement( talkerindex) != -1 ) {
					NPC_PetShop_selectWindow( meindex, talkerindex, 5, -1 );
				}
				else {
					NPC_PetShop_selectWindow( meindex, talkerindex, 8, -1 );
				}
			}else if( datanum == 2 && poolflg ) { // »â¨úÃdª«
				if( CHAR_getCharPetElement( talkerindex) != -1 ) {
					CHAR_setWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT, 0);
					NPC_PetShop_selectWindow( meindex, talkerindex, 9, 0);
				}else {
					NPC_PetShop_selectWindow( meindex, talkerindex, 12, -1 );
				}
			}
#ifdef _NPC_DEPOTPET
			else if( datanum == 4 ) { // ¨Ï¥ÎÃdª«­Ü®w
				print(" ¨Ï¥ÎÃdª«­Ü®w ");
				CHAR_setWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT, 0);
				if( !CHAR_CheckDepotPet( talkerindex) ){
					CHAR_GetDepotPet( meindex, talkerindex);
					CHAR_talkToCli( talkerindex, -1, "¨ú±oÃdª«¡A½Ðµy«á¡I", CHAR_COLORYELLOW);
				}else{
					NPC_PoolPetShop_DepotPet_Menu( meindex, talkerindex);
				}
			}
#endif
		}
		break;
	  case CHAR_WINDOWTYPE_WINDOWPETSHOP_PETSELECT:
		if(select==WINDOW_BUTTONTYPE_CANCEL){
			NPC_PetShop_selectWindow( meindex, talkerindex, 0,-1 );
			
		}else{
			NPC_PetShop_selectWindow( meindex, talkerindex, 2, datanum );
		}
		break;
	  case CHAR_WINDOWTYPE_WINDOWPETSHOP_PETSELECT2:
		if(select==WINDOW_BUTTONTYPE_CANCEL){
			NPC_PetShop_selectWindow( meindex, talkerindex, 0,-1 );
		}else{
			NPC_PetShop_selectWindow( meindex, talkerindex, 6, datanum );
		}
		break;
	  case CHAR_WINDOWTYPE_WINDOWPETSHOP_MAIN:
		if(select==WINDOW_BUTTONTYPE_YES){
			NPC_PetShop_selectWindow( meindex, talkerindex, 3,-1 );
		}else{
			NPC_PetShop_selectWindow( meindex, talkerindex,1,-1);
	  	}
	  	break;
	  case CHAR_WINDOWTYPE_WINDOWPETSHOP_MAIN2:
		if(select==WINDOW_BUTTONTYPE_YES){
			NPC_PetShop_selectWindow( meindex, talkerindex, 7,-1 );
		}else {
			NPC_PetShop_selectWindow( meindex, talkerindex,5,-1);
	  	}
	  	break;
	  case CHAR_WINDOWTYPE_WINDOWPETSHOP_ASKDRAW:
		if( select == WINDOW_BUTTONTYPE_YES){
			if( CHAR_getCharPetElement( talkerindex) != -1 ) {
				NPC_PetShop_selectWindow( meindex, talkerindex, 11,-1 );
			}else {
				NPC_PetShop_selectWindow( meindex, talkerindex, 12, -1 );
			}
		}else {
			CHAR_setWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT, 0);
			NPC_PetShop_selectWindow( meindex, talkerindex, 9, 0);
	  	}
	  	break;
	  case CHAR_WINDOWTYPE_WINDOWPETSHOP_GOLDOVER:
		if( select == WINDOW_BUTTONTYPE_YES){
			NPC_PetShop_selectWindow( meindex, talkerindex, 4,-1 );
		}else{
			NPC_PetShop_selectWindow( meindex, talkerindex, 0,-1 );
		}
	  	break;
	  case CHAR_WINDOWTYPE_WINDOWPETSHOP_DRAWSELECT:
		if( select == WINDOW_BUTTONTYPE_PREV){
			NPC_PetShop_selectWindow( meindex, talkerindex, 9, -1);
		}else if( select == WINDOW_BUTTONTYPE_NEXT) {
			NPC_PetShop_selectWindow( meindex, talkerindex, 9, 1);
		}else if( select != WINDOW_BUTTONTYPE_CANCEL) {
			if( CHAR_getCharPetElement( talkerindex) != -1 ) {
				NPC_PetShop_selectWindow( meindex, talkerindex, 10, datanum);
			}else {
				NPC_PetShop_selectWindow( meindex, talkerindex, 12, -1 );
			}
		}else {
			NPC_PetShop_selectWindow( meindex, talkerindex, 0,-1 );
		}
		break;

#ifdef _NPC_DEPOTPET
	  case CHAR_WINDOWTYPE_DEPOTPETSHOP_HANDLE:
		  if( !CHAR_CheckDepotPet( talkerindex) ) return ;
		  switch( datanum ) {
		  case 1://©ñ¤J
				if( CHAR_findEmptyDepotPet( talkerindex) == -1 ){
					CHAR_talkToCli( talkerindex, -1, "Ãdª«­Ü®w¤wº¡¡I", CHAR_COLORYELLOW);
					return;
				}
				//NPC_DepotPet_Pet_printWindow( meindex, talkerindex);
				NPC_PetShop_selectWindow( meindex, talkerindex, 13, -1 );
			  break;
		  case 2://¨ú¥X
				if( CHAR_getCharPetElement( talkerindex) < 0 ) {
					CHAR_talkToCli( talkerindex, -1, "¨­¤WÃdª«Äæ¦ì¤wº¡¡I", CHAR_COLORYELLOW);
					return;
				}
				NPC_PetShop_selectWindow( meindex, talkerindex, 14, 0);
			  break;
		  case 0:
			  NPC_PetShop_selectWindow( meindex, talkerindex, 0, -1);
			  break;
		  }
		  break;

	  case CHAR_WINDOWTYPE_DEPOTPETSHOP_ADD:
		if(select==WINDOW_BUTTONTYPE_CANCEL){
			NPC_PetShop_selectWindow( meindex, talkerindex, 0,-1 );
		}else{
			NPC_PetShop_selectWindow( meindex, talkerindex, 15, datanum );
		}
		break;

	  case CHAR_WINDOWTYPE_DEPOTPETSHOP_GET:
		if( select == WINDOW_BUTTONTYPE_PREV){
			NPC_PetShop_selectWindow( meindex, talkerindex, 14, -1);
		}else if( select == WINDOW_BUTTONTYPE_NEXT) {
			NPC_PetShop_selectWindow( meindex, talkerindex, 14, 1);
		}else if( select != WINDOW_BUTTONTYPE_CANCEL) {
			if( CHAR_getCharPetElement( talkerindex) != -1 ) {
				NPC_PetShop_selectWindow( meindex, talkerindex, 16, datanum);
			}else {
				NPC_PetShop_selectWindow( meindex, talkerindex, 12, -1 );
			}
		}else {
			NPC_PetShop_selectWindow( meindex, talkerindex, 0,-1 );
		}

		break;
	  case CHAR_WINDOWTYPE_DEPOTPETSHOP_ASKADD: // ½T»{¦s¤J­Ü®w
		if(select==WINDOW_BUTTONTYPE_YES){
			NPC_PetShop_selectWindow( meindex, talkerindex, 17,-1 );
		}else {
			NPC_PetShop_selectWindow( meindex, talkerindex,13,-1);
	  	}
		break;
			
	  case CHAR_WINDOWTYPE_DEPOTPETSHOP_ASKGET: // ½T»{¨ú¥X­Ü®w
		if( select == WINDOW_BUTTONTYPE_YES){
			if( CHAR_getCharPetElement( talkerindex) != -1 ) {
				NPC_PetShop_selectWindow( meindex, talkerindex, 18,-1 );
			}else {
				NPC_PetShop_selectWindow( meindex, talkerindex, 12, -1 );
			}
		}else {
			CHAR_setWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT, 0);
			NPC_PetShop_selectWindow( meindex, talkerindex, 14, 0);
	  	}
		break;
#endif
	  
	  case CHAR_WINDOWTYPE_WINDOWPETSHOP_END:
		if( select == WINDOW_BUTTONTYPE_OK){
			NPC_PetShop_selectWindow( meindex, talkerindex, 0,-1 );
		}
	  	break;

	  // CoolFish: Add 2001/7/4
	  default:
	  	break;

	}
}

void NPC_PetDate(int meindex,int talker,int select,char *token)
{
	
	int petindex;
	int cost;
	char *petname;
	char buf[1024];

	/*--¥ï¡¼¥¯¤Î½é´ü²½--*/
	CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,select);

	petindex = CHAR_getCharPet( talker, select-1);
        

   if( !CHAR_CHECKINDEX(petindex) ) return;

  	NPC_MaxGoldOver( meindex, talker, NPC_PETSHOP_MSG_REALY, buf);

	cost=NPC_GetCost(meindex,talker,petindex);

	petname = CHAR_getUseName( petindex);

	sprintf(token,"\n¬O[%s]¨S¿ù§a¡H""%s"
					"\n\n    ¶R½æ»ù®æ         %d STONE\n"
					,petname,buf,cost);	
		
}

void NPC_PetDate2(int meindex,int talker,int select,char *token)
{
	
	int petindex;
	char *petname;
	CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,select);

	petindex = CHAR_getCharPet( talker, select-1);
	if( !CHAR_CHECKINDEX(petindex) ) return;
	petname = CHAR_getUseName( petindex);
	sprintf(token,"\n±H©ñ[%s]"
					"\n\n    ±H©ñ»ù®æ            %d stone\n"
					,petname, NPC_GETPOOLCOST( talker)
					);	
}

void NPC_PetDate3(int meindex,int talker,int select,char *token)
{
	int poolindex;
	int petindex;
	char *petname;
	poolindex = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANT) * 5 + select - 1;
	CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,poolindex );
	petindex = CHAR_getCharPoolPet( talker, poolindex);
	if( !CHAR_CHECKINDEX(petindex) ) return;
	petname = CHAR_getUseName( petindex);
	sprintf(token,"\n»â¦^[%s]", petname);	
}

int NPC_GetCost(int meindex,int talker,int petindex)
{
	int cost;
	int levelcost;
	int level;
	int getlevel;
	int graNo;
	int rare;
	int charm=50;
	int petai;
	int i=1;
	double rate=1.0;
	int intrate;
	char buf[1024];
	char buff2[128];
	char npcarg[NPC_UTIL_GETARGSTR_BUFSIZE];
		
	if(NPC_Util_GetArgStr( meindex, npcarg, sizeof(npcarg))==NULL){
		print("GetArgStrErr");
		return FALSE;
	}

		
	getlevel=CHAR_getInt(petindex,CHAR_PETGETLV);
	level=CHAR_getInt(petindex,CHAR_LV);
	graNo=CHAR_getInt(petindex,CHAR_BASEBASEIMAGENUMBER);
	rare=CHAR_getInt(petindex,CHAR_RARE);

	if(getlevel==0)	getlevel=1	;
	
	/*--ÃÍÃÊ¤òµá¤á¤ë--*/
	levelcost=(level*level)*10;
	getlevel=(getlevel*getlevel)*10;
// Syu ADD ­×§ï½æÂà¥ÍÃdª÷¿ú·|ÅÜ­tªºbug
#ifdef _PET_2TRANS
	if( CHAR_getInt(petindex , CHAR_TRANSMIGRATION)>0)
#else
	if( CHAR_getInt(petindex , CHAR_TRANSMIGRATION)==1)
#endif
		getlevel=10;


	if(rare==0){ rare=1;
	}else if(rare==1){ rare=5;
	}else if(rare==2){ rare=8;}

	cost = ((levelcost-getlevel)+(level*PETCOST))*rare;
	intrate = CHAR_getWorkInt( meindex , CHAR_WORK_NOMALRATE);
	rate=(double) intrate / 1000;
	

	NPC_Util_GetStrFromStrWithDelim( npcarg, "special_pet", buf, sizeof( buf));

	i=1;
	/*--¥¹¥Ú¥·¥ã¥ë¥ì¡¼¥È¤Ç·×»»¤µ¤ì¤ë¥Ú¥Ã¥È¤ò³ä¤ê½Ð¤¹¡Ê¥Ú¥Ã¥È¤Î»ØÄê¤Ï²èÁüÈÖ¹æ)--*/
	while( getStringFromIndexWithDelim(buf,",",i,buff2,sizeof(buff2)) !=FALSE )
	{
		if(strstr( buff2, "-")==NULL){
			if(graNo == atoi(buff2)){
				intrate=CHAR_getWorkInt( meindex , CHAR_WORK_SPECIALRATE);
				rate=(double) intrate / 1000;
				break; 
			}
		}else{
			/*--¥¢¥¤¥Æ¥à¤¬¡É15-25¡É¤Î·Á¤Ç¶èÀÚ¤é¤ì¤Æ¤¤¤ë¾ì¹ç--*/
			int start;
			int end;
			int tmp;
			int graflg=0;
			char	token2[128];
	
			/*-"-"¤Ç¶èÀÚ¤é¤ì¤¿»Ï¤á¤Î¿ôÃÍ¤È¸å¤Î¿ôÃÍ¤ò¼èÆÀ--*/
			getStringFromIndexWithDelim( buff2, "-", 1, token2, sizeof(token2));
			start = atoi( token2);
			getStringFromIndexWithDelim( buff2, "-", 2 ,token2, sizeof(token2));
			end = atoi( token2);

			/*--ÈÖ¹æ¤¬µÕ¤Ë¤Ê¤Ã¤Æ¤¤¤¿¤é¡¢Æþ¤ìÂØ¤¨¤ë**/
			if(start > end){
				tmp=start;
				start=end;
				end=tmp;
			}

			end++;
			/*--"-"¤Ç¶èÀÚ¤é¤ì¤¿Ê¬¤Î¥¢¥¤¥Æ¥à¾ðÊó¤òÆÀ¤ë--*/
			if(start <= graNo && graNo < end){
				intrate=CHAR_getWorkInt( meindex , CHAR_WORK_SPECIALRATE);
				rate=(double) intrate / 1000;
				graflg=1;
				break; 
			}
			
			if(graflg ==1) break;
		}
		i++;
	}

	/*--¥Ú¥Ã¥È¤ÎÃéÀ¿ÅÙ¤ò£Ç£Å£Ô---*/
	petai=CHAR_getWorkInt(petindex,CHAR_WORKFIXAI);

	/*--Ì¥ÎÏ¤ò¥²¥Ã¥È--*/
	charm = CHAR_getWorkInt(talker,CHAR_WORKFIXCHARM);

	/*--¡ÊÌ¥ÎÏ¡ÜÃéÀ¿ÅÙ¡Ë/ £²  */
	charm = charm + petai;

	if(charm < 20){
		charm = 20;
	}

	charm = charm / 2;

	cost = (int)cost*rate;

	rate=(double)charm/100;

	/*--Ì¥ÎÏÃéÀ¿³Ý¤±--*/
	cost = cost * rate;

	/*--¤ª¶â¤òµ­²±--*/
   	CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,cost);

	return cost;
}


#if 1

void NPC_PetDel(int meindex, int talker,int select,char* token)
{
	int petindex;
	int petsel;
	char szPet[128];
	int cost;
	int defpet;
	int fd = getfdFromCharaIndex( talker );

	NPC_MaxGoldOver( meindex, talker, NPC_PETSHOP_MSG_THANKS, token);
	petsel=(CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANT)-1);
	petindex = CHAR_getCharPet( talker, petsel);
	
    if( !CHAR_CHECKINDEX(petindex) ) return;

        // CoolFish: Family 2001/6/14
	if (CHAR_getInt(petindex, CHAR_PETFAMILY) == 1){	
/*		char buf[1024];
		lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
			WINDOW_BUTTONTYPE_OK,
			-1, -1,
		makeEscapeString("\n\n«Ü©êºp¡ã¦uÅ@Ã~µLªk³c½æ³á¡I\n½Ð¦Û¤v¦n¦n·ÓÅU¡I", buf, sizeof(buf)));
*/
		sprintf(token,"\n\n«Ü©êºp¡ã¦uÅ@Ã~µLªk³c½æ³á¡I\n½Ð¦Û¤v¦n¦n·ÓÅU¡I");
		return;
	}

        // Robin 0725
        if (CHAR_getInt( talker, CHAR_RIDEPET ) == petsel ){	
/*        	char buf[1024];
        	lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
        		WINDOW_BUTTONTYPE_OK,
        		-1, -1,
        		makeEscapeString("\n\nÃM­¼¤¤ªºÃdª«µLªk³c½æ³á¡I", buf, sizeof(buf)));
*/
					sprintf(token,"\n\nÃM­¼¤¤ªºÃdª«µLªk³c½æ³á¡I");
        	return;
        }

	if( CHAR_getWorkInt( CONNECT_getCharaindex(fd),
                         CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE) return ;
	defpet=CHAR_getInt(talker,CHAR_DEFAULTPET);
	if(defpet==petsel){
		CHAR_setInt(talker,CHAR_DEFAULTPET,-1);
		lssproto_KS_send( fd, -1, TRUE);
	}
   	CHAR_setCharPet( talker, petsel, -1);
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ), /* ¥­¥ã¥éÌ¾ */
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"sell(½æÃd)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);
	CHAR_endCharOneArray( petindex );
		
	snprintf( szPet, sizeof( szPet ), "K%d", petsel);
	CHAR_sendStatusString( talker, szPet );
	cost=NPC_GetCost(meindex,talker,petindex);
	CHAR_AddGold( talker, cost);

}
#endif
void NPC_PetDel2( int meindex, int talker, int select, char *token)
{
	
	int petindex;
	int selectpet;
	int emptyindex;
	char szPet[128];
	int fd = getfdFromCharaIndex( talker );

	if( fd == -1 ) {
		fprint( "err\n");
		return;
	}
	// ¥Ú¥Ã¥È¥Ç¡¼¥¿ÊÑ¹¹¡£¥Ú¥Ã¥ÈÍó¢ªÍÂ¤±Æþ¤ìÍó
	selectpet = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANT) -1;
	petindex = CHAR_getCharPet( talker, selectpet);
	if( !CHAR_CHECKINDEX( petindex) ) return;
	
        // Robin 0725
        if (CHAR_getInt( talker, CHAR_RIDEPET ) == selectpet )
        {
/*        	char buf[1024];
        	lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
        		WINDOW_BUTTONTYPE_OK,
        		-1, -1,
        		makeEscapeString("\n\nÃM­¼¤¤ªºÃdª«µLªk±H©ñ³á¡I", buf, sizeof(buf)));
*/
					sprintf(token,"\n\nÃM­¼¤¤ªºÃdª«µLªk±H©ñ³á¡I");
        	return;
        }
        
	// ¶õ¤­¤òÃµ¤¹¡£Á°¤ËÃµ¤·¤Æ¥Á¥§¥Ã¥¯¤·¤Æ¤¤¤ë¤Î¤Ç¡¤¤³¤³¤Ç¶õ¤¤¤Æ¤¤¤Ê¤¤¤Î¤Ï¤ª¤«¤·¤¤
	emptyindex = CHAR_getCharPoolPetElement( talker);
	if( emptyindex == -1 ) {
		fprint( "err\n");
		return;
	}
	// ÀïÆ®½àÈ÷ÍÑ¥Ú¥Ã¥È¤Ê¤é¡¤¤½¤ì¤ÎÅÐÏ¿¤ò¾Ã¤¹¡£
	if( CHAR_getInt( talker, CHAR_DEFAULTPET) == selectpet) {
		CHAR_setInt( talker, CHAR_DEFAULTPET, -1);
		lssproto_KS_send( fd, -1, TRUE);
	}
	// ÍÂ¤±Æþ¤ìÍó¤Ë¡¤¥Ú¥Ã¥È¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹¤ò¥³¥Ô¡¼
	CHAR_setCharPoolPet( talker, emptyindex, petindex);
	// ¥Ú¥Ã¥ÈÍó¤«¤é¾Ã¤¹
	CHAR_setCharPet( talker, selectpet, -1);
	
	// ¥á¥Ã¥»¡¼¥¸¼èÆÀ
	NPC_MaxGoldOver( meindex, talker, NPC_PETSHOP_MSG_POOLTHANKS, token);
	// ¤ª¶â¸º¤é¤¹

	CHAR_DelGold( talker, NPC_GETPOOLCOST( talker) );

	// ¤ª¶â¥Ç¡¼¥¿Á÷¤ë
	CHAR_send_P_StatusString(talker,CHAR_P_STRING_GOLD);

	snprintf( szPet, sizeof( szPet ), "K%d", selectpet);
	// ¿·¤·¤¤¥Ú¥Ã¥È¥¹¥Æ¡¼¥¿¥¹Á÷¤ê¤Ä¤±¤ë
	CHAR_sendStatusString( talker, szPet );
	// ¥Ú¥Ã¥È¤òÍÂ¤±¤¿¥í¥°
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ), /* ¥­¥ã¥éÌ¾ */
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"pool(±H©ñÃdª«©±)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
        CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);

}	

void NPC_PetDel3( int meindex, int talker, int select, char *token)
{
	
	int petindex;
	int selectpet;
	int emptyindex;
	char szPet[128];
	int workindex[CHAR_MAXPOOLPETHAVE];
	int i;
	int cnt;

	selectpet = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANT) ;
	petindex = CHAR_getCharPoolPet( talker, selectpet);
	if( !CHAR_CHECKINDEX( petindex) ) return;
	emptyindex = CHAR_getCharPetElement( talker);
	if( emptyindex == -1 ) {
		fprint( "err\n");
		return;
	}

	// ¦pªG¬O¦uÅ@Ã~§ï¬°´¶³qÃ~
	if( CHAR_getInt( petindex, CHAR_PETFAMILY) == 1 
		&& CHAR_getInt( talker, CHAR_FMLEADERFLAG) != 3 ) {
		CHAR_setInt( petindex, CHAR_PETFAMILY, 0);
		CHAR_talkToCli( talker, -1, "¦uÅ@Ã~§ï¬°´¶³qÃ~", CHAR_COLORYELLOW);
	}

	CHAR_setCharPet( talker, emptyindex, petindex);
	CHAR_setCharPoolPet( talker, selectpet, -1);
	NPC_MaxGoldOver( meindex, talker, NPC_PETSHOP_MSG_POOLTHANKS, token);
	snprintf( szPet, sizeof( szPet ), "K%d", emptyindex);
	CHAR_sendStatusString( talker, szPet );
	snprintf( szPet, sizeof( szPet), "W%d",emptyindex);
	CHAR_sendStatusString( talker, szPet );
	for( i = 0; i < CHAR_MAXPOOLPETHAVE; i ++ ) {
		workindex[i] = -1;
	}
	cnt = 0;
	for( i = 0; i < CHAR_MAXPOOLPETHAVE; i ++ ) {
		int petindex = CHAR_getCharPoolPet( talker, i);
		if( CHAR_CHECKINDEX( petindex) ) {
			workindex[cnt++] = petindex;
		}
	}
	for( i = 0; i < CHAR_MAXPOOLPETHAVE; i ++ ) {
		CHAR_setCharPoolPet( talker, i, workindex[i]);
	}
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ),
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"draw(»â¨úÃdª«©±)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);
#ifdef _PET_EVOLUTION
	if( CHAR_getInt( petindex, CHAR_FUSIONBEIT) == 1 &&
		CHAR_getInt( petindex, CHAR_FUSIONRAISE) > 0 ){//ÀË¬d¬O§_¬°¿Ä¦XÃd
		CHAR_setInt( petindex, CHAR_FUSIONTIMELIMIT, (int)time( NULL));
	}
#endif
}	

void NPC_getDrawPet( int meindex, int toindex, char *token, int *buttontype, int select)
{
	// £±¥Ú¡¼¥¸¤¢¤¿¤ê¤Î¹Ô¿ô
	#define		LINENUM		5
	char buf[1024];
	int i;
	int startnum, endnum;
	int petindex = -1;
	// ¸½ºß¤Î¥Ú¡¼¥¸
	int page = CHAR_getWorkInt( toindex, CHAR_WORKSHOPRELEVANT);
	int limit;
	//andy_reEdit 2003/09/18
	limit = (CHAR_getInt(toindex,CHAR_TRANSMIGRATION)*2)+5;

	endnum = (page + select+1)*LINENUM ;
	startnum = endnum - LINENUM;
	*buttontype = WINDOW_BUTTONTYPE_CANCEL;
	// Ã¼¥Ú¡¼¥¸Ä´À°¡£
	if( endnum >= limit ) {
		endnum = limit;
		if( page +select!= 0 ) *buttontype |= WINDOW_BUTTONTYPE_PREV;
	}
	else if( endnum <= LINENUM ) {
		endnum = LINENUM;
		*buttontype |= WINDOW_BUTTONTYPE_NEXT;
	}
	else {
		*buttontype |= WINDOW_BUTTONTYPE_PREV | WINDOW_BUTTONTYPE_NEXT;
	}
	CHAR_setWorkInt( toindex, CHAR_WORKSHOPRELEVANT, page +select);
	
	strcpy( token, "2\n@½Ð¿ï¾ÜÃdª«@\n\n");
	for( i =  startnum; i < endnum; i ++ ) {
		petindex = CHAR_getCharPoolPet( toindex, i);
		if( CHAR_CHECKINDEX( petindex) ) {
			snprintf( buf, sizeof( buf), "LV.%3d MaxHP %3d %s\n", 
					CHAR_getInt( petindex, CHAR_LV),
					CHAR_getWorkInt( petindex, CHAR_WORKMAXHP),
					CHAR_getUseName( petindex)
					);
		}
		else {
			strcpy( buf, "\n");
		}
		strcat( token, buf);
	}
	// ¼¡¤Î°ì¸Ä¤òÄ´¤Ù¤Æ¡¤Ìµ¤«¤Ã¤¿¤é¼¡¤Ø¥Ü¥¿¥ó¤ò¼è¤ë
	if( i != limit ) {
		petindex = CHAR_getCharPoolPet( toindex, i);
		if( !CHAR_CHECKINDEX( petindex)) {
			*buttontype &= ~WINDOW_BUTTONTYPE_NEXT;
		}
	}
}

/*
 *¤ª¶â¤¬¥ª¡¼¥Ð¡¼¤½¤ÎÂ¾¤¤¤í¤¤¤í
 */
void	NPC_MaxGoldOver(int meindex,int toindex,int select,char *token)
{

	char buf[1024];
	char npcarg[NPC_UTIL_GETARGSTR_BUFSIZE];

	NPC_Util_GetArgStr( meindex, npcarg, sizeof(npcarg));
	
	if( select < 0 || select >= arraysizeof( shopmsg)) return;
	
	if( NPC_Util_GetStrFromStrWithDelim( npcarg, shopmsg[select].option, buf, sizeof( buf))
		== NULL ) 
	{
		strcpy( buf, shopmsg[select].defaultmsg);
	}

	snprintf(token,sizeof(buf),"\n\n%s",buf);

}

#ifdef _NPC_DEPOTPET
void NPC_PoolPetShop_DepotPet_Menu( int meindex, int talkerindex)
{
	int fd;
	
	//print(" NPC_PoolPetShop_DepotPet_Menu ");

	fd = getfdFromCharaIndex( talkerindex);
	if( fd != -1 ) {
		char	message[1024];
		char	buf[2048];

		strcpy( message, 
			"3\n\n"
			"          ¨Ï¥ÎÃdª«­Ü®w\n\n"
			"          ¡Õ¦s©ñÃdª«¡Ö\n"
			"          ¡Õ¨ú¦^Ãdª«¡Ö\n"
		);

		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_SELECT, 
						WINDOW_BUTTONTYPE_CANCEL,
						CHAR_WINDOWTYPE_DEPOTPETSHOP_HANDLE,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						makeEscapeString( message, buf, sizeof(buf)));
	}
}

void NPC_DepotPet_List( int meindex, int toindex, char *token, int *buttontype, int select)
{
	// £±¥Ú¡¼¥¸¤¢¤¿¤ê¤Î¹Ô¿ô
	#define		LINENUM		5
	char buf[1024];
	int i;
	int startnum, endnum;
	int petindex = -1;
	// ¸½ºß¤Î¥Ú¡¼¥¸
	int page = CHAR_getWorkInt( toindex, CHAR_WORKSHOPRELEVANT);
	int limit;

	limit = CHAR_MAXDEPOTPETHAVE;

	endnum = (page + select+1)*LINENUM ;
	startnum = endnum - LINENUM;
	*buttontype = WINDOW_BUTTONTYPE_CANCEL;
	// Ã¼¥Ú¡¼¥¸Ä´À°¡£
	if( endnum >= limit ) {
		endnum = limit;
		if( page +select!= 0 ) *buttontype |= WINDOW_BUTTONTYPE_PREV;
	}
	else if( endnum <= LINENUM ) {
		endnum = LINENUM;
		*buttontype |= WINDOW_BUTTONTYPE_NEXT;
	}
	else {
		*buttontype |= WINDOW_BUTTONTYPE_PREV | WINDOW_BUTTONTYPE_NEXT;
	}
	CHAR_setWorkInt( toindex, CHAR_WORKSHOPRELEVANT, page +select);
	
	strcpy( token, "2\n@½Ð¿ï¾Ü­n±q­Ü®w¨ú¥XªºÃdª«@\n\n");
	for( i =  startnum; i < endnum; i ++ ) {
		petindex = CHAR_getDepotPetIndex( toindex, i);
		if( CHAR_CHECKINDEX( petindex) ) {
			snprintf( buf, sizeof( buf), "LV.%3d MaxHP %3d %s\n", 
					CHAR_getInt( petindex, CHAR_LV),
					CHAR_getWorkInt( petindex, CHAR_WORKMAXHP),
					CHAR_getUseName( petindex)
					);
		}
		else {
			strcpy( buf, "\n");
		}
		strcat( token, buf);
	}
	// ¼¡¤Î°ì¸Ä¤òÄ´¤Ù¤Æ¡¤Ìµ¤«¤Ã¤¿¤é¼¡¤Ø¥Ü¥¿¥ó¤ò¼è¤ë
	if( i != limit ) {
		petindex = CHAR_getDepotPetIndex( toindex, i);
		if( !CHAR_CHECKINDEX( petindex)) {
			*buttontype &= ~WINDOW_BUTTONTYPE_NEXT;
		}
	}
	//print("\n DepotPet_List:%s ", token);
}


void NPC_DepotPet_Add( int meindex, int talker, int select, char *token)
{
	
	int petindex;
	int selectpet;
	int emptyindex;
	char szPet[128];
	int fd = getfdFromCharaIndex( talker );

	if( fd == -1 ) {
		fprint( "err\n");
		return;
	}
	// ¥Ú¥Ã¥È¥Ç¡¼¥¿ÊÑ¹¹¡£¥Ú¥Ã¥ÈÍó¢ªÍÂ¤±Æþ¤ìÍó
	selectpet = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANT) -1;
	//print("\n selectpet:%d ", selectpet);
	petindex = CHAR_getCharPet( talker, selectpet);
	if( !CHAR_CHECKINDEX( petindex) ) return;
	
    // Robin 0725
    if (CHAR_getInt( talker, CHAR_RIDEPET ) == selectpet )
    {
		sprintf(token,"\n\nÃM­¼¤¤ªºÃdª«µLªk¦s¤J­Ü®w³á¡I");
		return;
	}

	if( CHAR_getInt( petindex, CHAR_PETFAMILY) == 1 )
	{
		sprintf(token,"\n\n®a±Ú¦uÅ@Ã~µLªk¦s¤J­Ü®w³á¡I");
		return;
	}

#ifdef _CHECK_ENEMY_PET
	{
		int j;
		for( j=0;j<ENEMYTEMP_getEnemyNum();j++ ) {//ENEMY_getEnemyNum()
			if( CHAR_getInt( petindex, CHAR_PETID) == ENEMYTEMP_getInt( j, E_T_TEMPNO ) ) {//ENEMY_getInt( j, ENEMY_TEMPNO)
				if( ENEMYTEMP_getInt( j, E_T_PETFLG ) == 0 ){//ENEMY_getInt( j, ENEMY_PETFLG) 
				    sprintf(token,"\n\n³o¬O¤£¯à«ù¦³ªºÃdª«³á¡I");
					return;
				}
			}
		}
	}
#endif    
	// ¶õ¤­¤òÃµ¤¹¡£Á°¤ËÃµ¤·¤Æ¥Á¥§¥Ã¥¯¤·¤Æ¤¤¤ë¤Î¤Ç¡¤¤³¤³¤Ç¶õ¤¤¤Æ¤¤¤Ê¤¤¤Î¤Ï¤ª¤«¤·¤¤
	emptyindex = CHAR_getCharDepotPetElement( talker);
	if( emptyindex == -1 ) {
		fprint( "err\n");
		return;
	}

	// ÀïÆ®½àÈ÷ÍÑ¥Ú¥Ã¥È¤Ê¤é¡¤¤½¤ì¤ÎÅÐÏ¿¤ò¾Ã¤¹¡£
	if( CHAR_getInt( talker, CHAR_DEFAULTPET) == selectpet) {
		CHAR_setInt( talker, CHAR_DEFAULTPET, -1);
		lssproto_KS_send( fd, -1, TRUE);
	}
	// ÍÂ¤±Æþ¤ìÍó¤Ë¡¤¥Ú¥Ã¥È¤Î¥¤¥ó¥Ç¥Ã¥¯¥¹¤ò¥³¥Ô¡¼
	CHAR_setDepotPetIndex( talker, emptyindex, petindex);
	// ¥Ú¥Ã¥ÈÍó¤«¤é¾Ã¤¹
	CHAR_setCharPet( talker, selectpet, -1);
	
	// ¥á¥Ã¥»¡¼¥¸¼èÆÀ
	NPC_MaxGoldOver( meindex, talker, NPC_PETSHOP_MSG_POOLTHANKS, token);
	// ¤ª¶â¸º¤é¤¹

	CHAR_DelGold( talker, NPC_GETPOOLCOST( talker) );

	// ¤ª¶â¥Ç¡¼¥¿Á÷¤ë
	CHAR_send_P_StatusString(talker,CHAR_P_STRING_GOLD);

	snprintf( szPet, sizeof( szPet ), "K%d", selectpet);
	// ¿·¤·¤¤¥Ú¥Ã¥È¥¹¥Æ¡¼¥¿¥¹Á÷¤ê¤Ä¤±¤ë
	CHAR_sendStatusString( talker, szPet );
	// ¥Ú¥Ã¥È¤òÍÂ¤±¤¿¥í¥°
	if( NPC_DepotPet_CheckRepeat( talker) == 0)  // ÀË¬d½s½X­«½ÆÃd
	{
		LogPet(
			CHAR_getChar( talker, CHAR_NAME ), /* ¥­¥ã¥éÌ¾ */
			CHAR_getChar( talker, CHAR_CDKEY ),
			CHAR_getChar( petindex, CHAR_NAME),
			CHAR_getInt( petindex, CHAR_LV),
			"pool(¦s¤JÃdª«­Ü®w)",
			CHAR_getInt( talker,CHAR_FLOOR),
			CHAR_getInt( talker,CHAR_X ),
			CHAR_getInt( talker,CHAR_Y ),
			CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
		);
	}
	else
	{
		CHAR_talkToCli( talker, -1, "³o¬O«DªkªºÃdª«³á!!", CHAR_COLORYELLOW);
	}

}	



void NPC_DepotPet_Get( int meindex, int talker, int select, char *token)
{
	
	int petindex;
	int selectpet;
	int emptyindex;
	char szPet[128];
	int workindex[CHAR_MAXPOOLPETHAVE];
	int i;
	int cnt;

	selectpet = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANT) ;
	petindex = CHAR_getDepotPetIndex( talker, selectpet);
	if( !CHAR_CHECKINDEX( petindex) ) return;
	emptyindex = CHAR_getCharPetElement( talker);
	if( emptyindex == -1 ) {
		fprint( "err\n");
		return;
	}
	CHAR_setCharPet( talker, emptyindex, petindex);
	CHAR_setChar( petindex, CHAR_OWNERCHARANAME, CHAR_getChar( talker, CHAR_NAME));
	CHAR_setDepotPetIndex( talker, selectpet, -1);
	NPC_MaxGoldOver( meindex, talker, NPC_PETSHOP_MSG_POOLTHANKS, token);
	snprintf( szPet, sizeof( szPet ), "K%d", emptyindex);
	CHAR_sendStatusString( talker, szPet );
	snprintf( szPet, sizeof( szPet), "W%d",emptyindex);
	CHAR_sendStatusString( talker, szPet );
	for( i = 0; i < CHAR_MAXDEPOTPETHAVE; i ++ ) {
		workindex[i] = -1;
	}
	cnt = 0;
	for( i = 0; i < CHAR_MAXDEPOTPETHAVE; i ++ ) {
		int petindex = CHAR_getDepotPetIndex( talker, i);
		if( CHAR_CHECKINDEX( petindex) ) {
			workindex[cnt++] = petindex;
		}
	}
	for( i = 0; i < CHAR_MAXDEPOTPETHAVE; i ++ ) {
		CHAR_setDepotPetIndex( talker, i, workindex[i]);
	}
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ),
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"draw(»â¨ú­Ü®wÃdª«)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);
#ifdef _PET_EVOLUTION
	if( CHAR_getInt( petindex, CHAR_FUSIONBEIT) == 1 &&
		CHAR_getInt( petindex, CHAR_FUSIONRAISE) > 0 ){//ÀË¬d¬O§_¬°¿Ä¦XÃd
		CHAR_setInt( petindex, CHAR_FUSIONTIMELIMIT, (int)time( NULL));
	}
#endif
}	

void NPC_DepotPet_AskGet(int meindex,int talker,int select,char *token)
{
	int depotindex;
	int petindex;
	char *petname;
	depotindex = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANT) * 5 + select - 1;
	CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,depotindex );
	petindex = CHAR_getDepotPetIndex( talker, depotindex);
	if( !CHAR_CHECKINDEX(petindex) ) return;
	petname = CHAR_getUseName( petindex);
	sprintf(token,"\n»â¦^[%s]", petname);	
}

int NPC_DepotPet_CheckRepeat( int charaindex)
{
	int petindex1, petindex2;
	int i, j, find, ret = FALSE;
	int workindex[CHAR_MAXDEPOTPETHAVE], cnt =0;

	for( i =0; i <(CHAR_MAXDEPOTPETHAVE-1); i++)
	{
		petindex1 = CHAR_getDepotPetIndex( charaindex, i);
		if( !CHAR_CHECKINDEX( petindex1)) continue;
		find = 0;
		for( j =(i+1); j <CHAR_MAXDEPOTPETHAVE; j++)
		{
			petindex2 = CHAR_getDepotPetIndex( charaindex, j);
			if( !CHAR_CHECKINDEX( petindex2)) continue;
			if( petindex1 == petindex2 ) continue;
			if( !strcmp( CHAR_getChar( petindex1, CHAR_UNIQUECODE),
						CHAR_getChar( petindex2, CHAR_UNIQUECODE) ) )
			{
				print(" ¦b­Ü®w§ä¨ì«DªkÃd¤F ");
				CHAR_setDepotPetIndex( charaindex, j, -1);
				NPC_DepotPet_CheckRepeat_del( charaindex, petindex2);
				find ++;
				ret = TRUE;
			}
		}
		if( find >0)
		{
			CHAR_setDepotPetIndex( charaindex, i, -1);
			NPC_DepotPet_CheckRepeat_del( charaindex, petindex1);
		}
	}

	for( i = 0; i < CHAR_MAXDEPOTPETHAVE; i ++ ) {
		workindex[i] = -1;
	}
	for( i = 0; i < CHAR_MAXDEPOTPETHAVE; i ++ ) {
		int pindex = CHAR_getDepotPetIndex( charaindex, i);
		if( CHAR_CHECKINDEX( pindex) ) {
			workindex[cnt++] = pindex;
		}
	}
	for( i = 0; i < CHAR_MAXDEPOTPETHAVE; i ++ ) {
		CHAR_setDepotPetIndex( charaindex, i, workindex[i]);
	}

	return ret;
}

int NPC_DepotPet_CheckRepeat_del( int charaindex, int petindex)
{
	int i;

	LogPet(
		CHAR_getChar( charaindex, CHAR_NAME ), /* ¥­¥ã¥éÌ¾ */
		CHAR_getChar( charaindex, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"del(²M°£­Ü®w¬~Ãd)",
		CHAR_getInt( charaindex,CHAR_FLOOR),
		CHAR_getInt( charaindex,CHAR_X ),
		CHAR_getInt( charaindex,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
		);

	CHAR_endCharOneArray( petindex );

}

// ÀË¬dµ¥¯Å¬O§_¨¬°÷
int NPC_DepotPet_CheckLevel(int meindex,int talker,int select,char *token)
{
	int petindex;
	int selectpet;

	if( !CHAR_CheckDepotPet( talker) )
		return FALSE;

	selectpet = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANT) ;
	petindex = CHAR_getDepotPetIndex( talker, selectpet);
	if( !CHAR_CHECKINDEX(petindex) ) return FALSE;
	
	//print("\n trans:%d char_lv:%d pet_lv:%d ",
	//	CHAR_getInt( talker, CHAR_TRANSMIGRATION),
	//	CHAR_getInt( talker, CHAR_LV),
	//	CHAR_getInt( petindex, CHAR_LV) );

	// ¦pªG¬O¦uÅ@Ã~§ï¬°´¶³qÃ~
	if( CHAR_getInt( petindex, CHAR_PETFAMILY) == 1 
		&& CHAR_getInt( talker, CHAR_FMLEADERFLAG) != 3 ) {
		CHAR_setInt( petindex, CHAR_PETFAMILY, 0);
	}

	if( CHAR_getWorkInt( talker, CHAR_PickAllPet) == TRUE ) { // ¸Ë³Æ¹¥Ã~§Ù«üªº¸Ü...
		return TRUE;
	}

	if( CHAR_getInt( talker, CHAR_TRANSMIGRATION) > 0 
		|| CHAR_getInt( talker, CHAR_LV)+5 >= CHAR_getInt( petindex, CHAR_LV) ) {
		return TRUE;
	}
	
	return FALSE;
	
}

#endif

