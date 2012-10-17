#include "version.h"
#include "autil.h"
#include "lssproto_serv.h"
#include "common.h"	// for StoneAge
#include "log.h"	// for StoneAge
#include "char.h"
#include <stdio.h>
#include <time.h>
#include <net.h>


void lssproto_SetServerLogFiles( char *r , char *w )
{
	lssproto_strcpysafe( lssproto_writelogfilename , w , sizeof(lssproto_writelogfilename ));
	lssproto_strcpysafe( lssproto_readlogfilename , r , sizeof(lssproto_readlogfilename ));
}
int lssproto_InitServer(	int (*writefunc)(int,char*,int) ,	int worksiz )
{
	int i;
	if( (void*)writefunc == NULL){lssproto.write_func = lssproto_default_write_wrap;} else {lssproto.write_func = writefunc;}
	lssproto_AllocateCommonWork(worksiz);
	lssproto_stringwrapper = (char**)calloc( 1,sizeof(char*) * MAXLSRPCARGS);
	if(lssproto_stringwrapper ==NULL)return -1;
	memset( lssproto_stringwrapper , 0, sizeof(char*)*MAXLSRPCARGS);
	for(i=0;i<MAXLSRPCARGS;i++){
		lssproto_stringwrapper[i] = (char*)calloc( 1, worksiz );
		if( lssproto_stringwrapper[i] == NULL){
			for(i=0;i<MAXLSRPCARGS;i++){free( lssproto_stringwrapper[i]);return -1;}
		}
	}
	lssproto.ret_work = (char*)calloc( 1,sizeof( worksiz ));
	if( lssproto.ret_work == NULL ){ return -1; }
	return 0;
}
void lssproto_CleanupServer( void )
{
	int i;
	free( lssproto.work );
	free( lssproto.arraywork);
	free( lssproto.escapework );
	free( lssproto.val_str);
	free( lssproto.token_list );
	for(i=0;i<MAXLSRPCARGS;i++){free( lssproto_stringwrapper[i]);}
	free( lssproto_stringwrapper );
	free( lssproto.ret_work );
}

#define DME() print("<DME(%d)%d:%d>",fd,__LINE__,func)

int lssproto_ServerDispatchMessage(int fd, char *encoded)
{
	int func,fieldcount;
	char raw[65500];

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	
	util_DecodeMessage(raw,encoded);

	if( !util_SplitMessage(raw,SEPARATOR) ){
		print("\nDME1:package=%s\n",raw);
		DME(); return -1;
	}
	if (!util_GetFunctionFromSlice(&func, &fieldcount)) {
		logHack(fd,HACK_GETFUNCFAIL);
		DME(); return -1;
	}
	
	if (func==LSSPROTO_W_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char direction[65500];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, direction);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_W_recv(fd, x, y, direction);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_W2_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char direction[65500];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, direction);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_W2_recv(fd, x, y, direction);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_EV_RECV) {
		int checksum=0, checksumrecv;
		int event;
		int seqno;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &event);
		checksum += util_deint(3, &seqno);
		checksum += util_deint(4, &x);
		checksum += util_deint(5, &y);
		checksum += util_deint(6, &dir);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_EV_recv(fd, event, seqno, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DU_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DU_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_EO_RECV) {
		int checksum=0, checksumrecv;
		int dummy;

		checksum += util_deint(2, &dummy);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_EO_recv(fd, dummy);
		util_DiscardMessage();
		return 0;
	}
     
	if (func==LSSPROTO_BU_RECV) {
		int checksum=0, checksumrecv;
		int dummy;

		checksum += util_deint(2, &dummy);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_BU_recv(fd, dummy);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_JB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_JB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_LB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_LB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_B_RECV) {
		int checksum=0, checksumrecv;
		char command[65500];

		checksum += util_destring(2, command);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_B_recv(fd, command);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SKD_RECV) {
		int checksum=0, checksumrecv;
		int dir;
		int index;

		checksum += util_deint(2, &dir);
		checksum += util_deint(3, &index);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SKD_recv(fd, dir, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ID_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int haveitemindex;
		int toindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &haveitemindex);
		checksum += util_deint(5, &toindex);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_ID_recv(fd, x, y, haveitemindex, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PI_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &dir);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PI_recv(fd, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DI_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int itemindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &itemindex);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DI_recv(fd, x, y, itemindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DG_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int amount;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &amount);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DG_recv(fd, x, y, amount);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DP_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int petindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &petindex);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DP_recv(fd, x, y, petindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MI_RECV) {
		int checksum=0, checksumrecv;
		int fromindex;
		int toindex;

		checksum += util_deint(2, &fromindex);
		checksum += util_deint(3, &toindex);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_MI_recv(fd, fromindex, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MSG_RECV) {
		int checksum=0, checksumrecv;
		int index;
		char message[65500];
		int color;

		checksum += util_deint(2, &index);
		checksum += util_destring(3, message);
		checksum += util_deint(4, &color);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_MSG_recv(fd, index, message, color);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PMSG_RECV) {
		int checksum=0, checksumrecv;
		int index;
		int petindex;
		int itemindex;
		char message[65500];
		int color;

		checksum += util_deint(2, &index);
		checksum += util_deint(3, &petindex);
		checksum += util_deint(4, &itemindex);
		checksum += util_destring(5, message);
		checksum += util_deint(6, &color);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PMSG_recv(fd, index, petindex, itemindex, message, color);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_AB_RECV) {
		int checksum=0, checksumrecv;
		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_AB_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DAB_RECV) {
		int checksum=0, checksumrecv;
		int index;

		checksum += util_deint(2, &index);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DAB_recv(fd, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_AAB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_AAB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_L_RECV) {
		int checksum=0, checksumrecv;
		int dir;

		checksum += util_deint(2, &dir);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_L_recv(fd, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_TK_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char message[65500];
		int color;
		int area;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, message);
		checksum += util_deint(5, &color);
		checksum += util_deint(6, &area);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_TK_recv(fd, x, y, message, color, area);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_M_RECV) {
		int checksum=0, checksumrecv;
		int fl;
		int x1;
		int y1;
		int x2;
		int y2;

		checksum += util_deint(2, &fl);
		checksum += util_deint(3, &x1);
		checksum += util_deint(4, &y1);
		checksum += util_deint(5, &x2);
		checksum += util_deint(6, &y2);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_M_recv(fd, fl, x1, y1, x2, y2);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_C_RECV) {
		int checksum=0, checksumrecv;
		int index;

		checksum += util_deint(2, &index);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_C_recv(fd, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_S_RECV) {
		int checksum=0, checksumrecv;
		char category[65500];

		checksum += util_destring(2, category);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_S_recv(fd, category);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FS_RECV) {
		int checksum=0, checksumrecv;
		int flg;

		checksum += util_deint(2, &flg);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_FS_recv(fd, flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_HL_RECV) {
		int checksum=0, checksumrecv;
		int flg;

		checksum += util_deint(2, &flg);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_HL_recv(fd, flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PR_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int request;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &request);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PR_recv(fd, x, y, request);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_KS_RECV) {
		int checksum=0, checksumrecv;
		int petarray;
		checksum += util_deint(2, &petarray);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_KS_recv(fd, petarray);
		util_DiscardMessage();
		return 0;
	}

#ifdef _STANDBYPET
	if (func==LSSPROTO_SPET_RECV) {
		int checksum=0, checksumrecv;
		int standbypet;
		checksum += util_deint(2, &standbypet);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SPET_recv(fd, standbypet);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _RIGHTCLICK
	if (func==LSSPROTO_RCLICK_RECV) {
		int checksum=0, checksumrecv;
		int type;
		char data[1024];
		checksum += util_deint(2, &type);
		checksum += util_destring(3, data);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_RCLICK_recv(fd, type, data);
		util_DiscardMessage();
		return 0;
	}
#endif

	if (func==LSSPROTO_AC_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int actionno;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &actionno);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_AC_recv(fd, x, y, actionno);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MU_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int array;
		int toindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &array);
		checksum += util_deint(5, &toindex);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_MU_recv(fd, x, y, array, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PS_RECV) {
		int checksum=0, checksumrecv;
		int havepetindex;
		int havepetskill;
		int toindex;
		char data[65500];

		checksum += util_deint(2, &havepetindex);
		checksum += util_deint(3, &havepetskill);
		checksum += util_deint(4, &toindex);
		checksum += util_destring(5, data);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PS_recv(fd, havepetindex, havepetskill, toindex, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ST_RECV) {
		int checksum=0, checksumrecv;
		int titleindex;

		checksum += util_deint(2, &titleindex);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_ST_recv(fd, titleindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DT_RECV) {
		int checksum=0, checksumrecv;
		int titleindex;

		checksum += util_deint(2, &titleindex);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DT_recv(fd, titleindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FT_RECV) {
		int checksum=0, checksumrecv;
		char data[65500];

		checksum += util_destring(2, data);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_FT_recv(fd, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SKUP_RECV) {
		int checksum=0, checksumrecv;
		int skillid;

		checksum += util_deint(2, &skillid);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SKUP_recv(fd, skillid);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_KN_RECV) {
		int checksum=0, checksumrecv;
		int havepetindex;
		char data[65500];

		checksum += util_deint(2, &havepetindex);
		checksum += util_destring(3, data);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_KN_recv(fd, havepetindex, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_WN_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int seqno;
		int objindex;
		int select;
		char data[65500];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &seqno);
		checksum += util_deint(5, &objindex);
		checksum += util_deint(6, &select);
		checksum += util_destring(7, data);
		
		util_deint(8, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_WN_recv(fd, x, y, seqno, objindex, select, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SP_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &dir);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SP_recv(fd, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CLIENTLOGIN_RECV) {
		int checksum=0, checksumrecv;
		char cdkey[65500];
		char passwd[65500];
		
		strcpy(PersonalKey, _DEFAULT_PKEY);

		checksum += util_destring(2, cdkey);
		checksum += util_destring(3, passwd);
		
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		
		lssproto_ClientLogin_recv(fd, cdkey, passwd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CREATENEWCHAR_RECV) {
		int checksum=0, checksumrecv;
		int dataplacenum;
		char charname[65500];
		int imgno;
		int faceimgno;
		int vital;
		int str;
		int tgh;
		int dex;
		int earth;
		int water;
		int fire;
		int wind;
		int hometown;

		checksum += util_deint(2, &dataplacenum);
		checksum += util_destring(3, charname);
		checksum += util_deint(4, &imgno);
		checksum += util_deint(5, &faceimgno);
		checksum += util_deint(6, &vital);
		checksum += util_deint(7, &str);
		checksum += util_deint(8, &tgh);
		checksum += util_deint(9, &dex);
		checksum += util_deint(10, &earth);
		checksum += util_deint(11, &water);
		checksum += util_deint(12, &fire);
		checksum += util_deint(13, &wind);
		checksum += util_deint(14, &hometown);
		util_deint(15, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CreateNewChar_recv(fd, dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARDELETE_RECV) {
		int checksum=0, checksumrecv;
		char charname[65500];

		checksum += util_destring(2, charname);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CharDelete_recv(fd, charname);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLOGIN_RECV) {
		int checksum=0, checksumrecv;
		char charname[65500];

		checksum += util_destring(2, charname);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CharLogin_recv(fd, charname);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLIST_RECV) {
		int checksum=0, checksumrecv;
#ifdef _PKSEVER_VER
		int star = 0;
		util_deint(2, &checksumrecv);
		util_deint(3, &star);
#else
		util_deint(2, &checksumrecv);
#endif
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}

//#ifdef _PKSEVER_VER
//		lssproto_CharList_recv( fd, star);
//#else
		lssproto_CharList_recv( fd);
//#endif

		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLOGOUT_RECV) {
		int checksum=0, checksumrecv;
		int Flg=1;
#ifdef _CHAR_NEWLOGOUT
		checksum += util_deint(2, &Flg);
		util_deint(3, &checksumrecv);
#else
		util_deint(2, &checksumrecv);
#endif
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CharLogout_recv(fd, Flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PROCGET_RECV) {
		int checksum=0, checksumrecv;
		
		strcpy( PersonalKey, _DEFAULT_PKEY);

		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_ProcGet_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PLAYERNUMGET_RECV) {
		int checksum=0, checksumrecv;
		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PlayerNumGet_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ECHO_RECV) {
		int checksum=0, checksumrecv;
		char test[65500];

		checksum += util_destring(2, test);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_Echo_recv(fd, test);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SHUTDOWN_RECV) {
		int checksum=0, checksumrecv;
		char passwd[65500];
		int min;

		checksum += util_destring(2, passwd);
		checksum += util_deint(3, &min);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_Shutdown_recv(fd, passwd, min);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_TD_RECV) {
		int checksum=0, checksumrecv;
		char message[65500];

		checksum += util_destring(2, message);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_TD_recv(fd, message);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FM_RECV) {
		int checksum=0, checksumrecv;
		char message[65500];

		checksum += util_destring(2, message);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_FM_recv(fd, message);
		util_DiscardMessage();
		return 0;
	}
    
	if (func==LSSPROTO_PETST_RECV) {
		int checksum=0, checksumrecv;
		int nPet;
		int sPet;		

		checksum += util_deint(2, &nPet);
		checksum += util_deint(3, &sPet);		
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_PETST_recv(fd, nPet, sPet);		
		util_DiscardMessage();
		return 0;
	}
    
	// _BLACK_MARKET
	if (func==LSSPROTO_BM_RECV) {
		int checksum=0, checksumrecv;
		int iindex;		

		checksum += util_deint(2, &iindex);		
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_BM_recv(fd, iindex);		
		util_DiscardMessage();
		return 0;
	}

#ifdef _FIX_DEL_MAP           // WON ADD ª±®a©â¦a¹Ï°eºÊº»
	if (func==LSSPROTO_DM_RECV) {
	    char buffer[2];
	    buffer[0] = '\0';
		lssproto_DM_recv( fd );
		util_DiscardMessage();
		return 0;
	}

#endif

#ifdef _CHECK_GAMESPEED
	if (func==LSSPROTO_CS_RECV) {
	    char buffer[2];
	    buffer[0] = '\0';
		lssproto_CS_recv( fd );
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _TEAM_KICKPARTY
	if ( func == LSSPROTO_KTEAM_RECV ) {
		int checksum = 0, checksumrecv;
		int sindex;
		checksum += util_deint( 2, &sindex);
		util_deint( 3, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_KTEAM_recv( fd, sindex);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _MIND_ICON
	if(func==LSSPROTO_MA_RECV){
		int checksum = 0, checksumrecv;
		int nMind;
		int x, y;
		
		checksum += util_deint( 2, &nMind);
		checksum += util_deint( 3, &x);
		checksum += util_deint( 4, &y);
		util_deint( 5, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_MA_recv(fd, x, y, nMind);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _CHATROOMPROTOCOL			// (¤£¥i¶}) Syu ADD ²á¤Ñ«ÇÀW¹D
	if (func==LSSPROTO_CHATROOM_RECV) {
		int checksum=0, checksumrecv;
		char test[65500];

		checksum += util_destring(2, test);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CHATROOM_recv(fd, test);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _NEWREQUESTPROTOCOL			// (¤£¥i¶}) Syu ADD ·s¼WProtocol­n¨D²Ó¶µ
	if ( func==LSSPROTO_RESIST_RECV ) {
	    char buffer[2];
	    buffer[0] = '\0';
		lssproto_RESIST_recv( fd );
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _OUTOFBATTLESKILL			// (¤£¥i¶}) Syu ADD «D¾Ô°«®É§Þ¯àProtocol
	if(func==LSSPROTO_BATTLESKILL_RECV){
		int checksum = 0, checksumrecv;
		int iNum;
		
		checksum += util_deint( 2, &iNum);
		util_deint( 3, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_BATTLESKILL_recv(fd, iNum);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _STREET_VENDOR
	if(func == LSSPROTO_STREET_VENDOR_RECV){
		int checksum = 0,checksumrecv;
		char message[65500];

		checksum += util_destring(2,message);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_STREET_VENDOR_recv(fd,message);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _JOBDAILY
	if(func == LSSPROTO_JOBDAILY_RECV){
		int checksum = 0,checksumrecv;
		char buffer[16384];

		buffer[0] = '\0';
		memset(buffer,0,16384); //kkkkkkkkk
		checksum += util_destring(2,buffer);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		print("\njobdaily:%s ",buffer);
		lssproto_JOBDAILY_recv(fd,buffer);
		util_DiscardMessage();
		return 0;

	}
#endif
#ifdef _TEACHER_SYSTEM
	if(func == LSSPROTO_TEACHER_SYSTEM_RECV){
		int checksum = 0,checksumrecv;
		char message[65500];

		checksum += util_destring(2,message);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_TEACHER_SYSTEM_recv(fd,message);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _ADD_STATUS_2
	if(func == LSSPROTO_S2_RECV){
		return 0;
	}
#endif

	util_DiscardMessage();
	logHack(fd,HACK_NOTDISPATCHED);
	DME(); return -1;
}

/*
   servertoclient XYD( int x, int y, int dir );
 	¼ç¤ËÀïÆ®½ªÎ»¸å¤Ë¥×¥ì¥¤¥ä¡¼¤Î°ÌÃÖ¤òÈùÄ´À°¤¹¤ë¤¿¤á¤Ë»È¤¦¡£
*/
void lssproto_XYD_send(int fd,int x,int y,int dir)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, dir);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_XYD_SEND, buffer);
}

void lssproto_EV_send(int fd,int seqno,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, seqno);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EV_SEND, buffer);
}

void lssproto_EN_send(int fd,int result,int field)
{
	char buffer[65500];
	int checksum=0;

	//print(" EN_send ");

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, result);
	checksum += util_mkint(buffer, field);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EN_SEND, buffer);
}


void lssproto_RS_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RS_SEND, buffer);
}


void lssproto_RD_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RD_SEND, buffer);
}

void lssproto_B_send(int fd,char* command)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, command);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_B_SEND, buffer);
}

void lssproto_I_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_I_SEND, buffer);
}

void lssproto_SI_send(int fd,int fromindex,int toindex)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fromindex);
	checksum += util_mkint(buffer, toindex);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SI_SEND, buffer);
}

void lssproto_MSG_send(int fd,int aindex,char* text,int color)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, aindex);
	checksum += util_mkstring(buffer, text);
	checksum += util_mkint(buffer, color);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_MSG_SEND, buffer);
}


void lssproto_PME_send(int fd,int objindex,int graphicsno,int x,int y,int dir,int flg,int no,char* cdata)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, objindex);
	checksum += util_mkint(buffer, graphicsno);
	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, dir);
	checksum += util_mkint(buffer, flg);
	checksum += util_mkint(buffer, no);
	checksum += util_mkstring(buffer, cdata);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PME_SEND, buffer);
}


