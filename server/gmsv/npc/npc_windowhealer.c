#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_windowhealer.h"



/*--º£Œ≈»Ò---*/
#define RATE  1000


/* 
 * ¿ﬂƒÍ§µ§Ï§ø•¶•£•Û•…•¶§ÚΩ–§πNPC
 * ¥ ∞◊•∆•≠•π•»•¢•…•Ÿ•Û•¡•„°º§Ø§È§§§ §È∫Ó§Ï§Î§´§‚°£
 *
 */

enum {
	CHAR_WORK_LEVEL	= CHAR_NPCWORKINT1,
	CHAR_WORK_RANGE	= CHAR_NPCWORKINT2,
	CHAR_WORK_HP	= CHAR_NPCWORKINT3,
	CHAR_WORK_MP	= CHAR_NPCWORKINT4,

};


static void NPC_WindowHealer_selectWindow( int meindex, int toindex, int num);
void NPC_WindowHealerAllHeal( int talker,int mode );
BOOL NPC_WindowHealerLevelCheck(int meindex,int talker);
BOOL NPC_WindowMoneyCheck(int meindex,int talker,int mode);
int NPC_WindowCostCheck(int meindex,int talker);
BOOL NPC_PetHealerCheck(int talker);
int NPC_WindowCostCheckMp(int meindex,int talker);


/*********************************
* ΩÈ¥¸ΩËÕ˝
*********************************/
BOOL NPC_WindowHealerInit( int meindex )
{

	char *npcarg;
	char buf2[256];
	int range=1;
	int rate;
	double drate;
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEHEALER );

    npcarg = CHAR_getChar(meindex,CHAR_NPCARGUMENT);

	/*--≤Û…¸§π§Î§»§≠§Œ§™∂‚§ÚºË§Î•Ï•Ÿ•Î§Úµ≠≤±--*/
	/*--Ãµ§±§Ï§–∫Ó§È§Ï§ §§--*/
	if(getStringFromIndexWithDelim(npcarg,"|",1,buf2,sizeof(buf2))!=FALSE){
		CHAR_setWorkInt(meindex,CHAR_WORK_LEVEL,atoi(buf2));

	}else{
		return FALSE;
	}
	/*--¬—µ◊Œœ•Ï°º•»--*/
	if(getStringFromIndexWithDelim(npcarg, "|", 2, buf2, sizeof( buf2)) != FALSE ){
		drate = atof(buf2);
		if(drate==0) {
			rate=500;
		}else{
			rate=(int) (drate * RATE);
		}
		CHAR_setWorkInt(meindex,CHAR_WORK_HP,rate);
	}
	if(getStringFromIndexWithDelim(npcarg, "|", 3, buf2, sizeof( buf2)) != FALSE ){
		drate = atof(buf2);
		if(drate==0){
			rate=2000;
		}else{
			rate=(int)( drate * RATE);
		}
		CHAR_setWorkInt(meindex,CHAR_WORK_MP,rate);
	}
	if(getStringFromIndexWithDelim(npcarg, "|", 4, buf2, sizeof( buf2)) != FALSE ){
		range=atoi(buf2);
		if(range == 0){
			range=1;
		}
	}
	CHAR_setWorkInt(meindex,CHAR_WORK_RANGE,range);
    return TRUE;

}

