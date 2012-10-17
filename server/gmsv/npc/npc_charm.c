#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_charm.h"

//Ì¥ÎÏÂå¤òÆ³¤¯·×»»¼°¤Ï

//¥ì¥Ù¥ë¡ö£Ò£Á£Ô£Å¡ö¡Ê¸½ºß¤ÎÌ¥ÎÏ/£×£Á£Ò£Õ¡Ë

//²óÉüÎÌ¤Ï¡¡£µ¤Ç¤¹¡£
/*
#define RATE  4		//¥ì¡¼¥È¡©
#define CHARMHEAL 5 //Ì¥ÎÏ¤Î²óÉüÎÌ
#define WARU	3	//Ì¥ÎÏ¤ò³ä¤ëÃÍ

*/

#define RATE  10	//¥ì¡¼¥È¡©
#define CHARMHEAL 5 //Ì¥ÎÏ¤Î²óÉüÎÌ
#define WARU	3	//Ì¥ÎÏ¤ò³ä¤ëÃÍ


static void NPC_Charm_selectWindow( int meindex, int toindex, int num);
int NPC_CharmCost(int meindex,int talker);
void NPC_CharmUp(int meindex,int talker);


/*********************************
* ½é´ü½èÍý
*********************************/
BOOL NPC_CharmInit( int meindex )
{
	/*--¥­¥ã¥é¤Î¥¿¥¤¥×¤òÀßÄê--*/
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPECHARM );
	return TRUE;

}


/*********************************
* ÏÃ¤·¤«¤±¤é¤ì¤¿»þ¤Î½èÍý
*********************************/
void NPC_CharmTalked( int meindex , int talkerindex , char *szMes ,int color )
{

    /* ¥×¥ì¥¤¥ä¡¼¤ËÂÐ¤·¤Æ¤À¤±È¿±þ¤¹¤ë */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER )
    {
    	return;
    }
	
	/*--ÌÜ¤ÎÁ°¤Ë¤¤¤ë¤«¤É¤¦¤«¡©--*/
	if(NPC_Util_isFaceToFace( meindex ,talkerindex , 2) == FALSE) {
		/* £±¥°¥ê¥Ã¥É°ÊÆâ¤Î¤ß */
		if(NPC_Util_isFaceToChara( talkerindex, meindex, 1) == FALSE) return;
	}

	NPC_Charm_selectWindow( meindex, talkerindex, 0);
}


/*
 * ³Æ½èÍý¤ËÊ¬¤±¤ë
 */
