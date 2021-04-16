
#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SLAVENAME "slave"
#define SLAVENUM 4
#define NAMESIZE 64
#define MSG_MSTR 1
#define MSG_SLV 2

int main() {
	int ptid, my2, mytid;
	char *str = malloc(NAMESIZE);
	int i = pvm_recv(-1, MSG_MSTR);
	if( i < 0 ) {
		perror("Nieoczekiwany blad");
		exit(0);
	}
	pvm_upkint(&ptid, 1, 1);
	pvm_upkint(&my2, 1, 1);
	printf("Slave: otrzymalem %x %d\n", ptid, my2);
	mytid = pvm_mytid();
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&mytid, 1,1);
	gethostname(str, NAMESIZE);
	str[NAMESIZE - 1] = 0;
	pvm_pkstr(str);
	pvm_send(ptid, MSG_SLV);
	pvm_exit();
}