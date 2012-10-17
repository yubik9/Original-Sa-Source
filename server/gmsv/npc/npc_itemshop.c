#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "pet_skill.h"
#include "npc_eventaction.h"
#include "readmap.h"
#include "log.h"
#include "family.h"

#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨	
extern struct  FM_POINTLIST fmpointlist;                   // Æa±⁄æ⁄¬I
#endif
#define MAXSHOPITEM 33
static void NPC_ItemShop_selectWindow( int meindex, int talker, int num,int select);
void NPC_ItemShop_BuyMain(int meindex,int talker,int before );

#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨	
void NPC_GetItemList(int meindex, char *argstr,char * argtoken2);
void NPC_ItemStrStr(int meindex, int itemID,double rate,char *name,char *token2);
#else
void NPC_GetItemList(char *argstr,char * argtoken2);
	#ifdef _NEW_MANOR_LAW
	void NPC_ItemStrStr(int itemID,double rate,char *name,char *token2,int iCostFame,int iChangeItemCost);
	#else
	void NPC_ItemStrStr(int itemID,double rate,char *name,char *token2);
	#endif
#endif

BOOL NPC_SetNewItem(int meindex,int talker,char *data);
BOOL NPC_SellNewItem(int meindex,int talker,char *data);

void NPC_ItemShop_Menu(int meindex,int talker);

#ifdef _NPC_SHOPALTER01
int NPC_GetLimtItemList(int talker,char *argstr,char *token2,int sell, char* sLimt); //sLimt¶s©Ò™±Æa™∫™´´~•iß_ΩÊ•X,•H§Œª˘ø˙,ÆÊ¶°¨∞ 0|100|10|0|1|50|10|0|1|80|10|0| ,0:•iΩÊ,1:§£•iΩÊ,¶A®”º∆≠»¨∞ª˘ø˙,10¨∞table,0¨∞∞Ô≈|
#else
int NPC_GetLimtItemList(int talker,char *argstr,char *token2,int sell);
#endif

void NPC_ItemShop_SellMain(int meindex,int talker,int select);
int NPC_GetSellItemList(int itemindex,int flg,char *argstr,char *argtoken,int select,int sell);
#ifdef _NEW_MANOR_LAW
BOOL NPC_AddItemBuy(int meindex, int talker,int itemID,int kosuu,double rate,int iCostFame,int iChangeItemCost);
#else
BOOL NPC_AddItemBuy(int meindex, int talker,int itemID,int kosuu,double rate);
#endif
int NPC_SellItemstrsStr(int itemindex,int flg,double rate,char *argtoken,int select,int sell);
void NPC_LimitItemShop(int meindex,int talker,int select);
void NPC_ExpressmanCheck(int meindex,int talker);

enum{
	NPC_SHOP_WORK_NO 		= CHAR_NPCWORKINT1,
	NPC_SHOP_WORK_EV 		= CHAR_NPCWORKINT2,
	NPC_SHOP_WORK_EXPRESS	= CHAR_NPCWORKINT3,
};

		  

typedef struct {
	char	arg[32];
	int		type;
}NPC_Shop;


static NPC_Shop		TypeTable[] = {
	{ "FIST",		0 }, //Æ±
	{ "AXE",		1 }, //©Ú
	{ "CLUB",		2 }, //¥“¥Œ
	{ "SPEAR",		3},  //•Ÿ
	{ "BOW",		4},  //§}
	{ "SHIELD",		5},  //¨ﬁ
	{ "HELM",		6 }, //¿Y≤Ø
	{ "ARMOUR",		7 }, //≤Ø•“
	{ "BRACELET",	8},  //§‚≈N
	{ "ANCLET",		9 }, //ΩÔπ¢
	{ "NECKLACE",	10}, //∂µ¡Â
	{ "RING",		11}, //ßŸ´¸
	{ "BELT",		12}, //∏y±a
	{ "EARRING",	13}, //¶’¿Ù
	{ "NOSERING",	14}, //ªÛ¿Ù
	{ "AMULET",		15}, //≈@®≠≤≈
	{ "OTHER",		16}, 
	{ "BOOMERANG",	17}, //¶^§Oº–
	{ "BOUNDTHROW",	18}, 
	{ "BREAKTHROW",	19}, //ßÎ¬Y™´
#ifdef _ITEM_TYPETABLE
	{ "DISH",	20},
	{ "METAL",	21},
	{ "JEWEL",	22},
	{ "WARES",	23},
	{ "WBELT",	24},
	{ "WSHIELD", 25},
	{ "WSHOES",	26},
	{ "WGLOVE",	27},
	{ "ANGELTOKEN",	28},
	{ "HEROTOKEN",	29},
#endif
	{ "ACCESSORY",	30}, //∞t•Û
	{ "OFFENCE",	40}, //ß¿ª
	{ "DEFENCE",	50}, //®æøm

};

BOOL NPC_ItemShopInit( int meindex )
{

	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];

	CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEITEMSHOP );
	if(NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr)) == NULL) {
		print("GetArgStrErr");
		return FALSE;
	}
	if(strstr(argstr,"LIMITSHOP") != NULL) {
		CHAR_setWorkInt( meindex, NPC_SHOP_WORK_NO, 1);
	}else{
		CHAR_setWorkInt( meindex, NPC_SHOP_WORK_NO, 0);
	}
	if(strstr( argstr, "EVENT") != NULL) {
		CHAR_setWorkInt( meindex, NPC_SHOP_WORK_EV, 1);
	}else{
		CHAR_setWorkInt( meindex, NPC_SHOP_WORK_EV, 0);
	}
	if(strstr( argstr, "EXPRESS") != NULL) {
		CHAR_setWorkInt( meindex, NPC_SHOP_WORK_EXPRESS, 1);
	}else{
		CHAR_setWorkInt( meindex, NPC_SHOP_WORK_EXPRESS, 0);
	}
	
	return TRUE;
}