static void NPC_Charm_selectWindow( int meindex, int toindex, int num)
{

	char token[1024];
	char escapedname[1024];
	int fd = getfdFromCharaIndex( toindex);
	int buttontype = 0;
	int windowtype = 0;
	int windowno = 0;
	int cost = 0;
	int chartype;
	
	/*--¥¦¥¤¥ó¥É¥¦¥¿¥¤¥×¥á¥Ã¥»¡¼¥¸¤¬¤ª¤ª¤¤¤Î¤ÇÀè¤ËÀßÄê--*/
  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;

	switch( num) {
	  case 0:
  		/*--ÁªÂò²èÌÌ--*/
		sprintf(token,"4\n @@@   = = ¬ü®e®v = = "
				     "\n\n @Åwªï¥úÁ{¡A¤µ¤Ñ­n°µ¤°»ò©O¡H"
				  "\n "
				  "\n\n@@@@¡m@µ¹§Ú¾y¤O@¡n "
				  "\n\n@@@@¡m ¤°»ò¤]¤£°µ ¡n "
		);

	  	buttontype = WINDOW_BUTTONTYPE_NONE;
	  	windowtype = WINDOW_MESSAGETYPE_SELECT;
	  	windowno = CHAR_WINDOWTYPE_CHARM_START; 
	  	break;

	case 1:
		cost = NPC_CharmCost( meindex, toindex);
		if(cost == -1){
			sprintf(token,"@@@@     = = ¬ü®e®v = =  "
				      "\n\n@@@@§Aªº¾y¤O¯u¬O§¹¬ü"
				      "\n\n@@@½Ð¥Ã»·«O«ù§Aªº¾y¤O­ò¡C"
			);
		  	buttontype = WINDOW_BUTTONTYPE_OK;
		}else{
			sprintf(token,"@@@@     = = ¬ü®e®v = =  "
				 	    "\n@á²{¦b,¤dÁH¯S´f»ù¹ê¬I¤¤â"
					  "\n\n ­n±N§Aªº¾y¤O¤W¤É¤­ÂIªº¸Ü"
					  "\n@@@@»Ý­n%6dªºstone­ò¡I"
						"\n\n@@@§Y¨Ï³o¼Ë¤]¥i¥H¶Ü¡H",cost
			);
		  	buttontype = WINDOW_BUTTONTYPE_YESNO;

		}
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno = CHAR_WINDOWTYPE_CHARM_END; 

		break;

	case 2:
		cost = NPC_CharmCost( meindex, toindex);
		chartype = CHAR_getInt( toindex, CHAR_IMAGETYPE);
		
		if(cost > CHAR_getInt( toindex, CHAR_GOLD)) {
			sprintf(token,"@@@@     = = ¬ü®e®v = =  "
					  "\n\n@@ «u§r!§Aªº¿ú¤£°÷­ò¡I"
				        "\n@@    ¥ý¥h¦s¦n¿ú¤U¦¸¦A¨Ó§a¡I"
			);

		}else{
			NPC_CharmUp( meindex, toindex);

			/*--¥­¥ã¥é¤Î¥¿¥¤¥×¤Ë¤è¤Ã¤Æ¥á¥Ã¥»¡¼¥¸¤òÊÑ¤¨¤Æ¤ß¤¿--*/
			switch( chartype) {
			  case CHAR_IMAGETYPE_GIRL:
				sprintf(token,"@@@@     = = ¬ü®e®v = =  "
					  "\n\n @¶â,³o¼Ë§Aªº¾y¤O´N¤W¤ÉÅo¡I"
					  "\n\n@@@·U¨Ó·U¥i·R¤F©O¡I"
				);

				break;
			  case CHAR_IMAGETYPE_BOY:
				sprintf(token,"@@@@     = = ¬ü®e®v = =  "
					  "\n\n @¶â,³o¼Ë§Aªº¾y¤O´N¤W¤ÉÅo¡I"
					  "\n\n@@¬O¤£¬O¤ñ­è¤~­n«Ó¦h¤F©O¡H"
				);
			  	break;
			  	
			  case CHAR_IMAGETYPE_CHILDBOY:
			  case CHAR_IMAGETYPE_CHILDGIRL:
				sprintf(token,"@@@@     = = ¬ü®e®v = =  "
					  "\n\n @¶â,³o¼Ë§Aªº¾y¤O´N¤W¤ÉÅo¡I"
					  "\n\n@@  ¯uªºÅÜ±o¦n¥i·R­ò¡I"
				);
			 	break;
			 	
			   case CHAR_IMAGETYPE_MAN:
				sprintf(token,"@@@@     = = ¬ü®e®v = =  "
					  "\n\n @¶â,³o¼Ë§Aªº¾y¤O´N¤W¤ÉÅo¡I"
					  "\n\n @@@ÅÜ±o§ó»Å¤F©O¡I"
				);
			 	break;
			 	
			   case CHAR_IMAGETYPE_WOMAN:
				sprintf(token,"@@@@     = = ¬ü®e®v = =  "
				  "\n\n @¶â,³o¼Ë§Aªº¾y¤O´N¤W¤ÉÅo¡I"
				  "\n\n    ¥i¯u¬OÅÜ±o·U¨Ó·U¬ü¤F©O¡I"
				);
			 	break;
			 
			 }
		}

		buttontype = WINDOW_BUTTONTYPE_OK;
		windowtype = WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno = CHAR_WINDOWTYPE_CHARM_END; 
		break;
	}
	
	makeEscapeString( token, escapedname, sizeof( escapedname));
	/*-¤³¤³¤ÇÁ÷¿®¤¹¤ë--*/
	lssproto_WN_send( fd, windowtype, 
					buttontype, 
					windowno,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					escapedname);

}