void lssproto_AB_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_AB_SEND, buffer);
}


void lssproto_ABI_send(int fd,int num,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, num);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_ABI_SEND, buffer);
}

void lssproto_TK_send(int fd,int index,char* message,int color)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, index);
	checksum += util_mkstring(buffer, message);
	checksum += util_mkint(buffer, color);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_TK_SEND, buffer);
}

void lssproto_MC_send(int fd,int fl,int x1,int y1,int x2,int y2,int tilesum,int objsum,int eventsum,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fl);
	checksum += util_mkint(buffer, x1);
	checksum += util_mkint(buffer, y1);
	checksum += util_mkint(buffer, x2);
	checksum += util_mkint(buffer, y2);
	checksum += util_mkint(buffer, tilesum);
	checksum += util_mkint(buffer, objsum);
	checksum += util_mkint(buffer, eventsum);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_MC_SEND, buffer);
}


void lssproto_M_send(int fd,int fl,int x1,int y1,int x2,int y2,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fl);
	checksum += util_mkint(buffer, x1);
	checksum += util_mkint(buffer, y1);
	checksum += util_mkint(buffer, x2);
	checksum += util_mkint(buffer, y2);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_M_SEND, buffer);
}

/*
   servertoclient C( string data );
       (Characters)
       ¥¯¥é¥¤¥¢¥ó¥È¤Ï CA ¤¬Íè¤Æ¡¢¤½¤Î¥­¥ã¥é¤Ë¤Ä¤¤¤ÆÄ´¤Ê¤«¤Ã¤¿»þ¤Ï¡¢ C 
       ¤Ç¤³¤Î´Ø¿ô¤òÍ×µá¤Ç¤­¤ë¡£¥µ¡¼¥Ð¤Ï¥Á¥§¥Ã¥¯¤·¤Ê¤¤¡£
       
 	string data
 		  ¥Õ¥©¡¼¥Þ¥Ã¥È¤Ï °Ê²¼¤Î3¼ïÎà¤Î¹àÌÜ¤ò¥³¥ó¥Þ¤Ç¤Ê¤é¤Ù¤¿
 	    ¤â¤Î¤Ç¤¢¤ë.¤½¤ì¤¾¤ì¤Î¹àÌÜ¤ÎÃæ¿È¤Ï¤µ¤é¤Ë¤¿¤ÆËÀ'|'¤Ç¤¯¤®¤é¤ì
 	    ¤Æ¤¤¤ë¡£
 	    
 	      ¤¿¤ÆËÀ¤Ç¤¯¤®¤é¤ì¤¿¥È¡¼¥¯¥ó¤¬12¸Ä¤Î¾ì¹ç
 		  ¥È¡¼¥¯¥ó¤ÎÆâÍÆ¤Ï
 
 		  WHICHTYPE|CHARINDEX|X|Y|DIR|BASEIMG|LEVEL|NAMECOLOR|NAME|SELFTITLE|WALKABLE|HEIGHT|POPUPNAMECOLOR
 		  ¤³¤Î¥ª¥Ö¥¸¥§¥¯¥È¤Ï¥­¥ã¥é¥¯¥¿¡¼¤Ç¤¢¤ë¤È¤¤¤¦¤³¤È¤Ç¤¢¤ë¡£
 		  
 		  WHICHTYPE ¤Ï¡¤¤³¤Î¥­¥ã¥é¥¯¥¿¡¼¤¬¤É¤¦¤¤¤Ã¤¿¼ïÎà¤Î¤â¤Î
 		  ¤Ç¤¢¤ë¤«¡£¤³¤ì¤Ï¥µ¡¼¥Ð¡¼¤Ç¤Ï°Ê²¼¤Î¤è¤¦¤ËÄêµÁ¤µ¤ì¤Æ¤¤¤ë¡£
 		  
 		typedef enum
 		{
 		    CHAR_TYPENONE,          ²¿¤Ç¤â¤Ê¤¤
 		    CHAR_TYPEPLAYER,        ¥×¥ì¥¤¥ä¡¼
 		    CHAR_TYPEENEMY,         Å¨
 		    CHAR_TYPEPET,           ¥Ú¥Ã¥È
 		    CHAR_TYPEDOOR,          ¥É¥¢
 		    CHAR_TYPEBOX ,          ÊõÈ¢
 		    CHAR_TYPEMSG ,          ´ÇÈÄ
 		    CHAR_TYPEWARP ,         ¥ï¡¼¥×¥¾¡¼¥ó
 		    CHAR_TYPESHOP ,         Å¹
 		    CHAR_TYPEHEALER ,       ¥Ò¡¼¥é¡¼
 		    CHAR_TYPEOLDMAN ,       Ä¹Ï·
 		    CHAR_TYPEROOMADMIN,     ÉÔÆ°»º²°
 		    CHAR_TYPETOWNPEOPLE,    ¤Þ¤Á¤Î¤Ò¤È
 		    CHAR_TYPEDENGON,        ÅÁ¸ÀÈÇ
 		    CHAR_TYPEADM,           ÅÁ¸ÀÈÇ
 		    CHAR_TYPETEMPLE,        Temple master
 		    CHAR_TYPESTORYTELLER,   ¸ì¤êÉô
 		    CHAR_TYPERANKING,       ÉÔÆ°»º¥é¥ó¥­¥ó¥°É½¼¨£Î£Ð£Ã
 		    CHAR_TYPEOTHERNPC,      ¤½¤ÎÂ¾¤Î¸¡º÷ÂÐ¾Ý¤Ë¤Ê¤é¤Ê¤¤NPC
 		    CHAR_TYPEPRINTPASSMAN,  ¥É¥¢¤Î¥Ñ¥¹¥ï¡¼¥ÉÉ½¼¨¤¹¤ëNPC
 		    CHAR_TYPENPCENEMY,      ¸ÇÄêÅ¨
 		    CHAR_TYPEACTION,        ¥¢¥¯¥·¥ç¥ó¤ËÈ¿±þ¤¹¤ëNPC
 		    CHAR_TYPEWINDOWMAN,     ¥¦¥£¥ó¥É¥¦É½¼¨¤¹¤ëNPC¡Ê¥Æ¥¹¥È¤«¤â)
 		    CHAR_TYPESAVEPOINT,     ¥»¡¼¥Ö¥Ý¥¤¥ó¥È
 		    CHAR_TYPEWINDOWHEALER,  ¥¦¥¤¥ó¥É¥¦¥¿¥¤¥×¤Î¥Ò¡¼¥é¡¼
 		    CHAR_TYPEITEMSHOP,	    ¤ªÅ¹
 		    CHAR_TYPESTONESHOP,	    ÀÐÈ×²°¡Ê¥Ú¥Ã¥È¤Îµ»²°¡Ë
 		    CHAR_TYPEDUELRANKING,   DUEL¥é¥ó¥­¥ó¥°NPC
 		    CHAR_TYPEWARPMAN,	    ¥ï¡¼¥×¥Þ¥óNPC
 		    CHAR_TYPEEVENT,	    ¥¤¥Ù¥ó¥ÈNPC
 		    CHAR_TYPEMIC,	    ¥¤¥Ù¥ó¥ÈNPC
 		    CHAR_TYPELUCKYMAN,	    ¥¤¥Ù¥ó¥ÈNPC
 		    CHAR_TYPEBUS,	    ¥Þ¥ó¥â¥¹¥Ð¥¹
 		    CHAR_TYPECHARM,	    ¥¤¥Ù¥ó¥ÈNPC
 		    CHAR_TYPENUM,
 		}CHAR_TYPE;
 		  
 		  ¤½¤ì¤ÏÂ¾¤Î¥×¥ì¥¤¥ä¡¼¤äNPC¤äÅ¨¤Ç¤¢¤ë¡£¥µ¡¼¥Ð¡¼¤¬Á÷¿®
 		  ¤¹¤ë¤È¤­¤Ï¸«¤¨¤ëÈÏ°Ï¤Ë¤Ä¤¤¤ÆÁ´ÉôÁ÷¿®¤¹¤ë¡£¤Ä¤Þ
 		  ¤ê¥¯¥é¥¤¥¢¥ó¥È¤Ï¡¢¤³¤Î¥Ñ¥±¥Ã¥È¤ò¼õ¤±¤È¤Ã¤¿¤È¤­¤Ë¤³¤Î
 		  ¥Ñ¥±¥Ã¥È¤Ë½ñ¤«¤ì¤Æ¤¤¤Ê¤¤¥­¥ã¥é¤ò»ý¤Ã¤Æ¤¤¤¿¤é¾Ã¤·¤Æ¤·
 		  ¤Þ¤Ã¤Æ¤è¤¤¤È¤¤¤¦¤³¤È¤Ç¤¢¤ë¡£¤Þ¤¿¡¢¥Þ¥¦¥¹¥«¡¼¥½¥ë¤ò¤¢
 		  ¤ï¤»¤¿¤È¤­¤ËÉ½¼¨¤¹¤ë¾ðÊó¤Ï¤³¤Î¾ðÊó¤Î¤ß¤Ë´ð¤¤¤Æ¤¤¤ë¡£
 		  ¤À¤«¤é¡¢¥Þ¥¦¥¹¥«¡¼¥½¥ë¤ò¤¢¤ï¤»¤¿¤È¤­¤ËÉ½¼¨¤¹¤ë¤³¤È¤¬
 		  ÊÑ¹¹¤µ¤ì¤¿¾ì¹ç¤Ï¡¢¥µ¡¼¥Ð¡¼¤Ï¡¢¤³¤Î´Ø¿ô¤òÇ½Æ°Åª¤Ë¸Æ¤Ó
 		  ¤À¤µ¤Ê¤±¤ì¤Ð¤Ê¤é¤Ê¤¤¡£ SELFTITLE¤Ë¤Ä¤¤¤Æ¤Ï¡¢¥Ç¥ê¥ß¥¿
 		  ¤Ç¤¢¤ë¤¿¤ÆËÀ¤ò¤Õ¤¯¤Þ¤Ê¤¤¤è¤¦¤Ë¥¨¥¹¥±¡¼¥×¤µ¤ì¤Æ¤¤¤ë¡£
 		  ¥¯¥é¥¤¥¢¥ó¥È¤Ï¥¢¥¯¥·¥ç¥ó¤Î¥³¥Þ¥ó¥É(CA)¤¬¤¯¤ë¤Þ¤Ç¤ÏÎ©
 		  ¤Á¤ÇÉ½¼¨¤¹¤ë¡£CHARINDEX¥µ¡¼¥Ð¡¼Æâ¤Î°ì°Õ¤Ë¥­¥ã¥é¤òÆÃ
 		  Äê¤Ç¤­¤ëÈÖ¹æ¡¢BASEIMG¤ÏÉ½¼¨¤Î¤¿¤á¤ÎÈÖ¹æ¡¢LEVEL¤Ï¥­¥ã
 		  ¥é¤Î¥ì¥Ù¥ë(0¤Ê¤éÉ½¼¨¤·¤Ê¤¤¡£¤³¤ÎÃÍ¤ÏNPC¤Ê¤É¤Ë»È¤¦¡£)
 		  WALKABLE¤Ï1¤Î¤È¤­¤½¤Î¾å¤òÄÌ²á¤¹¤ë¤³¤È¤¬¤Ç¤­¡¢0¤Ê¤éÄÌ
 		  ²á¤¹¤ë¤³¤È¤¬¤Ç¤­¤Ê¤¤¡£HEIGHT¤Ï¹â¤µ¤ò¤â¤Ä¤â¤Î¤«¤½¤¦¤Ç
 		  ¤Ê¤¤¤Î¤«¤Î»ØÄê¡£
 		  
 		  ¥­¥ã¥é¥¯¥¿¡¼¤ÎÌ¾Á°¤È¼«Í³¾Î¹æ¤Ï¡¢<a href="#escaping">
 		  ¥¨¥¹¥±¡¼¥×¤µ¤ì¤Ê¤±¤ì¤Ð¤Ê¤é¤Ê¤¤¡£'|'¤Ç¥È¡¼¥¯¥ó¤ò
 		  ¼è¤ê¤À¤·¤Æ¤«¤é¥¨¥¹¥±¡¼¥×¤ò²ò½ü¤¹¤ë¡£¥¨¥¹¥±¡¼¥×¤¹¤ë¤È'
 		  |'¤¬¤Û¤«¤ÎÊ¸»ú¤Ë¤ª¤­¤«¤ï¤ë¤Î¤Ç¡¢ºÇ½é¤ÏÃ±½ã¤Ë'|'¤ò¥Ç
 		  ¥ê¥ß¥¿¤È¤·¤Æ¤è¤¤¡£Á÷¿®¤¹¤ëÊý¤â¡¢Ì¾Á°¤È¼«Í³¾Î¹æ¤ò¥¨¥¹
                   ¥±¡¼¥×¤·¤Æ¤«¤é¤¿¤ÆËÀ¤Ç¤Ä¤Ê¤¤¤Ç¤«¤éÁ÷¿®¤¹¤ë¡£
                   ¤Þ¤¿¡¤¥Ú¥Ã¥È¤Î¾ì¹ç¤Ï¼«Í³¾Î¹æ¤ÎÂå¤ï¤ê¤Ë¥æ¡¼¥¶¡¼¤¬ÀßÄê
                   ¤·¤¿¥Ú¥Ã¥È¤ÎÌ¾Á°¤¬Á÷¿®¤µ¤ì¤Æ¤¯¤ë¡£
 		  
 	      ¤¿¤ÆËÀ¤Ç¤¯¤®¤é¤ì¤¿¥È¡¼¥¯¥ó¤¬6¸Ä¤Î¾ì¹ç
 		  ¥È¡¼¥¯¥ó¤ÎÆâÍÆ¤Ï
 		  INDEX|X|Y|BASEIMG|LEVEL|ITEM1LINEINFO
 		  ¤ÇÃÏÌÌ¤ËÍî¤Á¤Æ¤¤¤ë¥¢¥¤¥Æ¥à¤Ë¤Ä¤¤¤Æ¤Î¾ðÊó¤Ç¤¢¤ë¡£
 		  INDEX¤Ï¥­¥ã¥é¤Î¥¤¥ó¥Ç¥¯¥Ã¥¹¤È¤«¤Ö¤é¤Ê¤¤INDEX¤Ç¤¢¤ë¡£
 		  ¤½¤Î¥¢¥¤¥Æ¥à¤ò¾Ã¤¹»þ¤Ë»ÈÍÑ¤¹¤ë¡£X,Y¤Ï¥¢¥¤¥Æ¥à¤Î¥°¥í¡¼
 		  ¥Ð¥ë°ÌÃÖ¡£BASEIMG¤Ï²èÁü¤ÎÈÖ¹æ¡£ITEM1LINEINFO¤Ï1¹Ô
 		  info¤ËÉ½¼¨¤¹¤ë¤¿¤á¤Î¾ðÊó¤Ç¤¢¤ë¡£¥¢¥¤¥Æ¥à¥¦¥¤¥ó¥É¥¦Æâ
 		  ¤ÎÉ½¼¨ÍÑ¤Î¾ðÊó¤ÏÊÌ¤ÎÊýË¡¤ÇÍÑ°Õ¤¹¤ë¡£¥¢¥¤¥Æ¥à¤Ë´Ø¤·¤Æ
 		  ¤ÏCA¤ÏÍè¤Ê¤¤¡£ITEM1LINEINFO¤Ï<a href="#escape">¥¨¥¹¥±¡¼
 		  ¥×¤µ¤ì¤ë¡£¤³¤Î¥¨¥¹¥±¡¼¥×¤ÎÊýË¡¤Ï¾å¤Î¹àÌÜ¤ò»²¾È¡£
 
 	      ¤¿¤ÆËÀ¤Ç¤¯¤®¤é¤ì¤¿¥È¡¼¥¯¥ó¤¬4¸Ä¤Î¾ì¹ç
 		  ¥È¡¼¥¯¥ó¤ÎÆâÍÆ¤Ï
 		  INDEX|X|Y|VALUE
 		  ¤ÇÃÏÌÌ¤ËÍî¤Á¤Æ¤¤¤ë¤ª¶â¤Ë¤Ä¤¤¤Æ¤Î¾ðÊó¤Ç¤¢¤ë¡£ÆâÍÆ¤Ï¤¹
 		  ¤Ù¤Æ¿ô»ú¡£INDEX,X,Y ¤Ï¥¢¥¤¥Æ¥à¤ÈÆ±¤¸¡£VALUE ¤Ï¤É¤ì¤À
 		  ¤±¤ÎÎÌ¤«¤È¤¤¤¦»ö¤Ç¤¢¤ë¡£¥¢¥¤¥Æ¥à¤Ë¤Ä¤¤¤Æ¤Î¾ðÊó¤Ç¤¢¤ë¡£
 	      ¤¿¤ÆËÀ¤Ç¤¯¤®¤é¤ì¤¿¥È¡¼¥¯¥ó¤¬1¸Ä¤Î¾ì¹ç
 		  INDEX
 		  ¤³¤Î¥­¥ã¥é¤ÎC¤Ï¶µ¤¨¤é¤ì¤Ê¤¤¡£
*/
void lssproto_C_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_C_SEND, buffer);
}