void NPC_ItemShopTalked( int meindex , int talker , char *szMes ,int color )
{
	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char	buff[1024];
	char	buf2[256];
	char 	token[1024];
	int 	i = 1;
	BOOL	sellonlyflg = FALSE;
	char	sellmsg[1024];

    if( CHAR_getInt( talker , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }

	if(NPC_Util_isFaceToFace( meindex, talker, 2) == FALSE) {//•ÊΩÕÆ…¿À¨d¨Oß_≠±πÔ≠±
		if( NPC_Util_CharDistance( talker, meindex ) > 1) return; //≠Y∂Z¬˜§j©Û1∏ı•X
	}

	//®˙±onpc∂RΩÊ≥]©w¿…§∫™∫§∫Æe,≠Y¨∞NULL,´hø˘ª~
    if(NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL){
    	print("itemshopGetArgStrErr");
    	return;
    }

	//®˙±oNPC•uØ‡ΩÊ™∫∞TÆß
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "sellonly_msg", token, sizeof( token))	!= NULL){
		sellonlyflg = TRUE; //NPC§£Ø‡∂R™±Æa™∫™F¶Ë
		strcpysafe(sellmsg, sizeof( sellmsg), token);//®“¶p:sellmsg™∫≠»•iØ‡¨O->®√§£¨O±M™˘¶¨∂R™F¶Ë™∫©±°C
	}

    //®˙±o™±Æa∂R™F¶Ë™∫´¸•O. buff¨∞§@¶Í∂R™F¶Ë´¸•O™∫¶r¶Í,®“¶p:∂R,¡ ∂R,∑P¡¬±z,kau,buy,menu,¡¬¡¬,∂R™F¶Ë,∑Ì™±Æa•¥•X≥o®«¶r•yÆ…,¥N•i•H∂R™F¶Ë§F
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "buy_msg",	buff, sizeof( buff)) != NULL ){
	    while(getStringFromIndexWithDelim(buff,",",i,buf2,sizeof(buf2)) != FALSE ){
			i++; //≠Y∂R™F¶Ë™∫´¸•O¶≥8≠”,i¥N∑|•[®Ï8
			if( strstr( szMes, buf2) != NULL) {
				if( CHAR_getWorkInt( meindex, NPC_SHOP_WORK_EV) == 0) {
					if( CHAR_getWorkInt( meindex, NPC_SHOP_WORK_NO) == 1) {
						if( sellonlyflg ) {
							NPC_ItemShop_selectWindow( meindex, talker, 3, -1);
							return;
						}
					}else{
						NPC_ItemShop_selectWindow( meindex, talker, 1, -1);
						return;
					}
				}else{
					if(CHAR_getWorkInt( meindex, NPC_SHOP_WORK_NO) == 1) {
						if( sellonlyflg) {
							NPC_ItemShop_selectWindow( meindex, talker, 3, -1);
							return;
						}
					}else{
						NPC_ItemShop_selectWindow( meindex, talker, 1, -1);
						return;
					}
		 			return;
		 	 	}
			}
		}
	}
	i=1;

    //®˙±o™±ÆaΩÊ™F¶Ë™∫´¸•O. buff¨∞§@¶ÍΩÊ™F¶Ë´¸•O™∫¶r¶Í,®“¶p:ΩÊ™F¶Ë,ΩÊ,sell,uru  ∑Ì™±Æa•¥•X≥o®«¶r•yÆ…,¥N•i•HΩÊ™F¶Ë§F
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "sell_msg", buff, sizeof( buff)) != NULL ){
	    while( getStringFromIndexWithDelim(buff,",", i,buf2,sizeof(buf2)) != FALSE ){
			i++;
			if(strstr(szMes,buf2) != NULL) {
				NPC_ItemShop_selectWindow( meindex, talker, 2, -1);
				return;
			}
		}
	}
	i = 1;

	//®‰•¶∞TÆß
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "other_msg", buff, sizeof( buff)) != NULL ){
	    while(getStringFromIndexWithDelim( buff, ",", i, buf2, sizeof( buf2)) != FALSE ){
			i++;
			if(strstr(szMes,buf2) != NULL) {
				if(NPC_Util_GetStrFromStrWithDelim( argstr, "hint_msg", 
				token, sizeof( token)) != NULL)
				{
					CHAR_talkToCli( talker, meindex, token, CHAR_COLORWHITE);
					return;
				}
			}
		}	
	}

	if(CHAR_getWorkInt( meindex, NPC_SHOP_WORK_NO) == 1) {
		if(CHAR_getWorkInt( meindex, NPC_SHOP_WORK_EV) == 1) {
			if( sellonlyflg) {
				CHAR_talkToCli( talker, meindex, sellmsg, CHAR_COLORWHITE);
				return;
			}
		}else{
			if( sellonlyflg) {
				NPC_ItemShop_selectWindow( meindex, talker, 3, -1);
				return;
			}
		}
	}else{
		if(CHAR_getWorkInt( meindex, NPC_SHOP_WORK_EV) == 1) {
			if( sellonlyflg) {
				CHAR_talkToCli( talker, meindex, sellmsg, CHAR_COLORWHITE);
				return;
			}
		}else{			
			if(CHAR_getWorkInt( meindex, NPC_SHOP_WORK_EXPRESS) == 1) {
				NPC_ExpressmanCheck( meindex, talker);
			}else{
				NPC_ItemShop_selectWindow( meindex, talker, 0, -1);
			}
		}
	}
				
}

static void NPC_ItemShop_selectWindow( int meindex, int talker, int num,int select)
{
	switch( num) {
	  case 0:
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);//∂«∞e™˜ø˙,≠Y100§∏ ∞e•X•h™∫∏ÍÆ∆ÆÊ¶°•iØ‡¨∞ P8Nz2|100|
		
		if(CHAR_getWorkInt( meindex, NPC_SHOP_WORK_EXPRESS) == 1 ) {
			if(CHAR_getWorkInt( meindex, NPC_SHOP_WORK_NO) ==0 ) {
				NPC_ExpressmanCheck( meindex, talker);//™¯§Ú∂Hß÷ªº 
			}
		}else if(CHAR_getWorkInt( meindex, NPC_SHOP_WORK_NO) == 1) {
		
		}else{
		  	NPC_ItemShop_Menu( meindex, talker);//øÔæ‹(∂R,ΩÊ,¬˜∂})™∫§pµ¯µ°	
		}
	  	break;

	  case 1://∂i§J∂Rµ¯µ°
		CHAR_sendStatusString( talker,"I");//∂«∞e™±Æa®≠§W©“¶≥™∫πD®„µπClient
	  	NPC_ItemShop_BuyMain( meindex, talker, select);
	  	break;

	  case 2://∂i§JΩÊµ¯µ°
		CHAR_sendStatusString( talker,"I");
	  	NPC_ItemShop_SellMain( meindex, talker, select);
	  	break;

	  case 3:
	  	NPC_LimitItemShop( meindex, talker, select);
	  	break;

	}
}

void NPC_ItemShopWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 3) {
		CHAR_send_P_StatusString( talkerindex, CHAR_P_STRING_GOLD);
		return;
	}

	makeStringFromEscaped( data);

	switch( seqno){

	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG:
		if(atoi( data) == 1 )	NPC_ItemShop_selectWindow(meindex, talkerindex, 1, -1);
		if(atoi( data) == 2)	NPC_ItemShop_selectWindow(meindex, talkerindex, 2, -1);
		if(atoi( data) == 3)	return;/*--≤ø§‚§∑§ §§--*/
		break;
	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG:
		if(NPC_SetNewItem(meindex , talkerindex, data) == TRUE) {
			NPC_ItemShop_selectWindow( meindex, talkerindex, 1, 0);
		}else{
			NPC_ItemShop_selectWindow( meindex, talkerindex ,0, -1);
		}

		break;
	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG:
		if(NPC_SellNewItem(meindex , talkerindex, data) == TRUE) {
			NPC_ItemShop_selectWindow( meindex, talkerindex, 2, 0);

		}else{
			NPC_ItemShop_selectWindow( meindex,  talkerindex, 0, -1);
		}

		break;
			
	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT:
		 if(select == WINDOW_BUTTONTYPE_YES) {
			NPC_ItemShop_selectWindow( meindex, talkerindex ,2, -1);

		}else  if(select == WINDOW_BUTTONTYPE_NO) {
			return;
		}else if(select == WINDOW_BUTTONTYPE_OK) {
				NPC_ItemShop_selectWindow( meindex, talkerindex, 2, -1);
		}
		break;
	
	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_EXPRESS:
		if(atoi(data) == 2) {
			NPC_ItemShop_selectWindow( meindex, talkerindex, 1, -1);
		}else if(atoi( data) == 4) {
			NPC_ItemShop_selectWindow( meindex, talkerindex, 2, -1);
		}
	}
}

void NPC_ItemShop_BuyMain(int meindex,int talker,int before )
{      
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char token[NPC_UTIL_GETARGSTR_BUFSIZE];
	int fd = getfdFromCharaIndex( talker);
    
	char token2[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buff2[256];
    char buff[256];
    
#ifdef _NPC_SHOPALTER01
	char sLimt[1024]; //¶s©Ò™±Æa™´´~•iß_ΩÊ•X§Œª˘ø˙
	char tokentemp[NPC_UTIL_GETARGSTR_BUFSIZE]; //ªPtoken•\Ø‡¨€¶P
	float fdata; //¬‡¥´sell_rate¨∞º∆≠»
#endif

	if(NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr)) == NULL) {
       	print("itemshop_GetArgStr_Err");
       	return;
    }
