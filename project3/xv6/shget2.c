#include "types.h"
#include "stat.h"
#include "semt.h"
#include "user.h"

int
main(void)
{

	int k;
	struct sem_t *sem;
	char A[16];
	int *p;

	for(k=0;k<=15;k++)
		A[k]=k;	
	sem=shmget(A);
	if(check_sem(A))
		sem_init(sem,(int)sem);
	sem_down(sem);
	sem++;
	p=(int*)sem;
	for(k=1;k<=101;k+=2){
		*(p+k)=2;
	}
	printf(1,"Shared page with Key A array :\n");
	for(k=1;k<=101;k++)
		printf(1,"%d ",*(p+k));
	printf(1,"\n-----------------------------------------------------------\n");
	sem--;
	sem_up(sem);
	sleep(500);
	exit();
}