/*
   servertoclient CA( string data );
       (CharacterAction)
       ¸«¤¨¤ëÈÏ°Ï¤Ë¤¤¤ë¥­¥ã¥é¤Î¥¢¥¯¥·¥ç¥ó¾õÂÖ¤ò¹¹¿·¤¹¤ë¡£
       ¥µ¡¼¥Ð¡¼¤«¤é¥¯¥é¥¤¥¢¥ó¥È¤Ë°ìÊýÅª¤ËÁ÷¿®¤¹¤ë¡£
       ³Æ¥­¥ã¥é¤Î1¥¢¥¯¥·¥ç¥ó¤´¤È¤ËÁ÷¿®¡£¥µ¡¼¥Ð¡¼¤Ï¥¢¥¯¥·¥ç¥ó¤ò¤Ç¤­¤ë¤À
       ¤±°µ½Ì¤¹¤ë¤³¤È¡£
       
 	string data
 	CHARINDEX|X|Y|ACTION|PARAM1|PARAM2|PARAM3|PARAM4|....¤ò¥³
 	    ¥ó¥Þ¤Ç¤¯¤®¤Ã¤¿¤â¤Î¤Ë¤¹¤ë¡£PARAM¤Ï¥¢¥¯¥·¥ç¥ó¤´¤È¤Ë¸Ä¿ô¤â
 	    »È¤¤¤«¤¿¤â¤³¤È¤Ê¤ë¡£°Ê²¼¤Ï¥¢¥¯¥·¥ç¥ó°ìÍ÷¡£X,Y¤Ï°ÌÃÖ¤Ç¤É
 	    ¤Î¥¢¥¯¥·¥ç¥ó¤Ç¤â¡¢¤¢¤¿¤é¤·¤¤°ÌÃÖ¤ò»ØÄê¤¹¤ë¡£
 	    ¤³¤ÎÊ¸»úÎó¤Ï¥¨¥¹¥±¡¼¥×¤µ¤ì¤Ê¤¤¡£
 	  
 	        ACTION  PARAM1  PARAM2 
 	       PARAM3  PARAM4  
 	        Stand:0  Êý¸þ0~7        
 	        Walk:1  Êý¸þ0~7        
 	        Attack:2  Êý¸þ0~7        
 	        Throw:3  Êý¸þ0~7        
 	        Damage:4  Êý¸þ0~7      
 	        Dead:5  Êý¸þ0~7        
 	        UseMagic:6  Êý¸þ0~7  
 	        UseItem:7  Êý¸þ0~7  
 	        Effect:8  Êý¸þ0~7  ¥¨¥Õ¥§¥¯¥ÈÈÖ¹æ  
 	        Down:10 (ÅÝ¤ì¤ë)  Êý¸þ0~7  
 	        Sit:11 (ºÂ¤ë)  Êý¸þ0~7  
 	        Hand:12 (¼ê¤ò¿¶¤ë)  Êý¸þ0~7  
 	        Pleasure:13 (´î¤Ö)  Êý¸þ0~7  
 	        Angry:14 (ÅÜ¤ë)  Êý¸þ0~7  
 	        Sad:15 (Èá¤·¤à)  Êý¸þ0~7  
 	        Guard:16 (¥¬¡¼¥É)  Êý¸þ0~7  
 	        actionwalk:17 (¥¢¥¯¥·¥ç¥óÍÑÊâ¤­)  Êý¸þ0~7  
 	        nod:18 (¤¦¤Ê¤º¤¯)  Êý¸þ0~7  
 	        actionstand:19 (¥¢¥¯¥·¥ç¥óÍÑÎ©¤Á¥Ý¡¼¥º)  Êý¸þ0~7  
 	        Battle:20 (ÀïÆ®¾ðÊó)  Êý¸þ0~7  BattleNo(-1 ¤Ê¤éÉ½¼¨¾Ã¤¹¡Ë  SideNo  HelpNo¡Ê£±¤Ê¤é½õ¤±¤ò¸Æ¤ÖCAÉ½¼¨¡¤£°¤Ê¤é¾Ã¤¹¡¤¤Þ¤¿¤ÏÌµ¤·¡Ë  
 	        Leader:21 (¥ê¡¼¥À¡¼¾ðÊó)  Êý¸þ0~7  0:É½¼¨¾Ã¤¹ 1:É½¼¨  
 	        Watch:22 (ÀïÆ®´ÑÀï)  Êý¸þ0~7  0:É½¼¨¾Ã¤¹ 1:É½¼¨  
 	        namecolor:23(Ì¾Á°¤Î¿§¾ðÊó)  Êý¸þ0~7  Ì¾Á°¤Î¿§ÈÖ¹æ  
 	        Turn:30(Êý¸þÊÑ´¹)  Êý¸þ0~7    
 	        Warp:31(¥ï¡¼¥×)  Êý¸þ0~7    
 	      
 	ACTION¤ÎÃÍ¤ÏÀ°¿ô¤Ç¡¢ÆâÍÆ¤Ï¾å¤ÎÉ½¤Îº¸Ã¼¤Î¹àÌÜ¤Ç¤¢¤ë¡£
*/
void lssproto_CA_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CA_SEND, buffer);
}

/*
   servertoclient CD( string data );
       (CharacterDelete)
 	data ¤Ï¥Ç¥ê¥ß¥¿¡Ê,¡Ë¤Ç¶èÀÚ¤é¤ì¤¿¥¤¥ó¥Ç¥Ã¥¯¥¹¡£
 	Ê£¿ô·ïÁ÷¤ë»ö¤¬½ÐÍè¤ë¡£
       ¤³¤ÎID¤ò»ý¤Ã¤Æ¤¤¤ë¥­¥ã¥é¥¯¥¿¤¬¾Ã¤¨¤¿»þ¤Ë¥µ¡¼¥Ð¤«¤é¥¯¥é¥¤¥¢¥ó¥È¤Ø
       ¤ÈÅÁ¤ï¤ë¡£
*/
void lssproto_CD_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CD_SEND, buffer);
}

/*
   <a name="R">servertoclient R( string data );
       (Radar)
       ¥ì¡¼¥À¡¼¤ÎÆâÍÆ¤ò¥¯¥é¥¤¥¢¥ó¥È¤ËÁ÷¿®¤¹¤ë¡£¥¯¥é¥¤¥¢¥ó¥È¤Ï¤³¤ÎÁ÷¿®¤ò
       Í×µá¤¹¤ë¤³¤È¤Ï¤Ê¤¤¡£¥µ¡¼¥Ð¡¼¤¬Å¬Åö¤Ê¥¿¥¤¥ß¥ó¥°¤ÇÁ÷¿®¤¹¤ë¡£
       ¤¿¤È¤¨¤Ð10Êâ¤¢¤ë¤¯¤´¤È¤È¤«¡¢1Ê¬¤´¤È¤È¤«¡£
 
 	string data
 	x ,y, kind ,¤Î½ç¤ËÃÍ¤ò'|'¤Ç¤¯¤®¤Ã¤Æ¤Ê¤é¤Ù¤¿¤â¤Î¤ò
 	    ¤µ¤é¤Ë'|'¤Ç¤Ê¤é¤Ù¤¿¤â¤Î¡£
 	    x,y¤Ï¥­¥ã¥é¤«¤é¤ÎÀäÂÐ°ÌÃÖ¡£kind¤Ï°Ê²¼¤Ë¤·¤á¤¹¿ôÃÍ¤Ç¤Ï¤Ê¤¤
 	    µ­¹æ¡£¥ì¡¼¥À¡¼¤Ë¤¦¤Ä¤ëÊª¤Î¼ïÎà¤ò»ØÄê¤¹¤ë¡£Á÷¤é¤ì¤Æ¤¯¤ë¤â¤Î
 	    ¤Î¼ïÎà¤Ï¡¢¤½¤ì¤Þ¤Ç¤Ë¾§¤¨¤¿ËâË¡¤È¤«¡¢¥¹¥­¥ë¤ÇÊÑ²½¤·¡¢¤½¤ì¤Ï
 	    ¥µ¡¼¥Ð¡¼¤¬¹Í¤¨¤ë¡£
 	 
 	        kind¤ÎÃÍ  ÆâÍÆ  
 	        E  Å¨  
 	        P  ¥×¥ì¥¤¥ä¡¼  
 	        S  º²  
 	        G  ¤ª¶â  
 	        I  ¥¢¥¤¥Æ¥à(º²°Ê³°)  
   
   data¤ÎÎã¡§"12|22|E|13|24|P|14|28|P"
   ¤³¤ÎÊ¸»úÎó¤Ï¥¨¥¹¥±¡¼¥×¤µ¤ì¤Ê¤¤¡£
*/
void lssproto_R_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_R_SEND, buffer);
}