#ifdef _ADD_STATUS_2
	sprintf(token,"FAME|%d",CHAR_getInt(talker,CHAR_FAME)/100);
	lssproto_S2_send(fd,token);
#endif
	if(before != -1) {
		sprintf(token,"0|0");

		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN, 
				WINDOW_BUTTONTYPE_NONE, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);
	}else{

		NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "buy_main", buff2, sizeof( buff2));

		sprintf(token,"0|1|%d|%s|%s|%s|", CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG,
				CHAR_getChar( meindex, CHAR_NAME), buff, buff2);

		NPC_Util_GetStrFromStrWithDelim( argstr, "what_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "level_msg", buff2, sizeof( buff));

		snprintf( token2, sizeof( token2), "%s|%s", buff, buff2);
	
	    strncat( token, token2, sizeof( token));
			
		NPC_Util_GetStrFromStrWithDelim( argstr, "realy_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "itemfull_msg", buff2, sizeof( buff2));

		sprintf( token2, "|%s|%s", buff, buff2);
		strncat(token , token2,sizeof(token));
		strcpy(token2, "|");

#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨	
		NPC_GetItemList(meindex, argstr, token2);
#else
		NPC_GetItemList( argstr, token2);
#endif
		strncat( token, token2, sizeof( token));
	}

#ifdef _NPC_SHOPALTER01
	sprintf(tokentemp,"%s","");
	sprintf(sLimt,"%s","");
	NPC_GetLimtItemList( talker, argstr, tokentemp, -1, sLimt);//∏‘≤”™±Æa≠nΩÊ•X™∫πD®„∏ÍÆ∆
    fdata = atof(NPC_Util_GetStrFromStrWithDelim( argstr, "sell_rate", buff, sizeof( buff)));
	fdata *= 100; //≠Ï•ª∏ÍÆ∆¨∞§pº∆,Client∫›≥B≤z™∫¨∞æ„º∆,©“•H≠º•H100
	sprintf(token,"%suseritem|%d%s", token, (int)fdata, sLimt);//≠Ï•ª™∫¶r¶Í´·¶A•[§Wsell_rate,ªPsLimt∏ÍÆ∆
#endif

	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN, 
				WINDOW_BUTTONTYPE_NONE, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

}

#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨	
void NPC_GetItemList(int meindex, char *argstr,char *argtoken)
#else
void NPC_GetItemList(char *argstr,char *argtoken)
#endif
{
	int i = 1;
	int tmp;
	char *name ;
	char buff2[256];
#ifdef _NEW_MANOR_LAW
	char buff3[NPC_UTIL_GETARGSTR_LINEMAX];
	char buff4[256];
	char buff5[NPC_UTIL_GETARGSTR_LINEMAX];
	char buff6[256];
	int iCostFame = 0;
	int iChangeItemCost = 0;
#endif
	char buff[NPC_UTIL_GETARGSTR_LINEMAX];
	char token2[NPC_UTIL_GETARGSTR_BUFSIZE];
	double rate = 1.0;
	int loopcnt = 0;

	if(NPC_Util_GetStrFromStrWithDelim( argstr, "buy_rate", buff2, sizeof( buff2))
	 != NULL){
		rate = atof( buff2);
	}
#ifdef _NEW_MANOR_LAW
	memset(buff3,0,sizeof(buff3));
	memset(buff5,0,sizeof(buff5));
	if(NPC_Util_GetStrFromStrWithDelim(argstr,"CostFame",buff3,sizeof(buff3)) == NULL) iCostFame = -1;
	if(NPC_Util_GetStrFromStrWithDelim(argstr,"ChangeItemCost",buff5,sizeof(buff5)) == NULL) iChangeItemCost = -1;
#endif
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "ItemList", buff, sizeof( buff)) != NULL ){
	    while( getStringFromIndexWithDelim(buff,",",i,buff2,sizeof(buff2)) !=FALSE ) {
#ifdef _NEW_MANOR_LAW
				if(iCostFame > -1){
					if(getStringFromIndexWithDelim(buff3,",",i,buff4,sizeof(buff4)) != FALSE) iCostFame = atoi(buff4);
				}
				if(iChangeItemCost > -1){
					if(getStringFromIndexWithDelim(buff5,",",i,buff6,sizeof(buff6)) != FALSE) iChangeItemCost = atoi(buff6);
				}
#endif
			i++;
			if(strstr( buff2, "-") == NULL) {
			 	name = ITEM_getNameFromNumber( atoi(buff2));
				if(name == NULL) continue;
				loopcnt++;
				if(loopcnt == MAXSHOPITEM) break;

#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨
				NPC_ItemStrStr( meindex, atoi( buff2), rate, name, token2);
#else
	#ifdef _NEW_MANOR_LAW
				NPC_ItemStrStr( atoi( buff2), rate, name, token2,iCostFame,iChangeItemCost);
	#else
				NPC_ItemStrStr( atoi( buff2), rate, name, token2);
	#endif
#endif
	    		strncat( argtoken, token2, sizeof(token2));
			}else{
				int start;
				int end;
				getStringFromIndexWithDelim( buff2, "-", 1, token2, sizeof(token2));
				start = atoi( token2);
				getStringFromIndexWithDelim( buff2, "-", 2 ,token2, sizeof(token2));
				end = atoi( token2);
				if(start > end){
					tmp = start;
					start = end;
					end = tmp;
				}
				end++;
				for(; start < end ; start++ ) {
					/*--§ﬁ§∫Ãæ¡∞£«£≈£‘--*/

				 	name = ITEM_getNameFromNumber( start );
					if(name == NULL) continue;
					loopcnt++;
					if(loopcnt == MAXSHOPITEM) break;

#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨
					NPC_ItemStrStr( meindex, start, rate, name, token2);
#else
	#ifdef _NEW_MANOR_LAW
					NPC_ItemStrStr( start, rate, name, token2,iCostFame,iChangeItemCost);
	#else
					NPC_ItemStrStr( start, rate, name, token2);
	#endif
#endif
		    		strncat( argtoken, token2, sizeof(token2));
				}
			}
		}
	}
}

#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨
	void NPC_ItemStrStr(int meindex, int itemID,double rate,char *name,char *token2)
#else
	#ifdef _NEW_MANOR_LAW
		void NPC_ItemStrStr(int itemID,double rate,char *name,char *token2,int iCostFame,int iChangeItemCost)
	#else
		void NPC_ItemStrStr(int itemID,double rate,char *name,char *token2)
	#endif
