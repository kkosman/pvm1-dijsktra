#include "pvm3.h"
#include <stdio.h>

#define SLAVENAME "slave"
#define SLAVENUM 4
#define NAMESIZE 64
#define MSG_MSTR 1
#define MSG_SLV 2

int main() {
	int mytid;
	int tids[SLAVENUM];
	char slave_name[NAMESIZE];
	int nproc, i, who;
	nproc = pvm_spawn(SLAVENAME, NULL, PvmTaskDefault, "", SLAVENUM, tids);
	mytid = pvm_mytid();
	printf("Master tid: %d \n", mytid);
	printf("debug: %d \n", nproc);

	for (i = 0 ; i < nproc ; i++) {
		pvm_initsend(PvmDataDefault);
		pvm_pkint(&mytid, 1, 1);
		pvm_pkint(&i, 1, 1);
		pvm_send(tids[i],MSG_MSTR);
	}
	for (i = 0 ; i < nproc ; i++) {
		pvm_recv(-1, MSG_SLV);
		pvm_upkint(&who, 1,1);
		pvm_upkstr(slave_name);
		printf("Master: proces %x jest na hoscie %s \n", who, slave_name);
	}

	pvm_exit();
}