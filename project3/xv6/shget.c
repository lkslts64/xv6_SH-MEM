#include "types.h"
#include "stat.h"
#include "semt.h"
#include "user.h"


int
main(void)
{
	int k;
	char A[16];
	char B[16];
	char C[16];
	char D[16];
	char E[16];
	char F[16];
	char G[16];
	char H[16];
	char I[16];
	char J[16];
	char K[16];
	char L[16];
	char M[16];
	char N[16];
	char O[16];
	char P[16];
	char Q[16];
	int *p;
	int pid;
	struct sem_t *sem;
	struct sem_t *semn;

	for(k=0;k<=15;k++){
		A[k]=k;	
		B[k]=k+1;
		C[k]=3;
		D[k]=8;
		E[k]=5;
		F[k]=6;
		G[k]=7;
		H[k]=4;
		I[k]=9;
		J[k]=11;
		K[k]=22;
		L[k]=33;
		M[k]=44;
		N[k]=55;
		O[k]=66;
		P[k]=64;
		Q[k]=32;
	}
	sem=shmget(A);
        if(check_sem(A))
		sem_init(sem,0);
	sem_down(sem);
	sem++;
	p=(int*)sem;
	for(k=0;k<=100;k+=2){
		*(p+k)=4;
	}
	printf(1,"Shared page with Key A array :\n");
	for(k=0;k<=100;k++)
		printf(1,"%d ",*(p+k));
	printf(1,"\n-----------------------------------------------------------\n");
	sem--;
	sem_up(sem);
	semn=shmget(B);
	if(check_sem(B))
		sem_init(semn,0);
	sem_down(semn);
	semn++;
	p=(int*)semn;
	for(k=1;k<101;k+=2){
		*(p+k)=1;
	}
	printf(1,"Shared page with Key B array :\n");
	for(k=0;k<=100;k++)
		printf(1,"%d ",*(p+k));
	printf(1,"\n-----------------------------------------------------------\n");
	semn--;
	sem_up(semn);
	
	pid=fork();
	if(pid==0){		//testing fork()...
		sem_down(semn);
		semn++;
		p=(int*)semn;
		for(k=0;k<100;k+=2){
			*(p+k)=8;
		}
		printf(1,"Shared page with Key B array :\n");
		for(k=0;k<100;k++)
			printf(1,"%d,",*(p+k));
		printf(1,"\n-----------------------------------------------------------\n");
		semn--;
		sem_up(semn);
		
		exit();
	}
	if(pid<0){
		printf(1,"**************************************************\n");
		//exit();
	}
	wait(); 		//wait for the child to exit() and resumem with paremt...
	shmremove(B);
	shmremove(A); 		
	shmget(C);	//get some pages...
	shmget(D);
	shmget(E);
	shmget(F);
	shmget(G);
	shmget(H);
	shmget(I);
	shmget(J);
	shmremove(E);
	shmget(K);
	shmremove(I);
	shmget(L);
	shmremove(G);
	shmget(M);
	shmremove(C);
	shmget(N);
	shmget(O);
	shmget(P);
	shmremove(K);
	shmget(Q);
	shmremove(D);
	shmremove(F);
	shmremove(H);
	shmremove(J);
	shmremove(L);
	shmremove(M);
	for(k=1;k<=3;k++){	//testing multiple forks().....
		fork();
		
	}
	//We want ALL the forks() to be done when we enter exit() , so we sleep...
	sleep(1000);
	exit();
}