#endif
{

	int gold;
	int level;
	int graNo;
	char info[1024];
	char escape[256];	

#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨(ßÔ§E§j≤¯∂ÈÆ…≠n≠◊ßÔ)
	float fm_tax=1.00;
	int npc_village;

	// ßÔ§E§j≤¯∂ÈÆ…≠n≠◊ßÔ
	switch( (int)( CHAR_getInt( meindex, CHAR_FLOOR)/100 ) )
	{
		case 10:
			npc_village = 0;
			break;
		case 20:
			npc_village = 1;
			break;			
		case 30:
			npc_village = 2;
			break;		
		case 40:
			npc_village = 3;
			break;
#ifdef _FAMILY_MANORNUM_CHANGE	// •[≤¯∂ÈÆ…≠n∞O±o•[
		case 50:
			npc_village = 4;
			break;	
		case 60:
			npc_village = 5;
			break;
		case 70:
			npc_village = 6;
			break;			
		case 80:
			npc_village = 7;
			break;		
		case 90:
			npc_village = 8;
			break;			
#endif
		default:
			npc_village = -1;
			break;
	}
	if( npc_village < 0 || npc_village > 9 )
		return;
	// µ|≤v±±®Ó
	if( npc_village >= 0){
		fm_tax +=  (float)fmpointlist.fm_tax[ npc_village ] / 100;
	}
	if (fm_tax < 1)		fm_tax = 1 ;

	gold =(int)( (float)ITEM_getcostFromITEMtabl(itemID) * fm_tax );

#else
	#ifdef _NEW_MANOR_LAW
	if(iChangeItemCost > -1) gold = iChangeItemCost;
	else 
	#endif
	gold  = ITEM_getcostFromITEMtabl( itemID);
#endif
	level = ITEM_getlevelFromITEMtabl( itemID);
	graNo = ITEM_getgraNoFromITEMtabl( itemID);
	strcpy(escape,ITEM_getItemInfoFromNumber( itemID));
	gold=(int)(gold * rate);

	makeEscapeString( escape, info, sizeof( info));
	makeEscapeString( name, escape, sizeof( escape));
#ifdef _NEW_MANOR_LAW
	sprintf(token2,"%s|0|%d|%d|%d|%s|%d|",escape,level,gold,graNo,info,iCostFame < 0 ? -1:iCostFame/100);
#else
	sprintf( token2, "%s|0|%d|%d|%d|%s|", escape, level, gold, graNo, info);
#endif
}

BOOL NPC_SetNewItem(int meindex,int talker,char *data)
{

	char buf[1024];
	char buff2[128];
#ifdef _NEW_MANOR_LAW
	char buff3[1024];
	char buff4[128];
	char buff5[1024];
	char buff6[128];
	int iCostFame = 0;
	int iChangeItemCost = 0;
#endif
	int i = 1, j = 1;
	int select;
	int kosuu = 0;
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	double rate = 1.0;
	int gold = 0;		
	int kosuucnt = 0;
	int itemindex;

	/*-- ÷§√§∆§≠§ø•«°º•ø§Ú•ª•Ï•Ø•»§»∏ƒøÙ§À ¨≤Ú--*/
	getStringFromIndexWithDelim( data, "|", 1, buf, sizeof( buf));
	select = atoi(buf);
	if(select == 0) return FALSE;
	getStringFromIndexWithDelim( data, "|", 2, buf, sizeof( buf));
	kosuu = atoi(buf);
	if( kosuu <= 0 ) return FALSE;
	
	/*--∏ƒøÙ§Œ•¡•ß•√•ØÀ‹≈ˆ§À¡¥…Ù∆˛§Î§´°©-*/
	for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ) {
		itemindex = CHAR_getItemIndex( talker , i );
		if( !ITEM_CHECKINDEX( itemindex) ) {
			kosuucnt++;
		}
	}

	/*-- ÷§√§∆ÕË§ø∏ƒøÙ§Œ ˝§¨º¬∫›§Œ∏ƒøÙ° •µ°º•–¬¶°À§Ë§Í¬ø§§§»§™§´§∑§§§Œ§«--*/
	/*--•µ°º•–°º¬¶§Œ ˝§Ú∆˛§Ï§Î--*/
	if( kosuucnt < kosuu) kosuu = kosuucnt;
		
	/*--•º•Ì§ŒæÏπÁ§œ•®•È°º--*/
	if(kosuucnt == 0 ) return FALSE;

	i = 1;

	/*--§™≈π§Œ•’•°•§•ÎÃµ§§ÀÙ§œ•’•°•§•Î§¨≥´§±§ §´§√§ø§»§≠§œΩ™Œª--*/
	if(NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr)) == NULL) {
   	print("shop_GetArgStr_Err");
   	return FALSE;
	}

	/*---•Ï°º•»§ÚºË∆¿° § §±§Ï§–1.0)-*/
	if(NPC_Util_GetStrFromStrWithDelim( argstr, "buy_rate", buf, sizeof( buf)) != NULL) {
		rate= atof( buf);
	}

#ifdef _NEW_MANOR_LAW
	memset(buff3,0,sizeof(buff3));
	memset(buff5,0,sizeof(buff5));
	if(NPC_Util_GetStrFromStrWithDelim(argstr,"CostFame",buff3,sizeof(buff3)) == NULL) iCostFame = -1;
	if(NPC_Util_GetStrFromStrWithDelim(argstr,"ChangeItemCost",buff5,sizeof(buff5)) == NULL) iChangeItemCost = -1;
#endif

	/*--•¢•§•∆•‡§Œƒ…≤√§Úπ‘§¶§»§≥§Ì-*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "ItemList", buf, sizeof( buf)) != NULL ){
		while(getStringFromIndexWithDelim(buf , "," , j, buff2, sizeof(buff2)) != FALSE ){
#ifdef _NEW_MANOR_LAW
			if(iCostFame > -1){
				if(getStringFromIndexWithDelim(buff3,",",j,buff4,sizeof(buff4)) != FALSE) iCostFame = atoi(buff4);
			}
			if(iChangeItemCost > -1){
				if(getStringFromIndexWithDelim(buff5,",",j,buff6,sizeof(buff6)) != FALSE) iChangeItemCost = atoi(buff6);
			}
#endif
			j++;
			/*--  "-"§¨¥ﬁ§ﬁ§Ï§∆§§§Î§´§…§¶§´--*/
			if(strstr( buff2, "-") == NULL) {
				if( ITEM_getcostFromITEMtabl(atoi(buff2)) !=-1) {
					if ( i == select) {
						/*---•¢•§•∆•‡§Œ∫Ó¿Æ---*/
						/*--∏ƒøÙ ¨∫Ó¿Æ--*/
#ifdef _NEW_MANOR_LAW
						if(NPC_AddItemBuy(meindex, talker,atoi(buff2),kosuu,rate,iCostFame,iChangeItemCost) != TRUE)
#else
						if(NPC_AddItemBuy(meindex, talker,atoi(buff2),kosuu,rate) != TRUE)
#endif
						{
							return FALSE;
						}
						return TRUE;
					}
					i++;		
				}
			}else{
				/*--•¢•§•∆•‡§¨°…15-25°…§Œ∑¡§«¡˜§È§Ï§øæÏπÁ--*/
				int start;
				int end;

				/* "-"§«∂Ë¿⁄§È§Ï§øªœ§·§ŒøÙ√Õ§»∏Â§ŒøÙ√Õ§ÚºË∆¿--*/
				getStringFromIndexWithDelim( buff2, "-", 1, argstr, sizeof(argstr));
				start = atoi( argstr);
				getStringFromIndexWithDelim( buff2, "-", 2 ,argstr, sizeof(argstr));
				end = atoi( argstr);
				end++;

				/*--»÷πÊ§¨µ’§À§ §√§∆§§§ø§È°¢∆˛§Ï¬ÿ§®§Î**/
				if(start > end){
					gold = start;
					start = end;
					end = gold;
				}

				/*--"-"§«∂Ë¿⁄§È§Ï§ø ¨§Œ•¢•§•∆•‡§Úæ Û§Ú∆¿§Î--*/
				for(; start < end ; start++ ) {
					if( ITEM_getcostFromITEMtabl( start) != -1) {
						if ( i == select) {
							/*---•¢•§•∆•‡§Œ∫Ó¿Æ---*/
							/*--∏ƒøÙ ¨∫Ó¿Æ--*/
#ifdef _NEW_MANOR_LAW
							if(NPC_AddItemBuy(meindex, talker, start, kosuu, rate,iCostFame,iChangeItemCost) != TRUE)
#else
							if(NPC_AddItemBuy(meindex, talker, start, kosuu, rate) != TRUE)
#endif
							{
								return FALSE;
							}
							return TRUE;
						}
						i++;
					}
				}
			}
		}
	}

	return FALSE;

}

