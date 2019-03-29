#include "types.h"
#include "stat.h"
#include "semt.h"
#include "user.h"


int
main(void)
{
	char Z[16];
	char U[16];
	char R[16];
	char Q[16];
	char Y[16];
	char N[16];
	char M[16];
	char L[16];
	char K[16];
	char G[16];
	char F[16];
	char D[16];
	char B[16];
	char A[16];
	char P[16];
	char O[16];
	char I[16];
	char H[16];
	char S[16];
	char C[16];
	int k;


	for(k=0;k<=15;k++){
		 Z[k]=99;
		 U[k]=98;
		 R[k]=97;
		 Q[k]=96;
		 Y[k]=95;
		 N[k]=94;
		 M[k]=92;
		 L[k]=91;
		 K[k]=90;
		 G[k]=89;
		 F[k]=88;
		 D[k]=87;
		 B[k]=86;
		 A[k]=85;
		 P[k]=84;
		 O[k]=83;
		 I[k]=82;
		 H[k]=81;
		 S[k]=80;
		 C[k]=78;
	}
	shmget(Z);
	shmget(U);
	shmget(R);
	shmget(Q);
	shmget(Y);
	shmget(N);
	shmget(M);
	shmget(L);
	shmget(K);
	shmget(G);
	shmget(F);
	shmget(D);
	shmget(B);
	shmget(A);
	shmget(P);
	shmget(O);
	shmget(I);
	shmget(H);
	shmget(S);
	shmget(C);
	sleep(200);
	exit();
}
		