void NPC_WindowHealerTalked( int meindex , int talkerindex , char *szMes ,int color )
{
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }

	if( NPC_Util_CharDistance( talkerindex, meindex ) 
		> CHAR_getWorkInt(meindex,CHAR_WORK_RANGE)){
		return;
	}
	if( (CHAR_getWorkInt(talkerindex,CHAR_WORKPARTYMODE) == 0)
	 || (CHAR_getWorkInt(talkerindex,CHAR_WORKPARTYMODE) == 2) ){
		NPC_WindowHealer_selectWindow( meindex, talkerindex,0);
	}else{
		int i=0;
		int otherindex;

		for( i=0 ; i < CHAR_PARTYMAX ;i++){
			otherindex=CHAR_getWorkInt(talkerindex,CHAR_WORKPARTYINDEX1+i);
			if(otherindex != -1){
				NPC_WindowHealer_selectWindow( meindex, otherindex,0);
			}
		}
	}
}
void NPC_WindowHealerLooked( int meindex , int lookedindex)
{
    if( CHAR_getInt( lookedindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }
	if( NPC_Util_CharDistance( lookedindex, meindex ) 
		> CHAR_getWorkInt(meindex,CHAR_WORK_RANGE)){
		 return;
	}
	NPC_WindowHealer_selectWindow( meindex, lookedindex,0);
}

static void NPC_WindowHealer_selectWindow( int meindex, int toindex, int num)
{

	char token[1024];
	char escapedname[1024];
	int fd = getfdFromCharaIndex( toindex);
	int buttontype=0;
	int windowtype=0;
	int windowno=0;

	/*--•¶•§•Û•…•¶•ø•§•◊•·•√•ª°º•∏§¨§™§™§§§Œ§«¿Ë§À¿ﬂƒÍ--*/
  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;

	switch( num){
	  case 0:
  		/*--¡™¬Ú≤ËÃÃ--*/
		sprintf(token," Å@Å@Å@´ußr°IßA®¸∂À§F∂‹°HÅ@ "
				  "\n\n Å@Å@Å@Å@Å@Å@°’ ≠@§[§O¶^¥_°÷Å@Å@Å@Å@Å@ "
					"\n Å@Å@Å@Å@Å@Å@°’  Æ§O¶^¥_  °÷Å@Å@Å@Å@Å@ "
					"\n Å@Å@Å@Å@ °’ ≠@§[§O.Æ§O¶^¥_ °÷Å@Å@ "
				  "\n\n Å@Å@Å@Å@Å@°’√d™´¶^¥_(ßK∂O)°÷Å@Å@Å@Å@ "
			);

	  	buttontype=WINDOW_BUTTONTYPE_CANCEL;
	  	windowtype=WINDOW_MESSAGETYPE_SELECT;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_STARTMSG; 
	  	break;


	  case 1:
		if( CHAR_getInt(toindex,CHAR_HP) ==CHAR_getWorkInt( toindex, CHAR_WORKMAXHP)){
			if(NPC_PetHealerCheck(toindex)==FALSE){
				sprintf(token,
				"Å@Å@Å@Å@Å@Å@ °’Å@≠@§[§O¶^¥_Å@°÷"
				"\n\n\n\nÅ@Å@Å@Å@¶¸•G®S¶≥•≤≠n¶^¥_≠Ú°IÅ@");
		  	}else{
		  		sprintf(token,
				"Å@Å@Å@Å@Å@Å@ °’Å@≠@§[§O¶^¥_Å@°÷"
				"\n\nÅ@Å@Å@Å@      ¶¸•G®S¶≥•≤≠n¶^¥_≠Ú°IÅ@"
					  "\n\nÅ@Å@Å@¶]¨∞√d™´¶nπ≥§]®¸∂À§F°I"
					  "\n                            •˝¿∞•L¶^¥_ßa°I");
		  	
		  		NPC_WindowHealerAllHeal(toindex,0 );
		  	}

		  	buttontype=WINDOW_BUTTONTYPE_OK;
		  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_HPMSG; 
			break;
		}else if(NPC_WindowHealerLevelCheck(meindex,toindex)==TRUE){
			sprintf(token,"Å@Å@Å@Å@Å@Å@ °’Å@≠@§[§O¶^¥_Å@°÷"
					"\n\n\nÅ@Å@Å@Å@Å@Å@Å@¨O≠n¶^¥_≠@§[§O®Sø˘ßa°IÅ@Å@Å@ "
					"\n\nÅ@≤{¶b™∫µ•Ø≈•i•HßK∂O¿∞ßA¶^¥_≠Ú°I");
		
		}else{
			int gold;
			gold=NPC_WindowCostCheck(meindex,toindex);
			sprintf(token,"Å@Å@Å@Å@Å@Å@ °’Å@≠@§[§O¶^¥_Å@°÷"
				"\n\n\nÅ@Å@Å@Å@Å@Å@Å@¨O≠n¶^¥_≠@§[§O®Sø˘ßa°IÅ@Å@Å@ "
				 "\n\nÅ@Å@Å@Å@Å@¶¨±z%d™∫STONE °C"
				 ,gold);
		}

	  	buttontype=WINDOW_BUTTONTYPE_YESNO;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_HPMSG; 
	  	break;

	  case 2:
		if( CHAR_getInt(toindex,CHAR_MP) ==CHAR_getWorkInt( toindex, CHAR_WORKMAXMP)){
			if(NPC_PetHealerCheck(toindex)==FALSE){
				sprintf(token,  "Å@Å@Å@Å@Å@Å@  °’Å@Æ§O¶^¥_Å@°÷"
					"\n\n\n\nÅ@Å@Å@Å@¶¸•G®S¶≥•≤≠n¶^¥_≠Ú°IÅ@");
			}else{
			sprintf(token,  "Å@Å@Å@Å@Å@Å@  °’Å@Æ§O¶^¥_Å@°÷"
					"\n\nÅ@Å@Å@Å@      ¶¸•G®S¶≥•≤≠n¶^¥_≠Ú°IÅ@"
					  "\n\nÅ@Å@Å@¶]¨∞√d™´¶nπ≥§]®¸∂À§F°I"
					  "\n                            •˝¿∞•L¶^¥_ßa°I");
				NPC_WindowHealerAllHeal(toindex,0 );
			}
		  	buttontype=WINDOW_BUTTONTYPE_OK;
		  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_SPIRITMSG; 
			break;
		}

		if(NPC_WindowHealerLevelCheck(meindex,toindex)==TRUE){
			sprintf(token,"Å@Å@Å@Å@Å@Å@  °’ Æ§O¶^¥_°÷"
					  "\n\nÅ@Å@Å@Å@Å@Å@Å@¨O≠n¶^¥_Æ§O®Sø˘ßa°IÅ@Å@Å@Å@ "
					  "\n\n ≤{¶b™∫µ•Ø≈•i•HßK∂O¿∞ßA¶^¥_≠Ú°I");
		}else{
			int cost;
			cost=NPC_WindowCostCheckMp(meindex,toindex);
			sprintf(token,"Å@Å@Å@Å@Å@Å@  °’ Æ§O¶^¥_°÷"
					"\n\n\nÅ@Å@Å@Å@Å@Å@Å@¨O≠n¶^¥_Æ§O®Sø˘ßa°I "
 					  "\n\nÅ@Å@Å@Å@¶¨±z%d™∫STONE °C",
					cost);
		}
	  	buttontype=WINDOW_BUTTONTYPE_YESNO;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_SPIRITMSG; 
	  	break;


	  case 3:
		sprintf(token,	    "\nÅ@Å@Å@Å@Å@Å@°’≠@§[§O§w¶^¥_°÷"
						    "\nÅ@Å@  °’√d™´§]§w¶^¥_§@ØÎ™¨∫A°÷"
						"\n\n\nÅ@Å@Å@Å@Å@  ≥oºÀ§l¥N®S∞›√D§F°IÅ@Å@Å@Å@");

		/*--¬—µ◊Œœ§Œ§ﬂ≤Û…¸§µ§ª§Î--*/
	  	NPC_WindowHealerAllHeal( toindex ,1 );
	  	buttontype=WINDOW_BUTTONTYPE_OK;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_OKHPMSG; 
	  	break;


	  case 4:
		sprintf(token,	    "\nÅ@Å@Å@Å@Å@Å@°’Æ§O§w¶^¥_°÷"
						    "\nÅ@Å@ °’√d™´§]§w¶^¥_§@ØÎ™¨∫A°÷"
						"\n\n\nÅ@Å@Å@Å@Å@ ≥oºÀ§l¥N®S∞›√D§F°I Å@Å@Å@");
		/*--µ§Œœ§Œ§ﬂ≤Û…¸§µ§ª§Î---*/
	  	NPC_WindowHealerAllHeal( toindex ,2 );
	 	buttontype=WINDOW_BUTTONTYPE_OK;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_OKSPIRITMSG; 
	  	break;


	  case 6:
		{
			int cost=0;

			if(CHAR_getInt(toindex,CHAR_HP) < CHAR_getWorkInt( toindex, CHAR_WORKMAXHP))
			{
				cost+=NPC_WindowCostCheck(meindex,toindex);
			}

			if( CHAR_getInt(toindex,CHAR_MP) < CHAR_getWorkInt( toindex, CHAR_WORKMAXMP))
			{
				cost=cost+NPC_WindowCostCheckMp(meindex,toindex);
			}
		
			if(cost==0) {
				if(NPC_PetHealerCheck(toindex)==FALSE){
				sprintf(token,
						"Å@Å@Å@Å@  °’Å@≠@§[§OÆ§O¶^¥_Å@°÷"
				"\n\n\n\nÅ@Å@Å@Å@¶¸•G®S¶≥•≤≠n¶^¥_≠Ú°IÅ@");
				}else{
				sprintf(token,"Å@Å@Å@Å@  °’Å@≠@§[§OÆ§O¶^¥_Å@°÷"
					"\n\nÅ@Å@Å@Å@      ¶¸•G®S¶≥•≤≠n¶^¥_≠Ú°IÅ@"
					  "\n\nÅ@Å@Å@¶]¨∞√d™´¶nπ≥§]®¸∂À§F°I"
					  "\n                            •˝¿∞•L¶^¥_ßa°I");
				NPC_WindowHealerAllHeal(toindex,0 );
			}
		
				
				
			  	buttontype=WINDOW_BUTTONTYPE_OK;
			  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_SPIRITMSG; 
				break;

			}else{
				sprintf(token,"Å@Å@Å@  °’Å@≠@§[§OÆ§O¶^¥_Å@°÷"
						"\n\n\nÅ@Å@Å@Å@ ¨O≠n¶^¥_≠@§[§OÆ§O®Sø˘ßa°I"
						"\n\nÅ@Å@Å@Å@Å@¶¨±z%d™∫STONE °C",cost);
			}
		}

		if(NPC_WindowHealerLevelCheck(meindex,toindex)==TRUE){
			sprintf(token,"Å@Å@Å@Å@  °’Å@≠@§[§OÆ§O¶^¥_Å@°÷"
					"\n\n\nÅ@Å@Å@Å@ Å@¨O≠n¶^¥_≠@§[§OÆ§O®Sø˘ßa°I"
					"\n\nÅ@≤{¶b™∫µ•Ø≈•i•HßK∂O¿∞ßA¶^¥_≠Ú°I");
		}
	
	  	buttontype=WINDOW_BUTTONTYPE_YESNO;
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_ALLMSG; 
	  	break;


	  case 7:
		sprintf(token,"Å@Å@Å@Å@ °’≠@§[§OÆ§O§w¶^¥_°÷"
				  "\nÅ@Å@  °’√d™´§]§w¶^¥_§@ØÎ™¨∫A°÷"
				  "\n\n\nÅ@Å@Å@Å@≥oºÀ§@®”¥N¶^¥_∞∑±d§F°I");
	  	
	  	NPC_WindowHealerAllHeal( toindex ,3 );
		buttontype=WINDOW_BUTTONTYPE_OK;
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_OKALLMSG; 
	  	break;


	  case 8:
		sprintf(token,"\n\n\n\n Å@Å@Øu•i±§¶¸•G©“±a™∫™˜ø˙§£∞˜≠Ú°I");

		buttontype=WINDOW_BUTTONTYPE_OK;
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_OKHPMSG; 
	  	break;

	  case 9:
		sprintf(token,"Å@Å@Å@Å@Å@Å@Å@°’√d™´¶^¥_°÷Å@Å@Å@Å@Å@Å@"
				"\n\n\nÅ@Å@Å@Å@Å@Å@Å@§w∏g®S∞›√D§F∞’°IÅ@Å@Å@Å@Å@"
				  "\n\nÅ@Å@¶˝¨O§”´j±j™∫∏‹§]§£¶Ê≠Ú°IÅ@Å@");
		NPC_WindowHealerAllHeal(toindex,0 );
		buttontype=WINDOW_BUTTONTYPE_OK;
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_OKHPMSG; 
	  	break;

	  case 10:
		sprintf(token,"Å@Å@Å@Å@Å@Å@Å@°’√d™´¶^¥_°÷Å@Å@Å@Å@Å@Å@"
				"\n\n\nÅ@ ¶¸•G®S¶≥•≤≠n¶^¥_√d™´™∫ºÀ§l°CÅ@"
				  "\n\nÅ@Å@¶˝¨O§”´j±j™∫∏‹§]§£¶Ê≠Ú°IÅ@Å@");
		buttontype=WINDOW_BUTTONTYPE_OK;
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=CHAR_WINDOWTYPE_WINDOWHEALER_OKHPMSG; 
	  	break;


	}


	makeEscapeString( token, escapedname, sizeof(escapedname));
	/*-§≥§≥§«¡˜øÆ§π§Î--*/
	lssproto_WN_send( fd, windowtype, 
					buttontype, 
					windowno,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					escapedname);

}


/*-----------------------------------------
•Ø•È•§•¢•Û•»§´§È ÷§√§∆§≠§øª˛§À∏∆§”Ω–§µ§Ï§Î°£
-------------------------------------------*/
void NPC_WindowHealerWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{

	if( NPC_Util_CharDistance( talkerindex, meindex ) > (CHAR_getWorkInt(meindex,CHAR_WORK_RANGE)+4)) return;

	switch( seqno){
	  case CHAR_WINDOWTYPE_WINDOWHEALER_STARTMSG:
	  	if(atoi(data)==2)	/*--¡™¬Ú§∑§øπ‡Ã‹ 1--*/
	  	{
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 1 );
	  	
	  	}else if(atoi(data)==3){
	  		/*--¡™¬Ú§∑§øπ‡Ã‹ 2--*/
	  		NPC_WindowHealer_selectWindow( meindex, talkerindex, 2 );
	  	
	  	}else if(atoi(data)==4){
	  		/*--¡™¬Ú§∑§øπ‡Ã‹ 2--*/
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 6 );
		
		}else if(atoi(data)==6){
			/*--¡™¬Ú§∑§øπ‡Ã‹ 2--*/
	  		if(NPC_PetHealerCheck( talkerindex)==TRUE){
		  		NPC_WindowHealer_selectWindow( meindex, talkerindex, 9 );
			}else{
				NPC_WindowHealer_selectWindow( meindex, talkerindex, 10 );
			}
		}else if(select==WINDOW_BUTTONTYPE_CANCEL){
			  	/*--§≥§≥§ﬁ§«§Ø§Î§»Ω™Œª§ §Œ§«≤ø§‚§∑§ §§--*/
	  	}
	  	break;
	  

	  /*-----¬—µ◊Œœ≤Û…¸§Ú§™§≥§ §¶-----*/
	  case CHAR_WINDOWTYPE_WINDOWHEALER_HPMSG:
		if(select==WINDOW_BUTTONTYPE_OK){
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 0 );
		
		}else if(select==WINDOW_BUTTONTYPE_YES){
			/*--§™∂‚§Œ•¡•ß•√•Ø--*/
			if(NPC_WindowMoneyCheck(meindex,talkerindex,1)==TRUE){
				NPC_WindowHealer_selectWindow( meindex, talkerindex, 3 );
			}else{
				NPC_WindowHealer_selectWindow( meindex, talkerindex, 8 );
			}
		}else if(select==WINDOW_BUTTONTYPE_NO){
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 0 );

		}
		break;


	  /*-----µ§Œœ≤Û…¸§Ú§™§≥§ §¶-----*/
	  case CHAR_WINDOWTYPE_WINDOWHEALER_SPIRITMSG:
		if(select==WINDOW_BUTTONTYPE_OK){
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 0 );
		
		}else if(select==WINDOW_BUTTONTYPE_YES){
			/*--§™∂‚§Œ•¡•ß•√•Ø--*/
			if(NPC_WindowMoneyCheck(meindex,talkerindex,2)==TRUE){
				NPC_WindowHealer_selectWindow( meindex, talkerindex, 4 );
			}else{
				NPC_WindowHealer_selectWindow( meindex, talkerindex, 8 );
			}
		
		}else if(select==WINDOW_BUTTONTYPE_NO){
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 0 );
		}
		
		break;


	  /*-----¬—µ◊Œœ≤Û…¸§Œ∑Î≤Ã-----*/
	  case CHAR_WINDOWTYPE_WINDOWHEALER_OKHPMSG:
		if(select==WINDOW_BUTTONTYPE_OK){
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 0 );
		}
		break;


	  case CHAR_WINDOWTYPE_WINDOWHEALER_OKSPIRITMSG:
		if(select==WINDOW_BUTTONTYPE_OK){
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 0 );
		}
		break;
	

	  /*--¡¥≤Û…¸§µ§ª§Î--*/
	  case CHAR_WINDOWTYPE_WINDOWHEALER_ALLMSG:
		if(select==WINDOW_BUTTONTYPE_OK){
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 0 );
		
		}else if(select==WINDOW_BUTTONTYPE_YES){
			if(NPC_WindowMoneyCheck(meindex,talkerindex,3)==TRUE){
				NPC_WindowHealer_selectWindow( meindex, talkerindex, 7 );
			}else{
				NPC_WindowHealer_selectWindow( meindex, talkerindex, 8 );
			}
		
		}else if(select==WINDOW_BUTTONTYPE_NO){
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 0 );
		}
		break;
	

	  /*--¡¥≤Û…¸§Œ∑Î≤Ã--*/
	  case CHAR_WINDOWTYPE_WINDOWHEALER_OKALLMSG:
		if(select==WINDOW_BUTTONTYPE_YES){
			NPC_WindowHealer_selectWindow( meindex, talkerindex, 0 );
		}
		break;
	}

	if(select==WINDOW_BUTTONTYPE_CANCEL 
	&& CHAR_getWorkInt( talkerindex, CHAR_WORKPARTYMODE ) == 2)
	{
		CHAR_sendWatchEvent( CHAR_getWorkInt( talkerindex, CHAR_WORKOBJINDEX),
								CHAR_ACTPLEASURE,NULL,0,TRUE);
		CHAR_setWorkInt( talkerindex, CHAR_WORKACTION, CHAR_ACTPLEASURE);
	}

}