/*
   servertoclient S( string data );
       (Status)
       ¥­¥ã¥é¤Î¥¹¥Æ¡¼¥¿¥¹¤òÁ÷¿®¤¹¤ë¡£
       ¥Ç¡¼¥¿¤Ï ¥«¥Æ¥´¥êµ­¹æÊ¸»ú(°ìÊ¸»ú)ÆâÍÆ ¤È¤Ê¤Ã¤Æ¤¤¤ë¡£¤Ä¤Þ¤êºÇ½é¤Î
       1Ê¸»ú¤ò¸«¤ì¤Ð²¿¤Î¥¹¥Æ¡¼¥¿¥¹¤«Ê¬¤ë¡£ÆâÍÆ¤Ï2Ê¸»úÌÜ¤«¤é¤Ç¤¢¤ë¡£
       ÆâÍÆ¤Ï°Ê²¼¤Î¥Õ¥©¡¼¥Þ¥Ã¥È¤Ë¤·¤¿¤¬¤¦¡£¤¿¤ÆËÀµ­¹æ'|'¤¬¥Ç¥ê¥ß¥¿¤Ç¤¢¤ë¡£
       2¸ÄÌÜ°Ê¹ß¤Î¥È¡¼¥¯¥ó¤¬ÆâÍÆ¤Ç¤¢¤ë¡£
       
 	P Á´¥Ñ¥é¥á¡¼¥¿
 	    kubun hp maxhp mp maxmp str tough exp maxexp exp 
 	    level attackpower deffencepower 
 	    fixdex fixcharm fixluck fixfireat fixwaterat fixearthat fixwindat
 	    gold ÉÕ¤±¤Æ¤¤¤ë
 	    ¾Î¹æ¤Îindex Ì¾Á° ¼«¸Ê¾Î¹æ 
 
 	    ¥Ç¥ê¥ß¥¿¤Ï '|' ¤Ç¤¢¤ë¡£¤Þ¤¿Ì¾Á°¤È¼«¸Ê¾Î¹æ¤Ï¡¢
 	    <a href="#escaping">¥¨¥¹¥±¡¼¥×¤·¤¿¤â¤Î¤ò
 	    ¤¿¤ÆËÀ¤Ç¤Ä¤Ê¤²¤¿¤â¤Î¤¬ÄÌ¿®¤µ¤ì¤ë¡£
 	    ¤½¤ì¤¾¤ì¤ÎÃÍ¤Î·¿¤Ï°Ê²¼¡£
 	    kubun ¤Ë£±¤¬Æþ¤Ã¤Æ¤¤¤ë¤ÈÁ°¥Ñ¥é¥á¡¼¥¿¤òÁ÷¿®¡¤
 	    £²°Ê¾å¤À¤È¡¤³Æ¥Ó¥Ã¥È¤ÎÎ©¤Ã¤Æ¤¤¤ë¤â¤Î¤Î¥Ñ¥é¥á¡¼¥¿¤¬½çÈÖ¤ËÆþ¤Ã¤ÆÁ÷¿®¤µ¤ì¤Æ¤¤¤ë¡£
 	   Îã¤¨¤Ð¡¤£³¤¬Íè¤¿¤éhp¤ÈMaxhp¤¬kubun °Ê¹ß¤Î¥È¡¼¥¯¥ó¤ËÇ¼¤á¤é¤ì¤Æ¤¤¤ë¡£
 	    
 	        kunun  int  
 	        Hp(ÂÎÎÏ)hp  int  
 	        MaxHp  int  
 	        Mp(¼ö½Ñ¥Ñ¥ï¡¼)  int  
 	        MaxMp  int  
 	        Vital(ÂÎÎÏ)  int  
 	        Str(ÏÓÎÏ)  int  
 	        Tough(¾æÉ×¤µ)  int  
 	        Dex(ÁÇÁá¤µ)  int  
 	        Exp(·Ð¸³ÃÍ)exp  int  
 	        MaxExp(¼¡¤Þ¤Ç¤Î·Ð¸³ÃÍ)  int  
 	        Level(¥ì¥Ù¥ë)  int  
 	        Attack(¹¶·âÎÏ)  int  
      	        Defense(¼éÈ÷ÎÏ)  int  
 	        fQuick(ºÇ½ªÁÇÁá¤µ)  int  
 	        fCharm(Ì¥ÎÏ)  int  
 	        fLuck(±¿)  int  
 	        fEarth(ÃÏ)fixearthat  int  
 	        fWater(¿å)  int  
 	        fFire(²Ð)  int  
 	        fWid(É÷)fixwindat  int  
 	        Gold(¤ª¶â)  int  
 	        ¾Î¹æ¤Îindex  int  
 	        ¥Ç¥å¥¨¥ë¥Ý¥¤¥ó¥È  int  
 	        Å¾À¸²ó¿ô  int  
 	        Ì¾Á°  Ê¸»úÎó  
 	        ¼«¸Ê¾Î¹æ  Ê¸»úÎó  
 	    
 	    ¶ñÂÎÎã( char ¤ÎÇÛÎó¤½¤Î¤Þ¤Þ½ñ¤¯ )
 	    
 	    P10|20|10|20|10|10|10|1|2|13|13|1|100|10|¤Ø¤ó¤Ñ|abc|def
 	
 	C ºÂÉ¸
 	    floor maxx maxy x y
 	    ¤ò  | ¤Ç¶èÀÚ¤Ã¤ÆÁ÷¤ë¡£
 	    ¶ñÂÎÎã( char ¤ÎÇÛÎó¤½¤Î¤Þ¤Þ½ñ¤¯ )
 	    
 	    C1024|100|100|10|20
 	    
 	    ¾åµ­¤ÎÎã¤Ç¤Ï¡¢¥Õ¥í¥¢ID1024, ¥µ¥¤¥º100x100 X 10,Y 20¤Î¤È¤³
 	    ¤í¤Ë¥­¥ã¥é¥¯¥¿¡¼¤¬¤¤¤ë¤È¤¤¤¦ÆâÍÆ¤Ë¤Ê¤ë¡£¤³¤Î¥Þ¥¤¥­¥ã¥é¤Î¥Õ
 	    ¥í¥¢¤â¤Õ¤¯¤á¤¿ºÂÉ¸¤ò¥¯¥é¥¤¥¢¥ó¥È¤¬ÃÎ¤ë¤³¤È¤¬¤Ç¤­¤ë¤Î¤Ï¤³¤Î
 	    ¥³¥Þ¥ó¥É¤Î¤ß¤Ç¤¢¤ë¡£Êâ¤­¤Î·ë²Ì¤Ï¥Õ¥í¥¢Æâ¤Î°ÌÃÖ¤·¤«¤ï¤«¤é¤Ê
 	    ¤¤¡£¤è¤Ã¤Æ¡¢¥²¡¼¥à³«»Ï»þ¡¢³¬ÃÊ»ÈÍÑ¤Î¤È¤­¡¢¥ï¡¼¥×¤Î¤È¤­¤Ê¤É
 	    ¤Ï¡¢¤³¤Î¥³¥Þ¥ó¥É¤ò¤Ä¤«¤Ã¤ÆÀµ¤·¤¤ÃÍ¤òÁ÷¿®¤·¤Ê¤¯¤Æ¤Ï¤Ê¤é¤Ê¤¤¡£
 	    
 	I Á´¥¢¥¤¥Æ¥à
 	    (¥¢¥¤¥Æ¥à1)|(¥¢¥¤¥Æ¥à2)...(¥¢¥¤¥Æ¥àn)
 	    ¥¢¥¤¥Æ¥à¤ÎÆâÍÆ¤Ï°Ê²¼¤ÎÊýË¡¤Ç¥Ñ¥Ã¥¯¤¹¤ë¡£
 	    ¤Ò¤È¤Ä¤Î¥¢¥¤¥Æ¥à¤Ï¡¢¤«¤Ê¤é¤º°Ê²¼¤Î6¤Ä¤Î¥È¡¼¥¯¥ó¤Î¥»¥Ã¥È¤Ç
 	    Á÷¿®¤µ¤ì¤ë¡£¤Ò¤È¤Ä¤Ò¤È¤Ä¤ÎÆâÍÆ¤Ï¡¢
 	    
 	    Ì¾Á°|Ì¾Á°2|¥¹¥Æ¡¼¥¸|¥¢¥¤¥Æ¥à¤ÎÀâÌÀ|¥¢¥Ë¥áÈÖ¹æ|»ÈÍÑ²ÄÇ½¾ì½ê|»ÈÍÑÂÐ¾Ý|¥¢¥¤¥Æ¥à¥ì¥Ù¥ë|¥Õ¥é¥°Îà
 	    
 	      ¥¢¥¤¥Æ¥àÌ¾¤Ï¼±ÊÌ¥ì¥Ù¥ë¤Ç¼«Æ°Åª¤ËÊÑ¹¹¤µ¤ì¤ë¡£
 	      Ì¾Á°2¤Ï¼±ÊÌ¥ì¥Ù¥ë¤Ë¤è¤Ã¤Æ¡¢¥¢¥¤¥Æ¥à¤ÎÇ½ÎÏ¤¬Æþ¤ëÍ½Äê¡£
 		  ¥¯¥é¥¤¥¢¥ó¥È¤Î¥¢¥¤¥Æ¥àÍó¤Î2¹ÔÌÜ¤ËÆþ¤ì¤ë»ö¡£
 	      ¥¹¥Æ¡¼¥¸¤ÏÌ¾Á°¤Î¿§¤òÂØ¤ë¤Î¤Ë»È¤¦¡£
 	      ¥¢¥¤¥Æ¥à¤ÎÀâÌÀ¡£
 	      ¥¢¥Ë¥áÈÖ¹æ¤Ï¡¢²èÁüÈÖ¹æ¡£
 	      »ÈÍÑ²ÄÇ½¾ì½ê¤È¤Ï¡¤¤³¤Î¥¢¥¤¥Æ¥à¤¬¤É¤³¤Ç»ÈÍÑ¤¹¤ë»ö¤¬²ÄÇ½¤«¤¬Æþ¤Ã¤Æ¤¤¤ë¡£
 	          ¤³¤ì¤Ï¡¤¥µ¡¼¥Ð¡¼¤Ç¤Ï°Ê²¼¤Î¤è¤¦¤ËÄêµÁ¤µ¤ì¤Æ¤¤¤ë¡£
 	          
 			typedef enum
 			{
 				ITEM_FIELD_ALL,			¤¹¤Ù¤Æ¤Î¾ì½ê¤Ç»È¤¨¤ë
 				ITEM_FIELD_BATTLE,		ÀïÆ®Ãæ¤Î¤ß
 				ITEM_FIELD_MAP,			ÄÌ¾ï¥Þ¥Ã¥×¾å¤Î¤ß
 
 			}ITEM_FIELDTYPE;
 	          
 	      »ÈÍÑÂÐ¾Ý¤È¤Ï¡¤¤³¤Î¥¢¥¤¥Æ¥à¤ò»ÈÍÑ¤¹¤ë¤³¤È¤Î½ÐÍè¤ëÂÐ¾Ý¤¬Æþ¤Ã¤Æ¤¤¤ë¡£
 	          ¥µ¡¼¥Ð¡¼¤Ç¤Ï°Ê²¼¤Î¤è¤¦¤ËÄêµÁ¤µ¤ì¤Æ¤¤¤ë¡£
 	          
 			typedef enum
 			{
 				ITEM_TARGET_MYSELF,		¼«Ê¬¤Î¤ß
 				ITEM_TARGET_OTHER,		Â¾¤Î¿Í¡Ê¼«Ê¬´Þ¤à)
 				ITEM_TARGET_ALLMYSIDE,		Ì£ÊýÁ´ÂÎ
 				ITEM_TARGET_ALLOTHERSIDE,	Áê¼êÂ¦Á´ÂÎ
 				ITEM_TARGET_ALL,		Á´¤Æ
 			}ITEM_TARGETTYPE;
 		
   	        <font size=+1>¤³¤Î¿ô»ú¤Ë100¤òÂ­¤¹¤È¡¤»à¤ó¤Ç¤¤¤ë¼Ô¤âÂÐ¾Ý¤È¤Ê¤ë¡£
 	      ¥¢¥¤¥Æ¥à¥ì¥Ù¥ë¡£¤³¤Î¥ì¥Ù¥ë°Ê¾å¤Î¼Ô¤Ç¤Ê¤¤¤ÈÁõÈ÷½ÐÍè¤Ê¤¤¡£
 	      ¥Õ¥é¥°Îà¡£¿§¤ó¤Ê¥Õ¥é¥°¤òÁ÷¿®¤¹¤ë¡£³Æ¥Ó¥Ã¥È¤ÎÂÐ±þ¤Ï°Ê²¼¤ÎÄÌ¤ê¡£

 		      0bitÌÜ  ¥Ú¥Ã¥È¥á¡¼¥ë¤ÇÁ÷¿®²ÄÇ½¤Ê¥¢¥¤¥Æ¥à¤«¤É¤¦¤«¡£Á÷¿®²ÄÇ½¤À¤È£±¡££°¤À¤ÈÁ÷¿®¤Ç¤­¤Ê¤¤¡£  
 		      1BitÌÜ  ¤³¤Î¥¢¥¤¥Æ¥à¤¬¹çÀ®½ÐÍè¤ë¤«¤É¤¦¤«¡£  
 		      2BitÌÜ  ¤³¤Î¥¢¥¤¥Æ¥à¤¬ÎÁÍý¤«¤É¤¦¤«¡£1¤À¤ÈÎÁÍý¤Ç¤¢¤ë¡£  
 	    
 	    ¤³¤³¤ÎÃæ¤Ç¤Ï<a href="#escaping">¥¨¥¹¥±¡¼¥×¤¹¤ë¡£
 	    ¶ñÂÎÎã( char ¤ÎÇÛÎó¤½¤Î¤Þ¤Þ½ñ¤¯ )
 	    
 	    I¤¢¤Û ¤Ê »Ò|str+2|1|¤¢¤Û\|¤¤¤¸|10|2|0
 	    
 	    ¥¹¥Æ¡¼¥¸¤ÎÃÍ¤Î°ÕÌ£¤Ï¡¢À°¿ôÃÍ¤Ç
 	    
 	        1  ¥¹¥Æ¡¼¥¸A  
 	        2  ¥¹¥Æ¡¼¥¸B  	      
 	        3  ¥¹¥Æ¡¼¥¸C  
 	        4  ¥¹¥Æ¡¼¥¸D  
 	    
 	    ¤È¤¹¤ë¡£¤Þ¤¿¡¢¥¢¥¤¥Æ¥àÉ½¤Î¶õ¤ÎÉôÊ¬¤Ë¤Ä¤¤¤Æ¤Ï¡¢5¸Ä¤Î¥È¡¼¥¯¥ó¤¬
 	    ¶õ¤Ç¡¢¤Ä¤Þ¤ê¤¿¤ÆËÀ¤¬¤Ê¤é¤ó¤À¾õÂÖ¤ÇÁ÷¿®¤µ¤ì¤Æ¤¯¤ë¡£
 	
 	S Á´¥¹¥­¥ë
 	    (¥¹¥­¥ë0)|(¥¹¥­¥ë1)|(¥¹¥­¥ë2) ... (¥¹¥­¥ën)
 	    ¾Î¹æ¤ÈÆ±¤¸¤Ç¡¢¥­¥ã¥é¤¬¤â¤Æ¤ëºÇÂç¥¹¥­¥ë¿ôÊ¬¤òÁ÷¿®¤¹¤ë¡£¤Ä¤Þ
 	    ¤ê¤Ê¤¤¤â¤Î¤Ï ||| ¤È¤Ê¤ë¤Î¤Ç¤¢¤ë¡£
 	    ³Æ¥¨¥ó¥È¥ê¤ÎÆâÍÆ¤Ï¡¢
 	    ¥¹¥­¥ë¤Î¼ïÎà¥³¡¼¥É|¥ì¥Ù¥ë
 	    ¤Ç¤¢¤ë¡£
 	    ¶ñÂÎÎã( char ¤ÎÇÛÎó¤½¤Î¤Þ¤Þ½ñ¤¯ )
 	    
 	    S10|2|20|2|||
 	
 	T ¾Î¹æ
 	    (¾Î¹æ0)|(¾Î¹æ1)|(¾Î¹æ2) ... (¾Î¹æn)
 	    ¾Î¹æ¤Î¥¨¥ó¥È¥ê¤¬¶õ¤Î¾ì¹ç¤Ï '|' ¤¬Ï¢Â³¤¹¤ë¤Î¤Ç¡¢Ï¢Â³¤·¤¿¤â
 	    ¤Î¤ò¾ÊÎ¬¤·¤Æ¥¹¥­¥ã¥ó¤·¤Æ¤Ï¤Ê¤é¤Ê¤¤¡£¤«¤Ê¤é¤ººÇÂç¸Ä¿ôÊ¬Á÷¿®
 	    ¤¹¤ë¡£
 	    ¤Ò¤È¤Ä¡¢¤Ò¤È¤Ä¤ÎÆâÍÆ¤Ï¡¢
 	    Ì¾Á°
 	    ¤Î¤ß¤Ç¤¢¤ë¡£
 	    ¶ñÂÎÎã( Á÷¤é¤ì¤Æ¤¯¤ë char ¤ÎÇÛÎó¤½¤Î¤Þ¤Þ¤Ç¤¢¤ë )
 	    
 	    T¤¢¤Û|¤è¤Ã¤Ñ¤é¤¤
 	    
 	    ¤³¤ì¤â<a href="#escaping">¥¨¥¹¥±¡¼¥×¤¹¤ë¡£
 
 	M ÉôÊ¬¥Ñ¥é¥á¡¼¥¿
 	    ¥­¥ã¥é¥¯¥¿¡¼¤Î¥¹¥Æ¡¼¥¿¥¹¤Î¤¦¤Á¡¢ÉÑÈË¤ËÊÑ¹¹¤µ¤ì¤ë¤â¤Î(¸½ºß
 	    ÃÍ)¤È¤½¤¦¤Ç¤Ê¤¤¤â¤Î(ºÇÂçÃÍ¤Ê¤É)¤¬¤¢¤ë¡£¤É¤ì¤«¤Î¥¹¥Æ¡¼¥¿¥¹
 	    ¤¬ÊÑ¹¹¤µ¤ì¤ë¤¿¤Ó¤ËËè²óÁ´Éô¤Î¥Ñ¥é¥á¡¼¥¿¤òÁ÷¿®¤¹¤ë¤Î¤Ï¥Í¥Ã¥È
 	    ¥ï¡¼¥¯¥È¥é¥Õ¥£¥Ã¥¯¤òÌµÂÌ¤Ë»È¤¦¤³¤È¤Ë¤Ê¤ë¤Î¤Ç¡¢ÆÃ¤ËHP ,
 	    MP,EXP¤ÎÃÍ¤À¤±¤Ë¤·¤Ü¤Ã¤¿¥Ñ¥é¥á¡¼¥¿Á÷¿®¥³¥Þ¥ó¥É¤òÍÑ°Õ¤¹¤ë¡£
 	    ¤½¤ì¤¬¤³¤ÎM¥³¥Þ¥ó¥É¤Ç¤¢¤ë¡£2Ê¸»úÌÜ°Ê¹ß¤ÎÂè°ì¥È¡¼¥¯¥ó¤ÏHP¤Î
 	    ¸½ºßÃÍ , ÂèÆó¥È¡¼¥¯¥ó¤ÏMP¡¢Âè»°¥È¡¼¥¯¥ó¤ÏEXP¤Ç¤¢¤ë¡£
 	    °Ê²¼¤Ë¶ñÂÎÎã¤ò¤·¤á¤¹¡£
 	    
 	    M54|210|8944909
 	    
 	    ¤³¤ÎÎã¤Ç¤ÏHP¤Î¸½ºßÃÍ¤¬54¡¢MP¤Î¸½ºßÃÍ¤¬210¡¢·Ð¸³ÃÍ¤¬8944909
 	    ¤Ë¤Ê¤Ã¤Æ¤¤¤ë¤Î¤À¡£¤³¤Î3¼ïÎà¤ÏÆÃ¤ËÊÑ¹¹ÉÑÅÙ¤¬¹â¤¤¤È»×¤ï¤ì¤ë
 	    ¤¿¤áÆÈÎ©¤·¤¿¥³¥Þ¥ó¥É²½¤·¤¿¤¬¡¢¤â¤Á¤í¤ó¤³¤ÎM¥³¥Þ¥ó¥É¤Î¤«¤ï
 	    ¤ê¤ËP¥³¥Þ¥ó¥É¤ÇÁ´¥Ñ¥é¥á¡¼¥¿¤òÁ÷¿®¤·¤Æ¤â¤è¤¤¡£¤¿¤À¤·¡¢ºÇÅ¬
 	    ¤Ë¥Í¥Ã¥È¥ï¡¼¥¯¤ò»È¤¦¤¿¤á¤Ë¤Ï¡¢¤³¤Î¥³¥Þ¥ó¥É¤ò»È¤¦¤³¤È¤¬¿ä¾©
 	    ¤µ¤ì¤ë¡£
 	    
         D ¥­¥ã¥é¤ÎID
 	    ¥í¥°¥¤¥ó»þ¤Ë¤À¤±Á÷¤ë¥­¥ã¥é¤Îindex¡¤¤½¤ì¤È»þ´Ö¤òÁ÷¤ë¡£
 	    D1000|912766409¤È¤«¤ÇÁ÷¤é¤ì¤ë¡£
 	
 	E ¥¨¥ó¥«¥¦¥ó¥È³ÎÎ¨¡Ê²¼¸Â)|(¾å¸Â)
 	    ¥¨¥ó¥«¥¦¥ó¥ÈÎ¨¤òÁ÷¿®¤¹¤ë¡£³ÎÎ¨¤Ïn/100
 	    ¥¯¥é¥¤¥¢¥ó¥È¤Ï¡¤²¼¸Â¤ÎÃÍ¤«¤é¥¹¥¿¡¼¥È¤·¤Æ¡¤£±ÊâÊâ¤¯Ëè¤Ë+1¤·¤¿³ÎÎ¨¤Ç¥¨¥ó¥«¥¦¥ó¥È¤¹¤ë¤«¤òÈ½ÃÇ¤¹¤ë¡£
 	    ¤¿¤À¤·¡¤¾å¸Â¤ÎÃÍ¤ò±Û¤¨¤Ê¤¤¤è¤¦¤Ë¤¹¤ë¡£
 	    ¼¡²ó¤Ë¤³¤ì¤¬Á÷¤é¤ì¤Æ¤¯¤ë¤Þ¤Ç¤³¤Î³ÎÎ¨¤Ç¥¨¥ó¥«¥¦¥ó¥È¤ò·×»»¤¹¤ë»ö¡£
 	
 	J0 ¡ÁJ6 »È¤¨¤ë¼ö½Ñ¤ÎÆâÍÆ
 	    »È¤¨¤ë¼ö½Ñ¤ÎÆâÍÆ¤òÁ÷¿®¤¹¤ë¡£
 	    0 ¡Á6 ¤Ï¤½¤ì¤¾¤ì¥¢¥¤¥Æ¥à¤ÎÁõÈ÷²Õ½ê¤ËÂÐ±þ¤·¤Æ¤¤¤ë¡£°ìÉô¤ÎÁõÈ÷²Õ½ê¡ÊÆ¬¡¤Æ¹¡¤¼ê¡¤Áõ¾þÉÊ¡Ë
 	    ¤·¤«ÁõÈ÷½ÐÍè¤Ê¤¤Í½Äê¤À¤¬¡¤º£¸å³ÈÄ¥¤Î°Ù¤ËÁ´¤Æ¤ÎÁõÈ÷²Õ½ê¤Î¾ðÊó¤òÁ÷¤ë
 	    J0|kubun|mp|field|target|name|comment
 	    ¤È¤¤¤¦¥Õ¥©¡¼¥Þ¥Ã¥È¤Ë¤Ê¤Ã¤Æ¤¤¤ë¡£
 	    kubun ¤Ï¤½¤ÎÍó¤¬¤¢¤ë¤«Ìµ¤¤¤«¡££°¤À¤ÈÌµ¤¤¡£¤½¤ì°Ê¹ß¤Ë¥È¡¼¥¯¥ó¤¹¤é¤Ê¤¤¡££±¤À¤È¤¢¤ë¡£
 	    mp¤Ï¾ÃÈñµ¤ÎÏ¤òÉ½¤¹¡£
 	    field¤Ï¤É¤Î¾ì½ê¤Ç»È¤¨¤ë¤«¡£¥µ¡¼¥Ð¡¼¤Ç¤Ï°Ê²¼¤ÎÍÍ¤ËÄêµÁ¤µ¤ì¤Æ¤¤¤ë¡£
 	    
 		typedef enum
 		{
 			MAGIC_FIELD_ALL,		¤¹¤Ù¤Æ¤Î¾ì½ê¤Ç»È¤¨¤ë
 			MAGIC_FIELD_BATTLE,		ÀïÆ®Ãæ¤Î¤ß
 			MAGIC_FIELD_MAP,		ÄÌ¾ï¥Þ¥Ã¥×¾å¤Î¤ß
 			
 		}MAGIC_FIELDTYPE;
 	    
 	    target¤Ï¤É¤ì¤òÂÐ¾Ý¤Ë½ÐÍè¤ë¤«¡£¥µ¡¼¥Ð¡¼¤Ç¤Ï°Ê²¼¤Î¤è¤¦¤ËÄêµÁ¤µ¤ì¤Æ¤¤¤ë¡£
 	    
 		typedef enum
 		{
 			MAGIC_TARGET_MYSELF,		¼«Ê¬¤Î¤ß
 			MAGIC_TARGET_OTHER,		Â¾¤Î¿Í¡Ê¼«Ê¬´Þ¤à)
 			MAGIC_TARGET_ALLMYSIDE,		Ì£ÊýÁ´ÂÎ
 			MAGIC_TARGET_ALLOTHERSIDE,	Áê¼êÂ¦Á´ÂÎ
 			MAGIC_TARGET_ALL,		Á´¤Æ
 			MAGIC_TARGET_NONE,		Ã¯¤âÁªÂò½ÐÍè¤Ê¤¤¡£ËÉ¸æ¤ä¤¿¤á¤Î»þ
 			MAGIC_TARGET_OTHERWITHOUTMYSELF,Â¾¤Î¿Í¡Ê¼«Ê¬´Þ¤Þ¤Ê¤¤)
 			MAGIC_TARGET_WITHOUTMYSELFANDPET, ¼«Ê¬¤È¥Ú¥Ã¥È°Ê³°
 			MAGIC_TARGET_WHOLEOTHERSIDE,    ÊÒÊý¤Î¥µ¥¤¥ÉÁ´ÂÎ
 		}MAGIC_TARGETTYPE;
 	    
 	    <font size=+1>¤³¤Î¿ô»ú¤Ë100¤òÂ­¤¹¤È¡¤»à¤ó¤Ç¤¤¤ë¼Ô¤âÂÐ¾Ý¤È¤Ê¤ë¡£
 	    
 	    name¤Ï¼ö½ÑÌ¾¡£
 	    comment¤Ï¤³¤Î¼ö½Ñ¤ÎÀâÌÀ¡£
 	
 	N0 ¡Á N3 Ãç´Ö¤Î¥Ñ¥é¥á¡¼¥¿
 	    Ãç´Ö¤Ë¤Ê¤Ã¤Æ¤¤¤ë¿Í¤Î¾ðÊó¤òÁ÷¿®¤¹¤ë¡£
 	    N0|kubun|level|charaindex|maxhp|hp|mp|name
 	    
 	    kubun ¤Ï¤½¤ÎÍó¤¬¤¢¤ë¤«Ìµ¤¤¤«¡££°¤À¤ÈÌµ¤¤¡£¤½¤ì°Ê¹ß¤Ë¥È¡¼¥¯¥ó¤¹¤é¤Ê¤¤¡££±Á´¥Ñ¥é¥á¡¼¥¿¡£
 	    2°Ê¾å¤À¤È¡¤³Æ¥Ó¥Ã¥È¤ÎÎ©¤Ã¤Æ¤¤¤ë¤â¤Î¤Î¥Ñ¥é¥á¡¼¥¿¡Ê1bit ÌÜ level 2bit ÌÜcharaindex¤Ê¤É¡Ë
 	    ¤¬Á÷¿®¤µ¤ì¤ë¡£
 	    charaindex ¤ÏÃç´Ö¤Îcharaindex
 	    level¤Ï¤½¤Î¿Í¤Î¥ì¥Ù¥ë¡£
 	    maxhp¤Ï¤½¤Î¿Í¤ÎMAXHP
 	    hp¤Ï¤½¤Î¿Í¤Î¸½ºß¤ÎHP
 	    mp¤Ï¤½¤Î¿Í¤Îµ¤ÎÏ
 	    name ¤Ï¤½¤Î¿Í¤ÎÌ¾Á°¡£
 	
 	K0¡ÁK4 ¶²Îµ¥Ñ¥é¥á¡¼¥¿
 	    hp maxhp mp maxmp str tough exp 
 	    level attackpower deffencepower 
 	    fixdex fixcharm fixluck fixfireat fixwaterat fixearthat fixwindat
 	    Ì¾Á° status
 	    K¤Î¸å¤Ë0¡Á4¤ò»ØÄê¤·¤Æ¡¢¤É¤Î¶²Îµ¤«¤ò»ØÄê¤¹¤ë¤³¤È¡£
 	    ¤½¤Î¼¡¤Î¥È¡¼¥¯¥ó¤Ë0¤¬Íè¤¿¤é¤½¤Î¥Ú¥Ã¥ÈÍó¤ÏÌµ¤¤¤È¸À¤¦»ö¡£
 	    ¤¢¤ë¾ì¹ç¤Ï£±¤Ç¤¢¤ë¡£1¤À¤ÈÁ´¥Ñ¥é¥á¡¼¥¿¡£
 	    2°Ê¾å¤À¤È¡¤³Æ¥Ó¥Ã¥È¤ÎÎ©¤Ã¤Æ¤¤¤ë¤â¤Î¤Î¥Ñ¥é¥á¡¼¥¿¡Ê1bit ÌÜ hp  2bit ÌÜmaxhp¤Ê¤É¡Ë
 	    ¤¬Á÷¿®¤µ¤ì¤ë¡£
 	    ¥Ç¥ê¥ß¥¿¤Ï '|' ¤Ç¤¢¤ë¡£¤Þ¤¿Ì¾Á°¤È¼«¸Ê¾Î¹æ¤Ï¡¢
 	    <a href="#escaping">¥¨¥¹¥±¡¼¥×¤·¤¿¤â¤Î¤ò
 	    ¤¿¤ÆËÀ¤Ç¤Ä¤Ê¤²¤¿¤â¤Î¤¬ÄÌ¿®¤µ¤ì¤ë¡£
 	    ¤½¤ì¤¾¤ì¤ÎÃÍ¤Î·¿¤Ï°Ê²¼¡£
 	    
 	        No.(ÈÖ¹æ)  int  
 	        islive(À¸»à)  int  
 	        GRA(²èÁüÈÖ¹æ)  int  
 	        Hp(ÂÑµ×ÎÏ)  int  
 	        MaxHp(ºÇÂçÂÑµ×ÎÏ)  int  
 	        Mp  int  
 	        MapMp  int  
 	        Exp(·Ð¸³ÃÍ)  int  
 	        MaxExp(¼¡¤Þ¤Ç¤Î·Ð¸³ÃÍ)  int  
 	        Level(¥ì¥Ù¥ë)  int  
 	        Attack(¹¶·âÎÏ)  int  
 	        Defense(¼éÈ÷ÎÏ)  int  
 	        Quick(ÁÇÁá¤µ)  int  
 	        Ai(ÃéÀ¿ÅÙ)  int  
 	        fEarth(ÃÏ)  int  
 	        fWater(¿å)  int  
 	        fFire(²Ð)  int  
 	        fWid(É÷)  int  
 	        Slot(ºÇÂçÀÐÈÇ¿ô)  int  
 	        Ì¾Á°ÊÑ¹¹µö²Ä¥Õ¥é¥°  int  
 	        Ì¾Á°  Ê¸»úÎó  
 	        ¥æ¡¼¥¶¡¼¥Ú¥Ã¥ÈÌ¾  Ê¸»úÎó  
 	    
 	    Ì¾Á°ÊÑ¹¹µö²Ä¥Õ¥é¥°¤È¤Ï¡¤¤³¤Î¥Ú¥Ã¥È¤ÎÌ¾Á°¤òÊÑ¹¹¤·¤Æ¤è¤¤¤«¤É¤¦¤«¤Î¥Õ¥é¥°¤Ç¡¤
 	    1 ¤À¤ÈÊÑ¹¹£Ï£Ë¡¤£°¤À¤ÈÊÑ¹¹ÉÔ²Ä¤È¤Ê¤ë¡£
 	    
 	    ¶ñÂÎÎã( char ¤ÎÇÛÎó¤½¤Î¤Þ¤Þ½ñ¤¯ )
 	    
 	    P10|20|10|20|10|10|10|1|2|13|13|1|100|10|¤Ø¤ó¤Ñ|PC
 
 	W0¡ÁW4 ¥Ú¥Ã¥È¤Îµ»¥Ç¡¼¥¿
 	  W0|skillid|field|target|name|comment| x 7
 	  W0 ¡Á W4 ¤Ï¤½¤ì¤¾¤ì¤Î¥Ú¥Ã¥È¤ËÂÐ±þ¤·¤Æ¤¤¤ë¡£
 	  petskillid ¤Ï¡¤¥Ú¥Ã¥È¤Îµ»¤ÎÈÖ¹æ¡£pet_skillinfo.h¤ËÄêµÁ¤µ¤ì¤Æ¤¤¤ë¡£
 	  field ¤Ï¤½¤Îµ»¤¬¤É¤³¤Ç»ÈÍÑ¤Ç¤­¤ë¤«¡£¥µ¡¼¥Ð¡¼¤Ç¤Ï°Ê²¼¤Î¤è¤¦¤ËÄêµÁ¤µ¤ì¤Æ¤¤¤ë¡£
 	  
 		typedef enum
 		{
 			PETSKILL_FIELD_ALL,		¤¹¤Ù¤Æ¤Î¾ì½ê¤Ç»È¤¨¤ë
 			PETSKILL_FIELD_BATTLE,		ÀïÆ®Ãæ¤Î¤ß
 			PETSKILL_FIELD_MAP,		ÄÌ¾ï¥Þ¥Ã¥×¾å¤Î¤ß
 
 		}PETSKILL_FIELDTYPE;
 	  
 	  target ¤Ï¤½¤Îµ»¤ÎÂÐ¾Ý¤¬¤É¤¦¤¤¤¦¤â¤Î¤«¡£¥µ¡¼¥Ð¡¼¤Ç¤Ï¼¡¤Î¤è¤¦¤ËÄêµÁ¤µ¤ì¤Æ¤¤¤ë¡£
 	  
 		typedef enum
 		{
 			PETSKILL_TARGET_MYSELF,		¼«Ê¬¤Î¤ß
 			PETSKILL_TARGET_OTHER,		Â¾¤Î¿Í¡Ê¼«Ê¬´Þ¤à)
 			PETSKILL_TARGET_ALLMYSIDE,	Ì£ÊýÁ´ÂÎ
 			PETSKILL_TARGET_ALLOTHERSIDE,	Áê¼êÂ¦Á´ÂÎ
 			PETSKILL_TARGET_ALL,		Á´¤Æ
 			PETSKILL_TARGET_NONE,		Ã¯¤âÁªÂò½ÐÍè¤Ê¤¤¡£ËÉ¸æ¤ä¤¿¤á¤Î»þ
 			PETSKILL_TARGET_OTHERWITHOUTMYSELF,Â¾¤Î¿Í¡Ê¼«Ê¬´Þ¤Þ¤Ê¤¤) 
 			PETSKILL_TARGET_WITHOUTMYSELFANDPET, ¼«Ê¬¤È¥Ú¥Ã¥È°Ê³°
 		}PETSKILL_TARGETTYPE;
 	  
 	  name ¤Ïµ»¤ÎÌ¾Á°¡£
 	  comment¤Ï¤½¤Îµ»¤ËÂÐ¤¹¤ëÀâÌÀ¡£
 	  target|name|comment| ¤Ïµ»¤Î¿ô¤ÎÊ¬£±¹Ô¤ÇÁ÷¤é¤ì¤Æ¤¯¤ë¡£
 	  µ»¤Ï°ì±þ£·¤Ä¤Þ¤Ç¡£ÅÓÃæ¤Çµ»¤¬È´¤±¤Æ¤¤¤ë¡Ê"|"¤Î¤ß¤¬Â³¤¯¡Ë¾ì¹ç
 	  ¤Ï¥¯¥é¥¤¥¢¥ó¥È¤ÇµÍ¤á¤ÆÉ½¼¨¤¹¤ë»ö¡£
*/
void lssproto_S_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_S_SEND, buffer);
}

