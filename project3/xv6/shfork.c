#include "types.h"
#include "stat.h"
#include "semt.h"
#include "user.h"


int
main(void)
{
	char A[16];
	int k;
	
	for(k=0;k<=15;k++)
		A[k]=68;
	shmget(A);
	for(k=1;k<=4;k++){
		fork();
	}
	sleep(1000);
	exit();	
}