/*--Ω˝§ƒ§§§∆§§§Î§Œ§ §È°¢TRUE§Ú ÷§π--*/
BOOL NPC_PetHealerCheck(int talker)
{

	int petindex;
	int i;
	
	/*--•⁄•√•»§œ§™§ﬁ§±§«≤Û…¸§∑§∆§¢§≤§Î--*/
	for(i=0;i<CHAR_MAXPETHAVE;i++){
    	petindex = CHAR_getCharPet(talker,i);

		if( petindex == -1  )  continue;

		/*  •≠•„•È§Œ«€ŒÛ•¡•ß•√•Ø    */
		if( !CHAR_CHECKINDEX( talker ) )  continue;
		
		
		if( CHAR_getInt(petindex,CHAR_HP) != CHAR_getWorkInt(petindex,CHAR_WORKMAXHP) )
		{
			return TRUE;
		}
	}
	
	return FALSE;

}


/*-------------------------------------
	º¬∫›§À¬—µ◊Œœ§»µ§Œœ§Ú≤Û…¸§µ§ª§Î§»§≥§Ì
---------------------------------------*/
void NPC_WindowHealerAllHeal( int talker,int mode )
{
	int i;
	int petindex;
	char petsend[64];	
	char msgbuf[64];

	/*--¬—µ◊Œœ§Œ§ﬂ≤Û…¸--*/
	if(mode==1){
		/*--HP§¨¬—µ◊Œœ§ §Œ§«§¢§Î--*/
    	CHAR_setInt( talker , CHAR_HP ,CHAR_getWorkInt( talker, CHAR_WORKMAXHP ) );
	}else if(mode==2){
		/*--µ§Œœ§Œ§ﬂ≤Û…¸--*/
		/*--MP§¨µ§Œœ§ §Œ§«§¢§Î--*/
    	CHAR_setInt( talker , CHAR_MP ,CHAR_getWorkInt( talker, CHAR_WORKMAXMP ) );
	}else if(mode==3){
    	/*--¡¥≤Û…¸---*/
    	CHAR_setInt( talker , CHAR_HP ,CHAR_getWorkInt( talker, CHAR_WORKMAXHP ) );
    	CHAR_setInt( talker , CHAR_MP ,CHAR_getWorkInt( talker, CHAR_WORKMAXMP ) );
	}

	/*--•⁄•√•»§œ§™§ﬁ§±§«≤Û…¸§∑§∆§¢§≤§Î--*/
	for(i=0;i<CHAR_MAXPETHAVE;i++){
    	petindex = CHAR_getCharPet(talker,i);

		if( petindex == -1  )  continue;

	   /*  •≠•„•È§Œ«€ŒÛ•¡•ß•√•Ø    */
		if( !CHAR_CHECKINDEX( talker ) )  continue;

		/*--≤Û…¸--*/
		CHAR_setFlg( petindex,CHAR_ISDIE,0 );
		CHAR_setInt( petindex , CHAR_HP ,CHAR_getWorkInt( petindex, CHAR_WORKMAXHP ) );
		CHAR_setInt( petindex , CHAR_MP ,CHAR_getWorkInt( petindex, CHAR_WORKMAXMP ) );

		/*--•—•È•·°º•øƒ¥¿∞--*/
		CHAR_complianceParameter( petindex );
		sprintf( petsend, "K%d", i );
		CHAR_sendStatusString( talker , petsend );

	}

	/*--•—•È•·°º•ø¡˜§Í--*/
	CHAR_send_P_StatusString(talker,CHAR_P_STRING_HP);
	CHAR_send_P_StatusString(talker,CHAR_P_STRING_MP);


	/*---√Á¥÷§¨§§§Ï§–√Á¥÷§À§‚¡˜øÆ--*/
	if(CHAR_getWorkInt( talker, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE )
	{
		int	topartyarray = -1;
		int	oyaindex = CHAR_getWorkInt( talker	, CHAR_WORKPARTYINDEX1);

		if( CHAR_CHECKINDEX( oyaindex )) {
			int		i;
		
			/* º´ ¨§»º£§∑§ø§‰§ƒ§Œ√Á¥÷§Œ«€ŒÛ§ŒæÏΩÍ§ÚºË∆¿ */
			for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
				int workindex = CHAR_getWorkInt( oyaindex, CHAR_WORKPARTYINDEX1 +i);
		
				if( CHAR_CHECKINDEX( workindex) ) {
					if( workindex == talker ) {
						topartyarray = i;
						break;
					}
				}
			}
		
			for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
				int otherindex = CHAR_getPartyIndex( talker, i);
		
				/* √Á¥÷•—•È•·°º•ø§Ú¡˜§Î */
				if( CHAR_CHECKINDEX( otherindex) ) {
					snprintf( msgbuf, sizeof( msgbuf), "N%d", topartyarray);
					if( otherindex != talker) {
						CHAR_sendStatusString( otherindex, msgbuf);
					}
				}
			}
		}
	}

}