/*
   servertoclient D( int category , int dx , int dy , string data );
       (Display)
       ²èÌÌ¤Ë²¿¤«É½¼¨¤¹¤ë»ØÎá¡£
 
       
 	int category
 	²¿¤òÉ½¼¨¤¹¤ë¤«¡£
 	    
 	        ÃÍ  ÆâÍÆ  
 	        1  ¼«Ê¬°Ê³°¤ËÍ¿¤¨¤¿¥À¥á¡¼¥¸¡£data¤ÏÊ¸»ú
 		  Îó¤Ë¤Ê¤Ã¤¿ÃÍ¡£  
 		    2  ¼«Ê¬¤¬¤¦¤±¤¿¥À¥á¡¼¥¸¡£data¤ÏÊ¸»úÎó¤Ë
 		      ¤Ê¤Ã¤¿ÃÍ  
 		
 	int dx
 	¥Þ¥¤¥­¥ã¥é¤«¤é¤Î¥°¥ê¥Ã¥ÉÁêÂÐ°ÌÃÖX¡£°ìÈÌÅª¤Ë¤Ï¥¤¥Ù¥ó¥È¤¬È¯À¸
 	    ¤·¤¿°ÌÃÖ¡£¥¯¥é¥¤¥¢¥ó¥È¤Ï¤³¤ÎÃÍ¤«¤éÅ¬µ¹Å¬ÀÚ¤Ê°ÌÃÖ¤ò·×»»¤·¤Æ
 	    ÉÁ²è¤¹¤ë¡£
 	int dy
 	ÁêÂÐ°ÌÃÖY
 	string data 
 	É½¼¨¤¹¤ëÆâÍÆ¡£ÆâÍÆ¤Ïcategory¤Ë¤è¤Ã¤Æ·è¤Þ¤ë¡£
 	    ¤³¤ÎÊ¸»úÎó¤Ï<a href="#escaping">¥¨¥¹¥±¡¼¥×¤µ¤ì¤Ê¤±¤ì¤Ð
 	    ¤Ê¤é¤Ê¤¤¡£
*/
void lssproto_D_send(int fd,int category,int dx,int dy,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, category);
	checksum += util_mkint(buffer, dx);
	checksum += util_mkint(buffer, dy);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_D_SEND, buffer);
}