/*---------------------------------------------
 *•¢•§•∆•‡§Œƒ…≤√§Úπ‘§¶
 *--------------------------------------------*/
#ifdef _NEW_MANOR_LAW
BOOL NPC_AddItemBuy(int meindex, int talker,int itemID,int kosuu,double rate,int iCostFame,int iChangeItemCost)
#else
BOOL NPC_AddItemBuy(int meindex, int talker,int itemID,int kosuu,double rate)
#endif
{

	int itemindex;
	int i;
	int gold;
	int ret;
	int maxgold;
#ifdef _NEW_MANOR_LAW
	int iTotalCostFame = -1;
#endif
#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨(ßÔ§E§j≤¯∂ÈÆ…≠n≠◊ßÔ)
	float fm_tax=1;
	int npc_village;

	gold = ITEM_getcostFromITEMtabl( itemID);
	switch( (int)( CHAR_getInt( meindex, CHAR_FLOOR)/100 ) ){
		case 10:
			npc_village = 0;
			break;
		case 20:
			npc_village = 1;
			break;			
		case 30:
			npc_village = 2;
			break;		
		case 40:
			npc_village = 3;
			break;
#ifdef _FAMILY_MANORNUM_CHANGE
		case 50:
			npc_village = 4;
			break;	
		case 60:
			npc_village = 5;
			break;
		case 70:
			npc_village = 6;
			break;			
		case 80:
			npc_village = 7;
			break;		
		case 90:
			npc_village = 8;
			break;			
#endif
		default:
			npc_village = -1;
			break;
	}
	// µ|≤v±±®Ó
	if( npc_village > 0)
		fm_tax += (float)fmpointlist.fm_tax[ npc_village ] / 100;
	if (fm_tax < 1)		fm_tax = 1 ;
	gold = (int)( (float)gold * rate * fm_tax ) ;
#else
	#ifdef _NEW_MANOR_LAW
	if(iChangeItemCost > -1) gold = iChangeItemCost;
	else
	#endif
	gold = ITEM_getcostFromITEMtabl( itemID);
	gold = (int)(gold * rate);
#endif

	maxgold = gold * kosuu;
	if(CHAR_getInt( talker, CHAR_GOLD) < maxgold ) return FALSE;
#ifdef _NEW_MANOR_LAW
	if(iCostFame > 0){
		iTotalCostFame= iCostFame * kosuu;
		if(CHAR_getInt(talker,CHAR_FAME) < iTotalCostFame) return FALSE;
	}
#endif

#ifdef _ADD_FAMILY_TAX			   // WON ADD ºW•[≤¯∂Èµ|¶¨		
	if( addNpcFamilyTax( meindex, talker, maxgold * (fm_tax - 1) ) )
#else
	if( addNpcFamilyTax( meindex, talker, maxgold*0.4 ) )
#endif
		print(" FamilyTaxDone! ");
	else
		print(" FamilyTaxError!");
	for(i = 0 ; i < kosuu ; i++){
		itemindex = ITEM_makeItemAndRegist( itemID);
		
		if(itemindex == -1) return FALSE;
		ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
		if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
			print( "npc_itemshop.c: additem error itemindex[%d]\n", itemindex);
			ITEM_endExistItemsOne( itemindex);
			return FALSE;
		}
		CHAR_sendItemDataOne( talker, ret);
	}
	CHAR_DelGold( talker, maxgold);
#ifdef _NEW_MANOR_LAW
	if(iTotalCostFame > 0){
		CHAR_setInt(talker,CHAR_FAME,CHAR_getInt(talker,CHAR_FAME) - iTotalCostFame);
		if(CHAR_getInt(talker,CHAR_FMINDEX) != -1 && (strcmp(CHAR_getChar(talker,CHAR_FMNAME),""))){
			int fd = getfdFromCharaIndex(talker);
			char	buf[256];
			sprintf(buf,"%d",CHAR_getInt(talker,CHAR_FAME));
			saacproto_ACFixFMData_send(acfd,
				CHAR_getChar(talker,CHAR_FMNAME),
				CHAR_getInt(talker,CHAR_FMINDEX),
				CHAR_getWorkInt(talker,CHAR_WORKFMINDEXI),
				FM_FIX_FAME,buf,"",
				CHAR_getWorkInt(talker,CHAR_WORKFMCHARINDEX),
				CONNECT_getFdid(fd));
			LogFMFameShop(CHAR_getChar(talker,CHAR_FMNAME),
										CHAR_getChar(talker,CHAR_CDKEY),
										CHAR_getChar(talker,CHAR_NAME),
										CHAR_getInt(talker,CHAR_FAME),
										iTotalCostFame);
		}
	}
#endif
	//CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
	return TRUE;

}

//øÔæ‹ (∂R,ΩÊ,•X•h) ™∫§pµ¯µ°
void NPC_ItemShop_Menu(int meindex,int talker)
{	
	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char	token[NPC_UTIL_GETARGSTR_LINEMAX];
	char buff[256];
	int fd = getfdFromCharaIndex( talker);

#ifdef _NPC_SHOPALTER01
	char sLimt[1024]; //¶s©Ò™±Æa™´´~•iß_ΩÊ•X§Œª˘ø˙
	char tokentemp[NPC_UTIL_GETARGSTR_BUFSIZE]; //ªPtoken•\Ø‡¨€¶P
	float fdata; //¬‡¥´sell_rate¨∞º∆≠»
#endif

    //argstr®˙±oæ„≠”≥]©w¿…™∫∞TÆß: ®“¶p->buy_rate:1.0|sell_rate:0.2|buy_msg:∂R,¡ ∂R,∑P¡¬±z,kau,buy............. (§§∂°™∫§¿ÆÊ∏π¨O≈™§JÆ…•[§J™∫)
    if(NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr)) == NULL) {
		print("shop_GetArgStr_Err");
       	return;
    }
    
	//token¨∞µ¯µ°§W≠±™∫title§Â¶r  ®“¶p: ¬ƒ©i¶N∫∏™∫®æ®„©±|≈w™Ô•˙¡{
    NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buff, sizeof( buff));
#ifdef _NEW_MANOR_LAW
	snprintf(token, sizeof(token),"%s|%s|%d",CHAR_getChar(meindex,CHAR_NAME),buff,CHAR_getInt(talker,CHAR_FAME)/100);
#else
	snprintf(token, sizeof(token),"%s|%s",CHAR_getChar( meindex, CHAR_NAME), buff);
#endif

#ifdef _NPC_SHOPALTER01
	sprintf(tokentemp,"%s","");
	sprintf(sLimt,"%s","");
	NPC_GetLimtItemList( talker, argstr, tokentemp, -1, sLimt);//∏‘≤”™±Æa≠nΩÊ•X™∫πD®„∏ÍÆ∆
    fdata = atof(NPC_Util_GetStrFromStrWithDelim( argstr, "sell_rate", buff, sizeof( buff)));
	fdata *= 100; //≠Ï•ª∏ÍÆ∆¨∞§pº∆,Client∫›≥B≤z™∫¨∞æ„º∆,©“•H≠º•H100
	sprintf(token,"%suseritem|%d%s", token, (int)fdata, sLimt);//≠Ï•ª™∫¶r¶Í´·¶A•[§Wsell_rate,ªPsLimt∏ÍÆ∆