/*-----------------------------------------
¥¯¥é¥¤¥¢¥ó¥È¤«¤éÊÖ¤Ã¤Æ¤­¤¿»þ¤Ë¸Æ¤Ó½Ð¤µ¤ì¤ë¡£
-------------------------------------------*/
void NPC_CharmWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) return;

	switch( seqno){
	  case CHAR_WINDOWTYPE_CHARM_START:
	  	if(atoi( data) == 2) {
			NPC_Charm_selectWindow( meindex, talkerindex, 1 );
		}
		break;

	  case CHAR_WINDOWTYPE_CHARM_END:
	  	if(select == WINDOW_BUTTONTYPE_YES) {
			NPC_Charm_selectWindow( meindex, talkerindex, 2 );
		}
		break;
	}
	
}



/*--Ì¥ÎÏ£Õ£Ð--*/
void NPC_CharmUp(int meindex,int talker)
{
	int cost;
	int i;
	int petindex;
	char petsend[64];	

	/*--¤ª¶â¤ò¸º¤é¤·¤Þ¤·¤ç¤¦--*/
	cost = NPC_CharmCost( meindex, talker);
	CHAR_setInt( talker, CHAR_GOLD,
			CHAR_getInt( talker, CHAR_GOLD) - cost);
	CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);

	/*--Ì¥ÎÏ¤¬£±£°£°°Ê¾å¤Ë¤Ê¤ë¾ì¹ç¤Ï¶¯°ú¤Ë£±£°£°¤Ë¤¹¤ë--*/
	if(CHAR_getInt( talker, CHAR_CHARM) + CHARMHEAL >= 100) {
		CHAR_setInt( talker, CHAR_CHARM, 100);
	}else{
		/*--Ì¥ÎÏ¤ò¥»¥Ã¥È--*/
		CHAR_setInt(talker, CHAR_CHARM,
	 			(CHAR_getInt( talker, CHAR_CHARM) + CHARMHEAL));
	}
	
	/*--¥¹¥Æ¡¼¥¿¥¹¤Î¹¹¿·--*/
	CHAR_complianceParameter( talker );
	CHAR_send_P_StatusString( talker, CHAR_P_STRING_CHARM);


	/*--¥Ú¥Ã¥È¤Î¥Ñ¥é¥á¡¼¥¿¤ò¹¹¿·--*/
	for( i = 0 ; i < CHAR_MAXPETHAVE ; i++){
    	petindex = CHAR_getCharPet( talker, i);

		if( petindex == -1  )  continue;

	   /*  ¥­¥ã¥é¤ÎÇÛÎó¥Á¥§¥Ã¥¯    */
		if( !CHAR_CHECKINDEX( talker ) )  continue;

		/*--¥Ñ¥é¥á¡¼¥¿Ä´À°--*/
		CHAR_complianceParameter( petindex );
		sprintf( petsend, "K%d", i );
		CHAR_sendStatusString( talker , petsend );
	}
}


/*--¤ª¶â¤Î·×»»--*/
int NPC_CharmCost(int meindex,int talker)
{
	int cost;
	int level;
	int charm;
	int trans;

	level = CHAR_getInt( talker, CHAR_LV);
	charm = CHAR_getInt( talker, CHAR_CHARM);
	trans = CHAR_getInt( talker, CHAR_TRANSMIGRATION);

	if(charm >= 100) return -1;
	
	if(charm <= 1) charm = WARU;
	
	/*-- ·×»»¼° --*/
	cost = level * RATE * (charm / WARU) * (trans+1);

	return cost;

}