/*
   servertoclient FS( int flg);
       (FlgSet)PS¤Î¤Î±þÅú¡£¤Þ¤¿¤Ï¥í¥°¥¤¥ó»þ¤Ê¤É¤Ë¼«Ê¬¤Î¾õÂÖ¤È¤·¤ÆÁ÷¤é¤ì¤Æ¤¯¤ë¡£
       
         int flg
          0 bit  0: Ãç´ÖOff                  1: Ãç´ÖOn
              ¡Ê¸½ºßÌ¤»ÈÍÑ¡Ë1 bit  0: ÀïÆ®ÅÓÃæ»²²Ãoff          1: ÀïÆ®ÅÓÃæ»²²ÃOn    
              2 bit  0: DUEL off                 1: DUEL On
              3 bit  0: ÉáÄÌ¤Î¥Á¥ã¥Ã¥È¥â¡¼¥É     1: ¥Ñ¡¼¥Æ¥£¤Ë¤·¤«¥Á¥ã¥Ã¥È¤¬Èô¤Ð¤Ê¤¤¥â¡¼¥É
              4 bit  0: Ì¾»É¸ò´¹OK               1: Ì¾»É¸ò´¹µñÈÝ
*/
void lssproto_FS_send(int fd,int flg)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_FS_SEND, buffer);
}

/*
   servertoclient HL( int flg);
       (HeLp)HL¤Î¤Î±þÅú¡£¤Þ¤¿¤Ï¥Ñ¡¼¥Æ¥£¤ÎÃç´Ö¤¬ÀïÆ®¤Î¤ª½õ¤±¥â¡¼¥É¤òÊÑ¹¹¤·¤¿¾ì¹ç¤ËÁ÷¤é¤ì¤Æ¤¯¤ë¡£
       
         int flg
          0: ¤ª½õ¤±¥â¡¼¥ÉOff                  1: ¤ª½õ¤±¥â¡¼¥ÉOn
*/
void lssproto_HL_send(int fd,int flg)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_HL_SEND, buffer);
}

/*
   servertoclient PR( int request, int result);
       (PartyRequest)Ãç´ÖÍ×µá¤Î±þÅú¡£PR¤ò¥¯¥é¥¤¥¢¥ó¥È¤¬Á÷¤Ã¤Æ¤¤¤Ê¤¯¤Æ¤â¤³¤ì¤ò¼õ¤±¼è¤ë»þ¤¬¤¢¤ë¡£
        ¥Ñ¡¼¥Æ¥£¤¬ÆÍÁ³²ò»¶¡Ê¿Æ¤¬È´¤±¤¿¡Ë¤Ê¤É¤Î»þ¡£
       
       	int request
       	0: ½üÂâ 1:ÆþÂâ
         int result
         0: ¼ºÇÔ 1: À®¸ù
*/
void lssproto_PR_send(int fd,int request,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, request);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PR_SEND, buffer);
}

#ifdef _PETS_SELECTCON
void lssproto_PETS_send(int fd,int petarray,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, petarray);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PETST_SEND, buffer);
}
#endif

void lssproto_KS_send(int fd,int petarray,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, petarray);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_KS_SEND, buffer);
}

void lssproto_SPET_send(int fd, int standbypet, int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, standbypet);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SPET_SEND, buffer);
}

/*
   servertoclient PS( int result, int havepetindex, int havepetskill, int toindex);
       (PetSkill use result)¥Ú¥Ã¥È¤Îµ»¤ò»È¤Ã¤¿·ë²Ì¥¯¥é¥¤¥¢¥ó¥È¤ÎPS¤ËÂÐ±þ¤·¤ÆÁ÷¤é¤ì¤Æ¤¯¤ë¡£
       result °Ê³°¤Ï¥¯¥é¥¤¥¢¥ó¥È¤ÎPS¤ËÂÐ±þ¤·¤Æ¤¤¤ë¡£¥Õ¥£¡¼¥ë¥É¾å¤«¤é¤·¤«¸Æ¤Ð¤ì¤Ê¤¤¡£
       
 	int result
 	·ë²Ì¡£0: ¼ºÇÔ 1:À®¸ù
         int havepetindex
         ²¿ÈÖÌÜ¤Î¥Ú¥Ã¥È¤¬»ÈÍÑ¤·¤¿¤«¡£
         int havepetskill
         ²¿ÈÖÌÜ¤Îµ»¤ò»ÈÍÑ¤·¤¿¤«¡£
 	int toindex
 	 Ã¯¤ËËâË¡¤ò»ÈÍÑ¤·¤¿¤«¡£¤³¤ì¤Ï¥ª¥Ö¥¸¥§¥¯¥È¤ä¥­¥ã¥é¤Îindex¤Ç¤Ï¤Ê¤¤¡£°Ê²¼¤ÎÍÍ¤Ë¤Ê¤Ã¤Æ¤¤¤ë¡£
 	
 	  ¼«Ê¬    = 0
 	  ¥Ú¥Ã¥È  = 1 ¡Á5
 	  Ãç´Ö    = 6 ¡Á10 ¡ÊS N ¤Î0¡Á4¤ËÂÐ±þ¡£¼«Ê¬¼«¿È¤â´Þ¤Þ¤ì¤Æ¤¤¤ë¡Ë
 	
 	ÂÐ¾Ý¤¬Á´°÷¡¤¤È¤«Ê¬¤«¤é¤Ê¤¤¡¤¤È¤«¤Î¾ì¹ç¤Ï-1¤ÇÁ÷¿®¤¹¤ë¡£
*/
void lssproto_PS_send(int fd,int result,int havepetindex,int havepetskill,int toindex)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, result);
	checksum += util_mkint(buffer, havepetindex);
	checksum += util_mkint(buffer, havepetskill);
	checksum += util_mkint(buffer, toindex);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PS_SEND, buffer);
}

/*
   servertoclient SKUP( int point );
       (SKillUP)
       ¥¹¥­¥ë¥¢¥Ã¥×¤¬¤Ç¤­¤ë»ö¤ò¥µ¡¼¥Ð¤¬ÄÌÃÎ¤¹¤ë¡£¤¤¤¯¤Ä¾å¤²¤é¤ì¤ë¤«¤ò»ØÄê¤¹¤ë¡£
*/
void lssproto_SKUP_send(int fd,int point)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, point);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SKUP_SEND, buffer);
}