#endif
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMENU, 
				WINDOW_BUTTONTYPE_NONE, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

}

void NPC_ItemShop_SellMain(int meindex,int talker,int before)
{
	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char	token[NPC_UTIL_GETARGSTR_BUFSIZE];
	int fd = getfdFromCharaIndex( talker);

	//®˙±onpc≥]©w∏ÍÆ∆
    if(NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr)) == NULL) {
       	print("shop_GetArgStr_Err");
       	return;
    }

#ifdef _ADD_STATUS_2
	sprintf(token,"FAME|%d",CHAR_getInt(talker,CHAR_FAME)/100);
	lssproto_S2_send(fd,token);
#endif

	if(before != -1) {
		sprintf(token,"1|0");
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN
							+CHAR_getWorkInt(meindex,NPC_SHOP_WORK_NO), 
					WINDOW_BUTTONTYPE_NONE, 
					CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					token);
	
	}else{

		char token2[NPC_UTIL_GETARGSTR_BUFSIZE];
		char buff2[256];
	   	char buff[256];

		NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "sell_main", buff2, sizeof( buff));
		sprintf( token, "1|1|%d|%s|%s|%s|", CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG,
				CHAR_getChar( meindex, CHAR_NAME), buff, buff2);

		NPC_Util_GetStrFromStrWithDelim( argstr, "stone_msg", buff, sizeof( buff));

		if(CHAR_getWorkInt(meindex,NPC_SHOP_WORK_EXPRESS) == 1 ) {
			NPC_Util_GetStrFromStrWithDelim( argstr, "exrealy_msg", buff2, sizeof(buff2));
		}else{
			NPC_Util_GetStrFromStrWithDelim( argstr, "realy_msg", buff2, sizeof( buff2));
		}
		sprintf( token2,"%s|%s|", buff, buff2);
#ifdef _NPC_SHOPALTER01
		NPC_GetLimtItemList( talker,argstr, token2, -1, NULL);//∏‘≤”™±Æa≠nΩÊ•X™∫πD®„∏ÍÆ∆
#else
		NPC_GetLimtItemList( talker,argstr, token2, -1);//∏‘≤”™±Æa≠nΩÊ•X™∫πD®„∏ÍÆ∆