/*-------------------------------------
•Ï•Ÿ•Î•¡•ß•√•Ø
•Ï•Ÿ•Î§Ú∏´§∆¿ﬂƒÍ•Ï•Ÿ•Î§Ë§Íƒ„§±§Ï§–ÃµŒ¡§À§π§Î
Ã·§Í√Õ
	¿ﬂƒÍ•Ï•Ÿ•ÎÃ§À˛		TRUE
	¿ﬂƒÍ•Ï•Ÿ•Î∞ æÂ§ §È	FALSE
---------------------------------------*/
BOOL NPC_WindowHealerLevelCheck(int meindex,int talker)
{
		int level;
	
		level=CHAR_getWorkInt(meindex,CHAR_WORK_LEVEL);

		if(level > CHAR_getInt(talker,CHAR_LV)){
			return TRUE;
		}

		return FALSE;

}


/*-------------------------------------------
§™∂‚ª˝§√§∆§Î§´•¡•ß•√•Ø
∞˙øÙ
meindex°¶°¶°¶£Œ£–£√§Œ•§•Û•«•√•Ø•π
talker°¶°¶°¶°¶•≠•„•È•Ø•ø°º§Œ•§•Û•«•√•Ø•π
---------------------------------------------*/
BOOL NPC_WindowMoneyCheck(int meindex,int talker,int mode)
{
	int cost=0;
	int level;
	
	level=CHAR_getWorkInt(meindex,CHAR_WORK_LEVEL);
	
	if(mode==1){
		if(level <= CHAR_getInt(talker,CHAR_LV)){
			/*---§™∂‚§Ú≥‰§ÍΩ–§π---*/
			cost=NPC_WindowCostCheck(meindex,talker);
			/*--∏Ω∫ﬂ§œ≤æ¿ﬂƒÍ---*/
			/*---§™∂‚§¨¬≠§Í§Î§´§…§¶§´§Œ•¡•ß•√•Ø---*/
			if(CHAR_getInt(talker,CHAR_GOLD) < cost){
				return FALSE;
			
			}
			CHAR_DelGold( talker, cost);
		}
	}
	if(mode==2){
		if(level <= CHAR_getInt(talker,CHAR_LV)){
			/*---§™∂‚§Ú≥‰§ÍΩ–§π---*/
			cost=NPC_WindowCostCheckMp(meindex,talker);
	
			/*--∏Ω∫ﬂ§œ≤æ¿ﬂƒÍ---*/
			/*---§™∂‚§¨¬≠§Í§Î§´§…§¶§´§Œ•¡•ß•√•Ø---*/
			if(CHAR_getInt(talker,CHAR_GOLD) < cost){
				return FALSE;
			}
			CHAR_DelGold( talker, cost);
		}
	}

	if(mode==3){
		if(level <= CHAR_getInt(talker,CHAR_LV)){

			/*--¬—µ◊Œœ§Œ§™∂‚§Œ∑◊ªª--*/
			if( CHAR_getInt(talker,CHAR_HP) <CHAR_getWorkInt( talker, CHAR_WORKMAXHP))
			{
				cost=NPC_WindowCostCheck(meindex,talker);
			}

			/*--µ§Œœ§Œ§™∂‚§Œ∑◊ªª--*/
			if( CHAR_getInt(talker,CHAR_MP) < CHAR_getWorkInt( talker, CHAR_WORKMAXMP))
			{
				cost=cost+NPC_WindowCostCheckMp(meindex,talker);
			}
		
			/*--≤æ¿ﬂƒÍ--*/
			if(CHAR_getInt(talker,CHAR_GOLD) < cost){
				return FALSE;
			}
		
			/*---§™∂‚§Ú∏∫§È§π---*/
			CHAR_DelGold( talker, cost);
		}
		
	}
	
	CHAR_send_P_StatusString(talker,CHAR_P_STRING_GOLD);

	return TRUE;

}



/*-----------------------------------------
•≥•π•»§Œ∑◊ªª
------------------------------------------*/
int NPC_WindowCostCheck(int meindex,int talker)
{
	int cost;
	double drate;
	int rate;
	
	rate=CHAR_getWorkInt(meindex,CHAR_WORK_HP);
	drate=(double) rate / 1000 ;
	cost=CHAR_getInt(talker,CHAR_LV);
	cost=(int)cost*drate;
	if(cost < 1) cost=1;
	return cost;
}

/*-----------------------------------------
µ§Œœ•≥•π•»§Œ∑◊ªª
------------------------------------------*/
int NPC_WindowCostCheckMp(int meindex,int talker)
{
	int cost;
	double drate;
	int rate;
	
	rate=CHAR_getWorkInt(meindex,CHAR_WORK_MP);
	drate=(double) rate / 1000 ;
	cost=CHAR_getInt(talker,CHAR_LV);
	cost=(int)cost*drate;
	if(cost==0) cost=1;
	return cost;
}