/*
   servertoclient WN( int windowtype, int buttontype, int seqno, int objindex, string data );
       (Window)
       ¥¦¥£¥ó¥É¥¦¤òÉ½¼¨¤»¤è¤È¥µ¡¼¥Ð¡¼¤¬¥¯¥é¥¤¥¢¥ó¥È¤ËÄÌÃÎ¤¹¤ë¡£
       
         int windowtype
         ¥¦¥£¥ó¥É¥¦¥¿¥¤¥×¡£¤É¤Î¤è¤¦¤Ê·Á¼°¤Î¥¦¥£¥ó¥É¥¦¤òÉ½¼¨¤¹¤ë¤«¡£¤³¤ì¤Èbuttontype ¤ÎÁÈ¤ß¹ç¤ï¤»¤Ç
             ¥¦¥£¥ó¥É¥¦¤¬·èÄê¤¹¤ë¡£
             ¥µ¡¼¥Ð¡¼¤Ç¤Ï°Ê²¼¤ÎÍÍ¤ËÀßÄê¤µ¤ì¤Æ¤¤¤ë¡£
           
 		typedef enum
 		{
 			WINDOW_MESSAGETYPE_MESSAGE,			¥á¥Ã¥»¡¼¥¸¤Î¤ß
 			WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,		¥á¥Ã¥»¡¼¥¸¤È°ì¹ÔÆþÎÏ
 			WINDOW_MESSAGETYPE_SELECT,			ÁªÂò¥¦¥£¥ó¥É¥¦
 			WINDOW_MESSAGETYPE_PETSELECT,			¥Ú¥Ã¥ÈÁªÂò¥¦¥£¥ó¥É¥¦
 			WINDOW_MESSAGETYPE_PARTYSELECT,			Ãç´ÖÁªÂò¥¦¥£¥ó¥É¥¦
 			WINDOW_MESSAGETYPE_PETANDPARTYSELECT,		¥Ú¥Ã¥È¡¤Ãç´ÖÁªÂò¥¦¥£¥ó¥É¥¦
 			WINDOW_MESSAGETYPE_ITEMSHOPMENU,		¤ªÅ¹¤Î¥á¥Ë¥å¡¼¥¦¥¤¥ó¥É¥¦
 			WINDOW_MWSSAGETYPE_ITEMSHOPYMAIN,		¤ªÅ¹¤Î¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦
 			WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN,		Çã¤¤¼è¤êÀìÌç¥¢¥¤¥Æ¥à²°¤Î¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦
 			WINDOW_MESSAGETYPE_PETSKILLSHOP,		¥Ú¥Ã¥È¤Îµ»²°¤µ¤ó¥¦¥¤¥ó¥É¥¦
 			WINDOW_MESSAGETYPE_WIDEMESSAGE,			¥á¥Ã¥»¡¼¥¸¤Î¤ß¡ÊÂç¤­¤¤Êý¡Ë
 			WINDOW_MESSAGETYPE_WIDEMESSAGEANDLINEINPUT,	¥á¥Ã¥»¡¼¥¸¤È°ì¹ÔÆþÎÏ¡ÊÂç¤­¤¤Êý¡Ë
 
 		}WINDOW_MESSAGETYPE;
 
         int buttontype
         ¥Ü¥¿¥ó¤Î·Á¼°¤ò»ØÄê¤¹¤ë¡£¥µ¡¼¥Ð¡¼¤Ç¤Ï°Ê²¼¤ÎÍÑ¤ËÄêµÁ¤µ¤ì¤Æ¤¤¤ë¡£
             
 		#define		WINDOW_BUTTONTYPE_NONE		(0)
 		#define		WINDOW_BUTTONTYPE_OK		(1 << 0)
 		#define		WINDOW_BUTTONTYPE_CANCEL	(1 << 1)
 		#define		WINDOW_BUTTONTYPE_YES		(1 << 2)
 		#define		WINDOW_BUTTONTYPE_NO		(1 << 3)
 		#define		WINDOW_BUTTONTYPE_PREV		(1 << 4)
 		#define		WINDOW_BUTTONTYPE_NEXT		(1 << 5)
             
             ¤³¤ì¤é¤ÎÁÈ¤ß¹ç¤ï¤»¤ÇÁ÷¿®¤¹¤ë¡£Îã¤¨¤Ð¡¤YES¥Ü¥¿¥ó¤ÈNO¥Ü¥¿¥ó¤¬Íß¤·¤¤»þ¤Ï
             WINDOW_BUTTONTYPE_YES | WINDOW_BUTTONTYPE_NO   (=12)
             ¤ÇÁ÷¤ë¡£
             
         int seqno
         ¤³¤Î¥¦¥£¥ó¥É¥¦¤ÎÈÖ¹æ¤ò¼¨¤¹¡£¥µ¡¼¥Ð¡¼¤¬´ÉÍý¤¹¤ë¡£
             ¥¯¥é¥¤¥¢¥ó¥È¤ÏWN¤Ë¤Æ¤³¤Î¥¦¥£¥ó¥É¥¦¤ÎÁàºî·ë²Ì¤òÊÖ¤¹¤Î¤Ë¡¤¤³¤ÎÈÖ¹æ¤òÅº¤¨¤ÆÊÖÅú¤¹¤ë¡£
             ¤³¤ì¤Ë¤è¤Ã¤Æ¥µ¡¼¥Ð¡¼¤Ï¤É¤ÎNPC¤Î¤É¤Î¾ìÌÌ¤Ç¤Î¥¦¥£¥ó¥É¥¦¤«¤òÈ½ÃÇ½ÐÍè¤ë¤è¤¦¤Ë¤¹¤ë¡£
         int objindex
         ¤³¤Î¥¦¥£¥ó¥É¥¦¤ò½Ð¤»¤È¸À¤Ã¤¿NPC¤Ê¤É¤Îindex¤¬³ÊÇ¼¤µ¤ì¤Æ¤¤¤ë¡£
             ¥·¥¹¥Æ¥à¤¬½Ð¤»¤È¸À¤Ã¤¿¾ì¹ç¤Ï-1¤Ê¤É¤¬Æþ¤Ã¤Æ¤¤¤ë¡£
             ¥¯¥é¥¤¥¢¥ó¥È¤Ï¡¤¥¦¥£¥ó¥É¥¦ÆþÎÏ¸å¤ÎWN¥×¥í¥È¥³¥ë¤Ç¤³¤Î¿ôÃÍ¤ò¤½¤Î¤Þ¤ÞÊÖ¤»¤ÐÎÉ¤¤¡£
         string data
         ¥á¥Ã¥»¡¼¥¸ÆâÍÆ¤ò¼¨¤¹¡£ÆâÍÆ¤Ï¥¨¥¹¥±¡¼¥×¤¹¤ë¡£"\n"¤Ç¶èÀÚ¤ë¤È²þ¹Ô¤È¤¤¤¦°ÕÌ£¤È¤¹¤ë¡£
             ¤Þ¤¿¡¤¥¯¥é¥¤¥¢¥ó¥È¤Î¥¦¥£¥ó¥É¥¦¤ÇÉ½¼¨½ÐÍè¤ë²£Éý¤ò±Û¤¨¤¿ºÝ¤Ï¼«Æ°Åª¤Ë²þ¹Ô¤µ¤ì¤ë¡£
             ÁªÂò»è¤Î¤¢¤ë¥¦¥£¥ó¥É¥¦¤Ç¤Ï¡¤"\n"¤Ç¶èÀÚ¤é¤ì¤¿ºÇ½é¤Î¥È¡¼¥¯¥ó¤¬¥á¥Ã¥»¡¼¥¸¤Î¹Ô¿ô
             ¤È¤Ê¤ê¡¤¼¡¤Ë¥á¥Ã¥»¡¼¥¸¤Î¥È¡¼¥¯¥ó¤¬ºÇ½é¤Î¥È¡¼¥¯¥ó¤Ç»ØÄê¤µ¤ì¤¿¸Ä¿ôÂ³¤­¡¤
             ¤½¤Î¤¢¤È¤Î¥È¡¼¥¯¥óËè¤¬£±¤Ä¤º¤Ä¤ÎÁªÂò»è¤È¤Ê¤ë¡£¤Þ¤¿Á°¤«¤é½ç¤Ë 1 ¤«¤é
             ÈÖ¹æ¤ò³ä¤êÅö¤Æ¡¤ÁªÂò¤·¤¿¾ì¹ç¤ÎWN¤Ç¤ÎÊÖÅú¤Î»þ¤ËÊÖ¤¹¡£
         string data(¤ªÅ¹ÍÑ¡Ë
         ¥á¥Ã¥»¡¼¥¸ÆâÍÆ¤ò¼¨¤¹¡£ÆâÍÆ¤Ï¥¨¥¹¥±¡¼¥×¤¹¤ë¡£"\n"¤Ç¶èÀÚ¤ë¤È²þ¹Ô¤È¤¤¤¦°ÕÌ£¤È¤¹¤ë¡£
             ¤Þ¤¿¡¤¥¯¥é¥¤¥¢¥ó¥È¤Î¥¦¥£¥ó¥É¥¦¤ÇÉ½¼¨½ÐÍè¤ë²£Éý¤ò±Û¤¨¤¿ºÝ¤Ï¼«Æ°Åª¤Ë²þ¹Ô¤µ¤ì¤ë¡£
             ¥Ç¡¼¥¿Æâ¤Ï¹àÌÜËè¤Ë"¡Ã"¤Ç¶èÀÚ¤é¤ì¤Æ¤¤¤Þ¤¹¡£
    ¢ãÇã¤¦¢ä
  	Çã¤¤Çä¤ë¥Õ¥é¥°¡ÊÇã¤¦£°¡§Çä¤ë£±¡Ë¡ÃÁ°¤Î¥Ç¡¼¥¿»È¤¦¤«¥Õ¥é¥°¡Ê»È¤¦£°¡§»È¤ï¤Ê¤¤¡§1¡Ë¡Ã
 		Å¹¤ÎÌ¾Á°¡Ã¥á¥Ã¥»¡¼¥¸¡ÃÅ¹¥á¥Ã¥»¡¼¥¸¡Ã¸Ä¿ôÁªÂò¥á¥Ã¥»¡¼¥¸¡Ã¥ì¥Ù¥ëÂ­¤ê¤Ê¤¤¥á¥Ã¥»¡¼¥¸¡Ã³ÎÇ§¥á¥Ã¥»¡¼¥¸¡Ã
 		¥¢¥¤¥Æ¥à¤¬¤¤¤Ã¤Ñ¤¤¥á¥Ã¥»¡¼¥¸¡Ã¥¢¥¤¥Æ¥àÌ¾¡ÃÇã¤¨¤ëÇã¤¨¤Ê¤¤¥Õ¥é¥°¡ÊÇã¤¨¤ë¡§£°Çã¤¨¤Ê¤¤¡§£±¡Ë¡Ã¥¢¥¤¥Æ¥à¥ì¥Ù¥ë¡Ã
 		ÃÍÃÊ¡Ã²èÁüÈÖ¹æ¡ÃÀâÌÀ¡Ã¥¢¥¤¥Æ¥àÌ¾¡ÃÇã¤¨¤ëÇã¤¨¤Ê¤¤¥Õ¥é¥°¡Ã¥¢¥¤¥Æ¥à¥ì¥Ù¥ë¡ÃÃÍÃÊ¡Ã²èÁüÈÖ¹æ¡ÃÀâÌÀ
       		
       		0|1|¥µ¥à¥®¥ë¤ÎÉð´ï²°|¤¤¤é¤Ã¤·¤ã¤¤¡¢²¿¤Î¤è¤¦¤À¤¤|¤¤¤é¤Ã¤·¤ã¤¤¤É¤ì¤Ë¤¹¤ë|²¿¸ÄÉ¬Í×¤À¤¤|¤½¤ì¤òÁõÈ÷¤¹¤ë¤Ë¤Ï¥ì¥Ù¥ë¤¬
       		Â­¤ê¤Ê¤¤¤±¤É¤¤¤¤¤Î¤«¤¤|ËÜÅö¤Ë¤¤¤¤¤ó¤À¤Í|¤ª¤¤¤ª¤¤¥¢¥¤¥Æ¥à¤¬¤¤¤Ã¤Ñ¤¤¤¸¤ã¤Í¤¨¤«¡ª¡Ã¥Ö¡¼¥á¥é¥ó|
       		£°|£±£°£°|£±£¶£°£°£°|²£°ìÎó¤Ë¹¶·â\n¸ú²ÌÈ´·²|ÌôÁð|£°|£µ£°|£±£¶£°£°£²|£È£Ð£²£°²óÉü\nËÁ¸±¤Ë¤Ï¤Ä¤­Êª\n°Â¤¤¡Ã
 
         ¢ãÇä¤ë¢ä
 		Çã¤¤Çä¤ë¥Õ¥é¥°¡ÊÇã¤¦£°¡§Çä¤ë£±¡Ë¡ÃÁ°¤Î¥Ç¡¼¥¿»È¤¦¤«¥Õ¥é¥°¡Ê»È¤¦£°¡§»È¤ï¤Ê¤¤¡§1¡Ë¡Ã
 		Å¹¤ÎÌ¾Á°¡Ã¥á¥Ã¥»¡¼¥¸¡ÃÅ¹¥á¥Ã¥»¡¼¥¸¡Ã¤ª¶â¤¬¤¤¤Ã¤Ñ¤¤¤Ë¤Ê¤Ã¤Á¤ã¤¦¥á¥Ã¥»¡¼¥¸|³ÎÇ§¥á¥Ã¥»¡¼¥¸¡Ã
 		¥¢¥¤¥Æ¥àÌ¾¡ÃÇä¤ì¤ëÇä¤ì¤Ê¤¤¥Õ¥é¥°¡ÃÃÍÃÊ¡Ã²èÁüÈÖ¹æ¡ÃÀâÌÀ¡Ã¥¢¥¤¥Æ¥àÌ¾¡ÃÇä¤ì¤ëÇä¤ì¤Ê¤¤¥Õ¥é¥°¡Ã
 		ÃÍÃÊ¡Ã²èÁüÈÖ¹æ¡ÃÀâÌÀ¡Ã
       
       		£±|1|¥µ¥à¥®¥ë¤ÎÆ»¶ñ²°|¤¤¤é¤Ã¤·¤ã¤¤¡¢¤É¤ë¤¹¤ë¡©¡Ã¤¤¤é¤Ã¤·¤ã¤¤¤É¤ì¤òÇä¤Ã¤Æ¤¯¤ì¤ë¡£|ËÜÅö¤Ë¤¤¤¤¤ó¤À¤Í|
       		¤½¤ì¤À¤È¡¢¤ª¶â¤¬¥ª¡¼¥Ð¡¼¤·¤Á¤ã¤¦¤Í¡Ã¥Ö¡¼¥á¥é¥ó|£°|£²£°|£±£¶£°£°£°|
       		²£°ìÎó¤Ë¹¶·â\n¸ú²ÌÈ´·²|ÌôÁð|£°|£±£°|£±£¶£°£°£²|£È£Ð£²£°²óÉü\nËÁ¸±¤Ë¤Ï¤Ä¤­Êª\n°Â¤¤¡Ã
         ¢ã¥¢¥¤¥Æ¥àÍÂ¤±¤ë¢ä
 		¥Õ¥é¥°¡ÊÍÂ¤ë=£°¡Ë¡Ã¤¢¤È²¿¸ÄÍÂ¤±¤é¤ì¤ë¤«¡Ã
 		Å¹¤ÎÌ¾Á°¡Ã
 		ÍÂ¤±¤ë¥á¥Ã¥»¡¼¥¸¡Ã¤³¤ì°Ê¾åÍÂ¤±¤é¤ì¤Ê¤¤¥á¥Ã¥»¡¼¥¸¡Ã³ÎÇ§¥á¥Ã¥»¡¼¥¸¡Ã
 		Ì¾Á°¡ÃÍÂ¤±¤é¤ì¤ë¤«¥Õ¥é¥°¡Ê²ÄÇ½¡á£°¡¤ÉÔ²Ä¡á£±¡Ë¡ÃÃÍÃÊ¡Ã²èÁüÈÖ¹æ¡Ã°ì¹Ô¥¤¥ó¥Õ¥©¡Ã¥¢¥¤¥Æ¥àÍóÈÖ¹æ¡Ê£±¤«¤é¡Ë
 		Ì¾Á°¡ÃÍÂ¤±¤é¤ì¤ë¤«¥Õ¥é¥°¡Ê²ÄÇ½¡á£°¡¤ÉÔ²Ä¡á£±¡Ë¡ÃÃÍÃÊ¡Ã²èÁüÈÖ¹æ¡Ã°ì¹Ô¥¤¥ó¥Õ¥©¡Ã¥¢¥¤¥Æ¥àÍóÈÖ¹æ¡Ê£±¤«¤é¡Ë
         ¢ã¥¢¥¤¥Æ¥à°ú¤­¼è¤ë¢ä
 		¥Õ¥é¥°¡Ê°ú¤­¼è¤ë=£±¡Ë¡Ã
 		Å¹¤ÎÌ¾Á°¡Ã°ú¤­½Ð¤¹¥á¥Ã¥»¡¼¥¸¡Ã¥¢¥¤¥Æ¥à¤¬°ìÇÕ¤Î»þ¤Î¥á¥Ã¥»¡¼¥¸|³ÎÇ§¥á¥Ã¥»¡¼¥¸¡Ã
 		Ì¾Á°¡Ã°ú¤­½Ð¤»¤ë¤«¤É¤¦¤«¥Õ¥é¥°¡Ê²ÄÇ½¡á£°¡¤ÉÔ²Ä¡á£±¡Ë¡Ã¥¢¥¤¥Æ¥à¥ì¥Ù¥ë¡ÃÃÍÃÊ¡Ã²èÁüÈÖ¹æ¡Ã°ì¹Ô¥¤¥ó¥Õ¥©¡Ã
 		Ì¾Á°¡Ã°ú¤­½Ð¤»¤ë¤«¤É¤¦¤«¥Õ¥é¥°¡Ê²ÄÇ½¡á£°¡¤ÉÔ²Ä¡á£±¡Ë¡Ã¥¢¥¤¥Æ¥à¥ì¥Ù¥ë¡ÃÃÍÃÊ¡Ã²èÁüÈÖ¹æ¡Ã°ì¹Ô¥¤¥ó¥Õ¥©¡Ã
 
         ¢ã¤½¤ÎÂ¾¢ä
 		Å¹¤ÎÌ¾Á°¡Ã¥á¥Ã¥»¡¼¥¸
       
       		¥µ¥à¥®¥ë¤ÎÆ»¶ñ²°|¤¤¤é¤Ã¤·¤ã¤¤¡¢¤É¤ë¤¹¤ë¡©
 	
         string data(¥Ú¥Ã¥È¤Îµ»²°¤µ¤óÍÑ¡Ë
         ¥á¥Ã¥»¡¼¥¸ÆâÍÆ¤ò¼¨¤¹¡£ÆâÍÆ¤Ï¥¨¥¹¥±¡¼¥×¤¹¤ë¡£"\n"¤Ç¶èÀÚ¤ë¤È²þ¹Ô¤È¤¤¤¦°ÕÌ£¤È¤¹¤ë¡£
             ¤Þ¤¿¡¤¥¯¥é¥¤¥¢¥ó¥È¤Î¥¦¥£¥ó¥É¥¦¤ÇÉ½¼¨½ÐÍè¤ë²£Éý¤ò±Û¤¨¤¿ºÝ¤Ï¼«Æ°Åª¤Ë²þ¹Ô¤µ¤ì¤ë¡£
             ¥Ç¡¼¥¿Æâ¤Ï¹àÌÜËè¤Ë"¡Ã"¤Ç¶èÀÚ¤é¤ì¤Æ¤¤¤Þ¤¹¡£
 	   ¢ã¥Ú¥Ã¥È¤Îµ»¼èÆÀ¢ä
 		Á°¤Î¾ðÊó»È¤¦¤«¤É¤¦¤«¡©¡Ê»È¤¦¡§0¡¡»È¤ï¤Ê¤¤¡§1¡Ë¡ÃÅ¹¤ÎÌ¾Á°¡Ã¥á¥¤¥ó¥á¥Ã¥»¡¼¥¸¡Ã
 		µ»Ì¾¡ÃÃÍÃÊ¡Ãµ»ÀâÌÀ¡Ãµ»Ì¾¡ÃÃÍÃÊ¡Ãµ»ÀâÌÀ
 	
 		1¡Ã¥µ¥à¥®¥ë¤ÎÉð´ï²°¡Ã¤¤¤é¤Ã¤·¤ã¤¤¤Þ¤Ã¤»¡£¤¹¤Ð¤é¤·¤¤µ»¤Ð¤Ã¤«¤Ç¤¹¤è¡Ã
 		ÇØ¿å¤Î¿Ø¤½¤Î­¡¡Ã500¡Ã¹¶·âÎÏ£³£°¡ó£Õ£Ð¡¡ËÉ¸æÎÏ£³£°¡ó£Ä£Ï£×£Î¡Ã
 		ÃÏµå°ì½µ¡Ã1000¡Ã¿·¤·¤¤È¯¸«¤¬¤¢¤ë¤«¤â¡ÊÅ¬Åö¡Ë¡Ã	
*/
void lssproto_WN_send(int fd,int windowtype,int buttontype,int seqno,int objindex,char* data)
{
	char buffer[65500];
	int checksum=0;

#ifdef _NO_WARP
	// shan hanjj add Begin
    CONNECT_set_seqno(fd,seqno);
    CONNECT_set_selectbutton(fd,buttontype);    
	// shan End
#endif

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, windowtype);
	checksum += util_mkint(buffer, buttontype);
	checksum += util_mkint(buffer, seqno);
	checksum += util_mkint(buffer, objindex);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_WN_SEND, buffer);
	
}