#endif	
		strncat( token, token2, sizeof( token));

		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN+
					CHAR_getWorkInt(meindex,NPC_SHOP_WORK_NO), 
					WINDOW_BUTTONTYPE_NONE, 
					CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					token);

	}
}
#ifdef _NPC_SHOPALTER01
int NPC_GetLimtItemList(int talker, char *argstr, char* token2,int sell, char* sLimt)//sLimt:™±Æa•iΩÊ™´´~™∫¶r¶Í
#else
int NPC_GetLimtItemList(int talker, char *argstr, char* token2,int sell)
#endif
{

	char token[NPC_UTIL_GETARGSTR_LINEMAX];
	char buff[NPC_UTIL_GETARGSTR_LINEMAX];
	char token3[NPC_UTIL_GETARGSTR_LINEMAX];
	int k = 0 , i = 1 , j = 0;
	int imax;
	int itemtype = 0;
	int itemindex;
	int okflg = 0;
	char buf[256];
	int flg=0;
	int cost;

#ifdef _NPC_SHOPALTER01
    char sbuf[256]; //®˙±osLimt´e,•Œsbuf®˙±o®C§@∂µ∏ÍÆ∆,¶A¶s§JsLimt
#endif


	if(sell == -1 ){
		i = CHAR_STARTITEMARRAY;
		imax = CHAR_MAXITEMHAVE;
		flg = -1;
	}else{
		i= sell;
		imax= sell + 1;
		flg = 1;
	}	

	for( ; i < imax ; i++ ){
		okflg=0;
		itemindex = CHAR_getItemIndex( talker , i );
		
		if( ITEM_CHECKINDEX( itemindex) ){
			if( NPC_Util_GetStrFromStrWithDelim( argstr,"LimitItemType", buff, sizeof( buff)) != NULL ){
				k = 1;
				while(getStringFromIndexWithDelim(buff , "," , k, token, sizeof(token)) != FALSE ){
#ifdef _ITEM_TYPETABLE
					int cmpmaxitem = sizeof(TypeTable)/sizeof(TypeTable[0]);
#endif
					k++;
#ifdef _ITEM_TYPETABLE
					for(j = 0 ; j < cmpmaxitem ; j++){
#else
					for(j = 0 ; j < ITEM_CATEGORYNUM+3 ; j++){
#endif
						if(strcmp( TypeTable[ j].arg  , token) == 0 ) {
							itemtype = TypeTable[ j].type;
							if(ITEM_getInt(itemindex,ITEM_TYPE) == itemtype) {
								cost = NPC_GetSellItemList(itemindex,0,argstr,token3,i,sell);
								if(cost != -1) return cost;
								strncat( token2, token3, sizeof( token3));
#ifdef _NPC_SHOPALTER01
								print("Change->itemtype = %d",itemtype);
								print("Change->token2:%s\n",token2);
								print("Change->token3:%s\n",token3);
								if( sLimt )
								{
									if( getStringFromIndexWithDelim(token3 , "|" , 2, sbuf, sizeof( sbuf)) ){
										sprintf(sLimt,"%s|%s",sLimt,sbuf);
										if( getStringFromIndexWithDelim(token3 , "|" , 3, sbuf, sizeof( sbuf)) ){
											sprintf(sLimt,"%s|%s",sLimt,sbuf);    
											if( getStringFromIndexWithDelim(token3 , "|" , 6, sbuf, sizeof( sbuf)) ){
												sprintf(sLimt,"%s|%s",sLimt,sbuf);  
												if( getStringFromIndexWithDelim(token3 , "|" , 7, sbuf, sizeof( sbuf)) ){
													sprintf(sLimt,"%s|%s",sLimt,sbuf);    
												}
											}
										}
									}
								}
#endif
								okflg = 1;
							}else if(itemtype == 30){
								if( 8 <= ITEM_getInt(itemindex,ITEM_TYPE) 
									&& (ITEM_getInt(itemindex,ITEM_TYPE) <= 15) ){
									cost = NPC_GetSellItemList(itemindex,0,argstr,token3,i,sell);
									if(cost != -1) return cost;
									strncat(token2,token3,sizeof(token3));
#ifdef _NPC_SHOPALTER01
									print("Change->itemtype = 30");
									print("Change->token2:%s\n",token2);
									print("Change->token3:%s\n",token3);
									if( sLimt )
									{
										if( getStringFromIndexWithDelim(token3 , "|" , 2, sbuf, sizeof( sbuf)) ){
											sprintf(sLimt,"%s|%s",sLimt,sbuf);
											if( getStringFromIndexWithDelim(token3 , "|" , 3, sbuf, sizeof( sbuf)) ){
												sprintf(sLimt,"%s|%s",sLimt,sbuf);    
												if( getStringFromIndexWithDelim(token3 , "|" , 6, sbuf, sizeof( sbuf)) ){
													sprintf(sLimt,"%s|%s",sLimt,sbuf); 
													if( getStringFromIndexWithDelim(token3 , "|" , 7, sbuf, sizeof( sbuf)) ){
														sprintf(sLimt,"%s|%s",sLimt,sbuf);    
													}
												}
											}
										}
									}
#endif
									okflg = 1;
								}
							}else if(itemtype == 40){
								if(( 0 <= ITEM_getInt(itemindex,ITEM_TYPE) 
									&& (ITEM_getInt(itemindex,ITEM_TYPE) <= 4)) 
									|| (17 <= ITEM_getInt(itemindex,ITEM_TYPE) 
									&& (ITEM_getInt(itemindex,ITEM_TYPE) <= 19))) {
									cost = NPC_GetSellItemList(itemindex,0,argstr,token3,i,sell);
									if(cost != -1) return cost;
									strncat(token2,token3,sizeof(token3));
#ifdef _NPC_SHOPALTER01
									print("Change->itemtype = 40");
									print("Change->token2:%s\n",token2);
									print("Change->token3:%s\n",token3);
									if( sLimt )
									{
										if( getStringFromIndexWithDelim(token3 , "|" , 2, sbuf, sizeof( sbuf)) ){
											sprintf(sLimt,"%s|%s",sLimt,sbuf);
											if( getStringFromIndexWithDelim(token3 , "|" , 3, sbuf, sizeof( sbuf)) ){
												sprintf(sLimt,"%s|%s",sLimt,sbuf);    
												if( getStringFromIndexWithDelim(token3 , "|" , 6, sbuf, sizeof( sbuf)) ){
													sprintf(sLimt,"%s|%s",sLimt,sbuf); 
													if( getStringFromIndexWithDelim(token3 , "|" , 7, sbuf, sizeof( sbuf)) ){
														sprintf(sLimt,"%s|%s",sLimt,sbuf);    
													}
												}
											}
										}
									}
#endif
									okflg = 1;
								}
							}else if(itemtype == 50){
								if( 5 <= ITEM_getInt(itemindex,ITEM_TYPE) 
									&& (ITEM_getInt(itemindex,ITEM_TYPE) <= 7) ){
									cost = NPC_GetSellItemList(itemindex,0,argstr,token3,i,sell);
									if(cost != -1) return cost;
									strncat(token2,token3,sizeof(token3));
#ifdef _NPC_SHOPALTER01
									print("Change->itemtype = 50");
									print("Change->token2:%s\n",token2);
									print("Change->token3:%s\n",token3);
									if( sLimt )
									{
										if( getStringFromIndexWithDelim(token3 , "|" , 2, sbuf, sizeof( sbuf)) ){
											sprintf(sLimt,"%s|%s",sLimt,sbuf);
											if( getStringFromIndexWithDelim(token3 , "|" , 3, sbuf, sizeof( sbuf)) ){
												sprintf(sLimt,"%s|%s",sLimt,sbuf);    
												if( getStringFromIndexWithDelim(token3 , "|" , 6, sbuf, sizeof( sbuf)) ){
													sprintf(sLimt,"%s|%s",sLimt,sbuf);    
													if( getStringFromIndexWithDelim(token3 , "|" , 7, sbuf, sizeof( sbuf)) ){
														sprintf(sLimt,"%s|%s",sLimt,sbuf);    
													}
												}
											}
										}
									}
#endif
									okflg = 1;
								}
							}
							break;
						}
					}
					
					if(okflg == 1) break;
				}
			}
			if( (NPC_Util_GetStrFromStrWithDelim( argstr, "LimitItemNo", buff,sizeof( buff))!= NULL) && okflg == 0 ){
				k = 1;
				while(getStringFromIndexWithDelim(buff , "," , k, token, sizeof(token)) != FALSE ){
					k++;
					if(strstr( token, "-")==NULL && strcmp(token,"") != 0) {
						if(ITEM_getInt(itemindex,ITEM_ID) == atoi(token)) {
							cost = NPC_GetSellItemList(itemindex,0,argstr,token3,i,sell);
							if(cost != -1) return cost;
							strncat(token2,token3,sizeof(token3));
#ifdef _NPC_SHOPALTER01
							print("Change->1\n");
							print("Change->token2:%s\n",token2);
							print("Change->token3:%s\n",token3);
							if( sLimt )
							{
								if( getStringFromIndexWithDelim(token3 , "|" , 2, sbuf, sizeof( sbuf)) ){
									sprintf(sLimt,"%s|%s",sLimt,sbuf);
									if( getStringFromIndexWithDelim(token3 , "|" , 3, sbuf, sizeof( sbuf)) ){
										sprintf(sLimt,"%s|%s",sLimt,sbuf);    
										if( getStringFromIndexWithDelim(token3 , "|" , 6, sbuf, sizeof( sbuf)) ){
											sprintf(sLimt,"%s|%s",sLimt,sbuf);    
											if( getStringFromIndexWithDelim(token3 , "|" , 7, sbuf, sizeof( sbuf)) ){
												sprintf(sLimt,"%s|%s",sLimt,sbuf);    
											}
										}
									}
								}
							}
#endif
							okflg=1;
						}
					}else if (strstr( token, "-") != NULL){
						int start, end;
						int work;
						
						if( getStringFromIndexWithDelim( token, "-", 1, buf, sizeof(buf)) == FALSE )
							return -1;
						start = atoi( buf);
						if( getStringFromIndexWithDelim( token, "-", 2 ,buf, sizeof(buf)) == FALSE )
							return -1;
						end = atoi( buf);
						
						if(start > end){
							work = start;
							start = end;
							end = work;
						}
						
						end++;
						if( (start <= ITEM_getInt(itemindex,ITEM_ID)) && (ITEM_getInt(itemindex,ITEM_ID) < end) ){
							cost = NPC_GetSellItemList(itemindex,0,argstr,token3,i,sell);
							if(cost != -1) return cost;
							strncat(token2,token3,sizeof(token3));
#ifdef _NPC_SHOPALTER01
							print("Change->2\n");
							print("Change->token2:%s\n",token2);
							print("Change->token3:%s\n",token3);
							if( sLimt )
							{
								if( getStringFromIndexWithDelim(token3 , "|" , 2, sbuf, sizeof( sbuf)) ){
									sprintf(sLimt,"%s|%s",sLimt,sbuf);
									if( getStringFromIndexWithDelim(token3 , "|" , 3, sbuf, sizeof( sbuf)) ){
										sprintf(sLimt,"%s|%s",sLimt,sbuf);    
										if( getStringFromIndexWithDelim(token3 , "|" , 6, sbuf, sizeof( sbuf)) ){
											sprintf(sLimt,"%s|%s",sLimt,sbuf);
											if( getStringFromIndexWithDelim(token3 , "|" , 7, sbuf, sizeof( sbuf)) ){
												sprintf(sLimt,"%s|%s",sLimt,sbuf);    
											}
										}
									}
								}
							}
#endif
							okflg = 1;
						}
					}
				}
			}
			
			if(okflg == 0) {
				cost = NPC_GetSellItemList(itemindex, 1, argstr, token3, i, sell);
				if(sell != -1) return -1;
				strncat( token2, token3, sizeof( token3));
#ifdef _NPC_SHOPALTER01
				print("Change->4\n");
				print("Change->token2:%s\n",token2);
				print("Change->token3:%s\n",token3);
				if( sLimt )
				{
					if( getStringFromIndexWithDelim(token3 , "|" , 2, sbuf, sizeof( sbuf)) ){
						sprintf(sLimt,"%s|%s",sLimt,sbuf);
						if( getStringFromIndexWithDelim(token3 , "|" , 3, sbuf, sizeof( sbuf)) ){
							sprintf(sLimt,"%s|%s",sLimt,sbuf);    
							if( getStringFromIndexWithDelim(token3 , "|" , 6, sbuf, sizeof( sbuf)) ){
								sprintf(sLimt,"%s|%s",sLimt,sbuf);  
								if( getStringFromIndexWithDelim(token3 , "|" , 7, sbuf, sizeof( sbuf)) ){
									sprintf(sLimt,"%s|%s",sLimt,sbuf);    
								}
							}
						}
					}
				}
#endif
			}
			
		}
	}
	return -1;
}



/*----------------------------------------------------------

	•Ø•È•§•¢•Û•»§À¡˜øÆ§π§Î•◊•Ì•»•≥•Î§Œ∫Ó¿Æ

 *----------------------------------------------------------*/
int NPC_GetSellItemList(int itemindex,int flg, char *argstr,char *argtoken,int select,int sell)
{
	char buff[256];
	double rate = 0.2;
	char buff2[256];
	char buff3[64];
	int k = 1;
	int cost = -1;

	if(NPC_Util_GetStrFromStrWithDelim( argstr,"special_item",buff, sizeof( buff)) != NULL){
		if(NPC_Util_GetStrFromStrWithDelim( argstr,"special_rate",buff2, sizeof( buff2)) != NULL ){
			rate = atof(buff2);
		}else{
			rate = 1.2;
		}
		
		while(getStringFromIndexWithDelim(buff , "," , k, buff2, sizeof(buff2)) !=FALSE ){
			k++;
			if(strstr( buff2, "-") == NULL && strcmp(buff2,"") != 0) {
				if(ITEM_getInt(itemindex,ITEM_ID) == atoi(buff2)){
					cost = NPC_SellItemstrsStr( itemindex,0, rate, argtoken,select,sell);
					return cost;
				}
			}else if (strstr( buff2, "-") != NULL){
				int start;
				int end;
				int work;
				getStringFromIndexWithDelim( buff2, "-", 1, buff3, sizeof(buff3));
				start = atoi( buff3);
				getStringFromIndexWithDelim( buff2, "-", 2 ,buff3, sizeof(buff3));
				end = atoi( buff3);

				if(start > end){
					work = start;
					start = end;
					end = work;
				}
				end++;
				if( (start <= ITEM_getInt(itemindex,ITEM_ID)) && (ITEM_getInt(itemindex,ITEM_ID) < end)	){
					cost = NPC_SellItemstrsStr( itemindex,0, rate, argtoken,select,sell);
					return cost;
				}
			}
		}
	}
	if( NPC_Util_GetStrFromStrWithDelim( argstr,"sell_rate",buff, sizeof( buff)) != NULL ){
		rate = atof(buff);
		cost = NPC_SellItemstrsStr( itemindex, flg ,rate, argtoken,select,sell);
		return cost;
	}

	return cost;
}

int NPC_SellItemstrsStr(int itemindex,int flg,double rate,char *argtoken,int select,int sell)
{
	int cost;
	char escapedname[256];
	char name[256];	
	char *eff;
	

	cost = ITEM_getInt( itemindex, ITEM_COST);
	cost = (int)(cost * rate);

	if(sell != -1) return cost;

	strcpy( escapedname, ITEM_getChar( itemindex, ITEM_SECRETNAME));
	makeEscapeString( escapedname, name, sizeof( name));
	eff=ITEM_getChar(itemindex, ITEM_EFFECTSTRING);
	makeEscapeString( eff, escapedname, sizeof(escapedname));



	sprintf( argtoken,
#ifdef _ITEM_PILENUMS
			"%s|%d|%d|%d|%s|%d|%d|",
#else
			"%s|%d|%d|%d|%s|%d|",
#endif
			name, flg, cost,
			ITEM_getInt( itemindex, ITEM_BASEIMAGENUMBER),
			escapedname, select
#ifdef _ITEM_PILENUMS
			,ITEM_getInt( itemindex, ITEM_USEPILENUMS)
#endif
	);

	return -1;

}

BOOL NPC_SellNewItem(int meindex,int talker,char *data)
{
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char token[256], token2[256];
	int cost, k, select, itemindex;
	int MyGold, MaxGold, sellnum=1;

	MaxGold = CHAR_getMaxHaveGold( talker);
	MyGold = CHAR_getInt( talker, CHAR_GOLD);

	if(NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr)) == NULL) {
		print("GetArgStrErr");
		return FALSE;
	}
	getStringFromIndexWithDelim(data , "|" ,1, token, sizeof( token));
	select = atoi(token);
#ifdef _ITEM_PILENUMS
	getStringFromIndexWithDelim(data , "|" ,2, token, sizeof( token));
	sellnum = atoi(token);
#endif

	if( select < CHAR_STARTITEMARRAY || select >= CHAR_MAXITEMHAVE ) return FALSE;
	k = select;
	itemindex = CHAR_getItemIndex( talker , k);
#ifdef _NPC_SHOPALTER01
	cost = NPC_GetLimtItemList( talker,argstr, token2,select, NULL);
#else
	cost = NPC_GetLimtItemList( talker,argstr, token2,select);
#endif
	if( cost == -1 || (cost*sellnum)+MyGold >= MaxGold || !ITEM_CHECKINDEX( itemindex) ){
		int fd = getfdFromCharaIndex( talker);
		sprintf(token,"\n\n´ußr!πÔ§£∞_" "\n\nπÔ§£∞_∞⁄ ! •i§£•i•H¶AøÔ§@¶∏©O°H" );	
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
				WINDOW_BUTTONTYPE_OK, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);
		return FALSE;
	}
#ifdef _ITEM_PILENUMS
	if( NPC_DelItem( talker, k, sellnum) == FALSE ) return FALSE;
#else
	{
		LogItem(
			CHAR_getChar( talker, CHAR_NAME ),
			CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD ¶bitem™∫log§§ºW•[item¶W∫Ÿ
			itemindex,
#else
	   		ITEM_getInt( itemindex, ITEM_ID ),
#endif
			"Sell(ΩÊπD®„)",
			CHAR_getInt( talker,CHAR_FLOOR),
			CHAR_getInt( talker,CHAR_X ),
			CHAR_getInt( talker,CHAR_Y ),
			ITEM_getChar( itemindex, ITEM_UNIQUECODE),
			ITEM_getChar( itemindex, ITEM_NAME),
			ITEM_getInt( itemindex, ITEM_ID)

		);
	}
	CHAR_DelItem( talker, k);
#endif
	CHAR_AddGold( talker, cost*sellnum);
	CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
	return TRUE;
}



void NPC_LimitItemShop(int meindex,int talker,int select)
{

	int fd = getfdFromCharaIndex( talker);
	char token[NPC_UTIL_GETARGSTR_LINEMAX];
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buf[1024];

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr)) == NULL) {
       	print("shop_GetArgStr_Err");
       	return;
	}

	if(NPC_Util_GetStrFromStrWithDelim( argstr, "sellonly_msg", buf, sizeof( buf))!=NULL){
		sprintf(token,"\n\n%s", buf);
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
				WINDOW_BUTTONTYPE_YESNO, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

	}else{
		CHAR_talkToCli( talker, meindex, "≥o¨O∂RΩÊ±M™˘©±°C",CHAR_COLORWHITE);
	}
	return;
}

void NPC_ExpressmanCheck(int meindex,int talker)
{
	int fd = getfdFromCharaIndex( talker);
	char token[1024];
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buf[1024];

	if( NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr)) == NULL) {
       	print("shop_GetArgStr_Err");
       	return;
	}
	NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buf, sizeof( buf));
	sprintf(token,"4\nÅ@Å@Å@Å@Å@Å@Å@%s\n\n%s"
					"\n\nÅ@Å@Å@Å@Å@°’  •¥§u  °÷Å@Å@Å@"
				  "\n\nÅ@Å@Å@Å@  °’•Ê•I¶Êßı°÷"
					,CHAR_getChar(meindex,CHAR_NAME),buf);
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_SELECT, 
			WINDOW_BUTTONTYPE_CANCEL, 
			CHAR_WINDOWTYPE_WINDOWITEMSHOP_EXPRESS,
			CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
			token);

	return;


}