/*
   servertoclient EF( int effect, int level, string option );
       (EFfect)
       Àã¤ä±«¤ò¹ß¤é¤·¤¿¤ê¡£¥¯¥é¥¤¥¢¥ó¥È¤ËÁ´ÂÎÅª¤Ê¸ú²ÌÉ½¸½¤»¤è¤ÈÁ÷¤ë¡£
       
         int effect
         ¸ú²ÌÈÖ¹æ¡£¤½¤ì¤¾¤ì¤òÂ­¤·»»¤¹¤ë¤È¡¤Î¾Êý¤È¤â¤Î¸ú²Ì¤¬ÆÀ¤é¤ì¤Þ¤¹¡££³¤Ë¤¹¤ë¤È±«¤ÈÀã¤¬¹ß¤Ã¤¿¤ê¤È¤«¤·¤Þ¤¹¡£
           
             1:±«
             2:Àã
             4:»æ¿áÀã¡ÊÍ½Äê¡Ë
           
         int level
         ¸ú²Ì¤Î¶¯¤µ¡££°¤Ï¾Ã¤¹¡££±¡Á£µ¤ÏÉ½¸½¤Î¶¯¤µ¤òÉ½¤¹¡£
         string option
         º£¸å³ÈÄ¥ÍÑ
*/
/* Arminius
  ·s¼Wµ¹¥á»ë¤lªº°Êµe
  int effect = 10   ¥á»ë¤l
      level dont care
      option = "pet1x|pet1y|dice1|pet2x|pet2y|dice2" (no makeEscapeString)
*/
void lssproto_EF_send(int fd,int effect,int level,char* option)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, effect);
	checksum += util_mkint(buffer, level);
	checksum += util_mkstring(buffer, option);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EF_SEND, buffer);
}

/*
   servertoclient SE( int x, int y, int senumber, int sw );
       (SoundEffect)
 	¥¯¥é¥¤¥¢¥ó¥È¤ËSE¤òÌÄ¤é¤¹¤è¤¦¤Ë»Ø¼¨¤¹¤ë¡£
       
         int x,y
         SE¤òÈ¯¿®¸µ¤ÎºÂÉ¸¡£
         int senumber
         £Ó£Å¤ÎÈÖ¹æ
         int sw
         £Ó£Å¤òÌÄ¤é¤¹¤«»ß¤á¤ë¤«¡£
           
             0:»ß¤á¤ë
             1:ÌÄ¤é¤¹
*/
void lssproto_SE_send(int fd,int x,int y,int senumber,int sw)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, senumber);
	checksum += util_mkint(buffer, sw);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SE_SEND, buffer);
}

/*
   servertoclient ClientLogin(string result);
       ClientLogin¤ÎÊÖÅú¡£
       
 	string result
 	"ok" ¤È¤¤¤¦Ê¸»úÎó¡£¤³¤ÎÊ¸»úÎó¤Ï¥¨¥¹¥±¡¼¥×¤µ¤ì¤Ê¤¤¡£
*/
void lssproto_ClientLogin_send(int fd,char* result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CLIENTLOGIN_SEND, buffer);
}

/*
   servertoclient CreateNewChar(string result,string data);
       CreateNewChar¤ÎÊÖÅú¡£
       
 	string result
 	"successful" ¤« "failed" ¤Î¤¤¤º¤ì¤«¡£¤³¤ÎÊ¸»úÎó¤Ï¥¨¥¹¥±¡¼
 	    ¥×¤·¤Ê¤¤¡£
 	string data
 	"failed" ¤Î»þ¤ÏÍýÍ³¤ò¼¨¤¹¿Í´Ö¤Î¸«¤ÆÊ¬¤ëÊ¸»ú
 	    Îó¤Ç¤¢¤ë¡£¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤«¤é¤ÎÊÖÅú¤½¤Î¤Þ¤Þ¤Ç¤¢¤ë¡£
 	    °Ê²¼¤ÎÊ¸»úÎó

 	    "failed bad parameter"

 	    ¤Î¾ì¹ç¤Ï¡¢¥­¥ã¥éºîÀ®¤Î¤È¤­¤Ëµ¬Äê¤Î¥Ñ¥é¥á¡¼¥¿¤ÎÈÏ°Ï¤ò±Û¤¨¤Æ
 	    ¤¤¤ë¤È¤¤¤¦¤È¤ò°ÕÌ£¤¹¤ë¡£¤³¤ì¤Ï¥²¡¼¥à¥µ¡¼¥Ð¡¼¤¬½ÐÎÏ¤¹¤ë¥á¥Ã
 	    ¥»¡¼¥¸¤Ç¤¢¤ë¡£¤³¤ÎÊ¸»úÎó¤Ï¥¨¥¹¥±¡¼¥×¤·¤Ê¤¤¡£
*/
void lssproto_CreateNewChar_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CREATENEWCHAR_SEND, buffer);
}

/*
   servertoclient CharDelete(string result,string data);
       CharDelete ¤ÎÊÖÅú¡£
       
 	string result
 	"successful" ¤« "failed" ¤Î¤¤¤º¤ì¤«¡£¥¨¥¹¥±¡¼¥×¤·¤Ê¤¤¡£
 	string data
 	"failed" ¤Î»þ¤ÏÍýÍ³¤ò¼¨¤¹¿Í´Ö¤Î¸«¤ÆÊ¬¤ëÊ¸»ú
 	    Îó¤Ç¤¢¤ë¡£¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¤«¤é¤ÎÊÖÅú¤½¤Î¤Þ¤Þ¤Ç¤¢¤ë¡£
 	    ¥Ç¥ê¥ß¥¿¤ò¤Õ¤¯¤Þ¤Ê¤¤¤Î¤Ç¡¢¥¨¥¹¥±¡¼¥×¤·¤Ê¤¤¡£
*/
void lssproto_CharDelete_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARDELETE_SEND, buffer);
}

/*
   servertoclient CharLogin(string result,string data);
       CharaLogin¤ÎÊÖÅú¡£
       
 	string result
 	"successful" ¤« "failed" ¤Î¤¤¤º¤ì¤«¡£¥¨¥¹¥±¡¼¥×¤·¤Ê¤¤¡£
 	string data
 	"failed" ¤Î»þ¤Ï ¤½¤ÎÍýÍ³¤ÎÊ¸»úÎó¡£¥¨¥¹¥±¡¼¥×¤·¤Ê¤¤¡£
*/
void lssproto_CharLogin_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLOGIN_SEND, buffer);
}

/*
   servertoclient CharList(string result,string data);
       CharList¤ÎÊÖÅú¡£
       
 	string result
 	"successful" ¤« "failed" ¤Î¤¤¤º¤ì¤«¡£¥¨¥¹¥±¡¼¥×¤·¤Ê¤¤
 	string data
 	result¤¬"successful"¤Î»þ¤Ï¡¢¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¡¼¤ËÊÝÂ¸¤µ¤ì¤Æ
 	    ¤¤¤ë¤¹¤Ù¤Æ¤Î¥­¥ã¥é¤ÎÌ¾Á°¡¢¥ª¥×¥·¥ç¥ó¤ò¤ò¥¹¥Ú¡¼¥¹¤Ç¶èÀÚ¤Ã¤¿
 	    °ì¸Ä¤ÎÊ¸»úÎó¡£¤³¤ÎÊ¸»úÎó¤òºî¤Ã¤Æ¤¤¤ë¤Î¤Ï¡¢¥¢¥«¥¦¥ó¥È¥µ¡¼¥Ð¡£
 	    result ¤¬ "failed" ¤Î»þ¤ÏÍýÍ³¤ò¼¨¤¹¿Í´Ö¤Î¸«¤ÆÊ¬¤ëÊ¸»úÎó¤Ç
 	    ¤¢¤ë¡£À®¸ù¤Î»þ¤Î¥ª¥×¥·¥ç¥ó¤ÎÃæ¿È¤Ï°Ê²¼¤Î¤È¤ª¤ê¤Ç¤¢¤ë¡£
 	    dataplace|faceimage|level|maxhp|atk|def|quick|charm|earth|water|fire|wind|logincount|name|place
 	    
 	      dataplace
 	      ¥»¡¼¥Ö¥Ç¡¼¥¿¤Î²¿ÈÖÌÜ¤«¡£¤³¤ì¤Ç¥­¥ã¥é¥¯¥¿¡¼¥ê¥¹¥È¤Î²¿ÈÖÌÜ¤«¤ò·èÄê¤¹¤ë¡£
 	      faceimage
 	      ´é¤Î²èÁüÈÖ¹æ
 	      level
 	      ¥­¥ã¥é¤Î¥ì¥Ù¥ë
 	      maxhp,atk,def,quick,charm
 	      ³Æ¥Ñ¥é¥á¡¼¥¿¡£
 	      earth.water,fire,wind
 	      ³ÆÂ°À­ÃÍ
 	      logincount
 	      ¥í¥°¥¤¥ó¥«¥¦¥ó¥È
 	      name
 	      ¥­¥ã¥é¤ÎÌ¾Á°
 	      place
 	      ¥­¥ã¥é¤Î¤¤¤ë¾ì½ê
 	    
 	    "|" ¤Ç¶èÀÚ¤é¤ì¤Æ¤¤¤ë¡£ ¤½¤ì¤¾¤ì¤Î¹àÌÜ¤Ï¡¢<a
 	    href="#escaping">¥¨¥¹¥±¡¼¥×¤µ¤ì¤Æ¤¤¤ë¡£¤½¤Î¤¢¤È¤¿¤ÆËÀ
 	    ¤Ç¤Ä¤Ê¤²¤ë¡£
*/
void lssproto_CharList_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLIST_SEND, buffer);
}

/*
   servertoclient CharLogout(string result , string data);
       Logout¤ËÂÐ¤¹¤ëÊÖÅú¡£
       
 	string result
 	"successful" ¤« "failed" ¤Î¤¤¤º¤ì¤«¡£¥¨¥¹¥±¡¼¥×¤·¤Ê¤¤¡£
 	string data
 	"failed" ¤Î»þ¤Ë¤Î¤ß°ÕÌ£¤¬¤¢¤ê¡¢¼ºÇÔ¤ÎÍýÍ³(¾õÂÖ)¤ò¼¨¤¹¿Í´Ö
 	    ¤Î¸«¤ÆÊ¬¤ëÊ¸»úÎó¤Ç¤¢¤ë¡£¥¨¥¹¥±¡¼¥×¤·¤Ê¤¤¡£
*/
void lssproto_CharLogout_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLOGOUT_SEND, buffer);
}

/*
   servertoclient ProcGet( string data);
 	ProcGet¤ÎÊÖÅú¡£
       
 	string data
 	¥¨¥¹¥±¡¼¥×¤¹¤ë¡£¥²¡¼¥à¥µ¡¼¥Ð¡¼¤ÎÆâÉô¾ðÊó¤òÁ÷¿®¤¹¤ë¡£ÆâÍÆ¤Ïlog/proc¥Õ¥¡¥¤¥ë¤Ë½ñ¤«¤ì¤ëÆâÍÆ¤ÈÆ±¤¸¡£
*/
void lssproto_ProcGet_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");
	
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PROCGET_SEND, buffer);
}

/*
   servertoclient PlayerNumGet( int logincount, int player);
 	PlayerNumGet¤ÎÊÖÅú¡£
       
 	int logincount,player
*/
void lssproto_PlayerNumGet_send(int fd,int logincount,int player)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, logincount);
	checksum += util_mkint(buffer, player);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PLAYERNUMGET_SEND, buffer);
}

/*
   servertoclient Echo( string test );
       Echo¤ËÂÐ¤¹¤ëÊÖÅú¡£
       
 	string test
 	¤µ¤­¤Û¤ÉÆþÎÏ¤µ¤ì¤¿Ê¸»úÎó¡£¥¨¥¹¥±¡¼¥×¤·¤Ê¤¤¡£
*/
void lssproto_Echo_send(int fd,char* test)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, test);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_ECHO_SEND, buffer);
}

// CoolFish: Trade 2001/4/18

void lssproto_TD_send(int fd, int index, char* message)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_TD_SEND, buffer);
}
#ifdef _CHATROOMPROTOCOL			// (¤£¥i¶}) Syu ADD ²á¤Ñ«ÇÀW¹D
void lssproto_CHATROOM_send ( int fd , char* message )
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHATROOM_SEND, buffer);
}
#endif
#ifdef _NEWREQUESTPROTOCOL			// (¤£¥i¶}) Syu ADD ·s¼WProtocol­n¨D²Ó¶µ
void lssproto_RESIST_send ( int fd , char* message )
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RESIST_SEND, buffer);
}
#endif
#ifdef _OUTOFBATTLESKILL			// (¤£¥i¶}) Syu ADD «D¾Ô°«®É§Þ¯àProtocol
void lssproto_BATTLESKILL_send ( int fd , char* message )
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_BATTLESKILL_SEND, buffer);
}
#endif
void lssproto_NU_send(int fd, int nu)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, nu);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_NU_SEND, buffer);
}


void lssproto_FM_send(int fd, char* message)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_FM_SEND, buffer);
}


void lssproto_WO_send(int fd,int effect)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, effect);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_WO_SEND, buffer);
}
#ifdef _ITEM_CRACKER
void lssproto_IC_send(int fd, int x, int y)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_IC_SEND, buffer);
}
#endif
#ifdef _MAGIC_NOCAST             // ºëÆF:¨IÀq
void lssproto_NC_send(int fd,int flg)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_NC_SEND, buffer);
}
#endif

#ifdef _CHECK_GAMESPEED
void lssproto_CS_send( int fd, int deltimes)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint( buffer, deltimes);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CS_SEND, buffer);
}
#endif

#ifdef _STREET_VENDOR
void lssproto_STREET_VENDOR_send(int fd,char *message)
{
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	checksum += util_mkstring(buffer,message);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_STREET_VENDOR_SEND,buffer);
}
#endif

#ifdef _RIGHTCLICK
void lssproto_RCLICK_send(int fd, int type, char* data)
{
	char buffer[65500];
	int checksum=0;

	print("\n RCLICK_send( type=%d data=%s) ", type, data );

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, type);
	checksum += util_mkint(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RCLICK_SEND, buffer);
}
#endif

#ifdef _JOBDAILY
void lssproto_JOBDAILY_send(int fd,char *data)
{
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	//print("tocli_data:%s ",data);
	checksum += util_mkstring(buffer,data);
	util_mkint(buffer,checksum);
	//print("tocli_buffer:%s ",buffer);
	util_SendMesg(fd,LSSPROTO_JOBDAILY_SEND,buffer);
}
#endif

#ifdef _TEACHER_SYSTEM
void lssproto_TEACHER_SYSTEM_send(int fd,char *data)
{
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	checksum += util_mkstring(buffer,data);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_TEACHER_SYSTEM_SEND,buffer);
}
#endif

#ifdef _ADD_STATUS_2
void lssproto_S2_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_S2_SEND, buffer);
}
#endif
