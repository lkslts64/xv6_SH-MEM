#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "shpage.h"
struct{
	struct shpage Shp[32];
	struct spinlock lock;
}Shared;

void
shpinit(void)
{
	int i,j;
	char A[16];
	for(i=0;i<=15;i++)
		A[i]=0;
	initlock(&Shared.lock,"Shared lock");
	acquire(&Shared.lock);
	for(i=0;i<=31;i++){
		keyassign(Shared.Shp[i].key,A,16); //initilizing key with zeros
		Shared.Shp[i].sharing=0;
		Shared.Shp[i].pa=0;
		for(j=0;j<=15;j++){
			Shared.Shp[i].shproc[j].proc=0;
			Shared.Shp[i].shproc[j].mapped=0;
		}
	}
	release(&Shared.lock);
}
int
keycmp(char *A,char *B)		//return 0 if they are not equal and ret 1 if they are 
{
	int i;

	for(i=0;i<=15;i++){
		if(A[i]!=B[i])
			return 0;
	}
	return 1;
}

void
keyassign(char *A,char *B,int size) //assign B to A
{
	int i;
	for(i=0;i<=size-1;i++)
		A[i]=B[i];
}
//Return -1 if shared_pages>=32 or one shared_page is shared by >=16 procs
// (permissions) . Return 1 if everything goes smoothly
int
addshpage(char *Key,int *alloc)
{
	char A[16];
	int i,k=-1,counter=0,ret;
		
	for(i=0;i<=15;i++)	//i need this just for comparison (keycmp) 
		A[i]=0;
	acquire(&Shared.lock);
	for(i=0;i<=31;i++){
		if(keycmp(Shared.Shp[i].key,Key)){
			ret=sh_update(i);
			release(&Shared.lock);
			return ret;
		}
		if(keycmp(Shared.Shp[i].key,A) && counter==0){ //no need to have the variable counter but if we didnt
			k=i;			 	// the new shared page would have been located at the last 
			counter++;			// entries of the array (Shp)
		}
	}
	if(k<0) {			//this condition checks if <=32 sh_pages are currently existing
		cprintf("32 shared pages already existing!!!!\n");
		release(&Shared.lock);
		return k; 
	} 
	ret=sh_update(k);
	keyassign(Shared.Shp[k].key,Key,16);
	*alloc=1;
	release(&Shared.lock);
	return ret;

}
int
sh_update(int k)
{
	int i,ret=-1;
	struct proc *p=myproc();
	
	if(Shared.Shp[k].sharing>=16) 		//checks if this sh_page is shared with >=16 procs
		return ret;
	Shared.Shp[k].sharing++;		//now we are ready to add data to our structure
	for(i=0;i<=15;i++){
		if(Shared.Shp[k].shproc[i].proc==0){ 
			Shared.Shp[k].shproc[i].proc=p;
			ret=Shared.Shp[k].shproc[i].mapped=addpshpage();
			break;
		}
	}
	return ret;
}

void *
setsh_pa(void *pa,char *Key)
{
	int i;
	acquire(&Shared.lock);
	for(i=0;i<=31;i++){
		if(keycmp(Shared.Shp[i].key,Key)){
			Shared.Shp[i].pa=pa;
			release(&Shared.lock);
			return pa;
		}
	}
	release(&Shared.lock);
	return 0;
}
char *
getsh_pa(char *Key)
{
	int i;
	acquire(&Shared.lock);
	for(i=0;i<=31;i++){
		if(keycmp(Shared.Shp[i].key,Key)){
			release(&Shared.lock);
			return (Shared.Shp[i].pa);
		}
	}
	release(&Shared.lock);
	return 0;
}
int
forkshpage(struct proc *np)
{
	int i,j;
	char A[16];
	struct proc *p=myproc();
	
	for(i=0;i<=15;i++)
		A[i]=0;
	acquire(&Shared.lock);
	for(i=0;i<=31;i++){
		if(keycmp(Shared.Shp[i].key,A))
			continue;
		for(j=0;j<=15;j++){
			if(Shared.Shp[i].shproc[j].proc==p){
				if(Shared.Shp[i].sharing>=16){
					release(&Shared.lock);
					return -1; //fail if a shpage is shared by 16 procs
				}
				forksh_update(i,Shared.Shp[i].shproc[j].mapped,np);
				break;
			}
		}
	}
	release(&Shared.lock);
	return 0;

}
void
forksh_update(int pos,uint mapped,struct proc *np)
{
	int j;

	for(j=0;j<=15;j++){
		if(Shared.Shp[pos].shproc[j].proc==0){
			Shared.Shp[pos].shproc[j].proc=np;
			Shared.Shp[pos].shproc[j].mapped=mapped;
			Shared.Shp[pos].sharing++;
			break;
		}
	}
}
int
exitshpage(struct proc *p) 		
{
	int i,j;
	char A[16];
	
	for(i=0;i<=15;i++)
		A[i]=0;
	acquire(&Shared.lock);
	for(i=0;i<=31;i++){
		if(keycmp(Shared.Shp[i].key,A))
			continue;
		for(j=0;j<=15;j++){
			if(Shared.Shp[i].shproc[j].proc==p){
				if(Shared.Shp[i].sharing<1){
					release(&Shared.lock);
					break; // 
				}
				else if(Shared.Shp[i].sharing==1){
					Shared.Shp[i].shproc[j].proc=0;
					Shared.Shp[i].shproc[j].mapped=0;
					keyassign(Shared.Shp[i].key,A,16);
					Shared.Shp[i].pa=0;
					Shared.Shp[i].sharing--;
					break;
				}
				else{					
					Shared.Shp[i].shproc[j].proc=0;
					Shared.Shp[i].shproc[j].mapped=0;
					Shared.Shp[i].sharing--;
					break;
				}
			}
		}
	}
	release(&Shared.lock);
	return 0;
}		

// Return 0 if there is only one process sharing this page or 1 if there are more .
// If the given key does not exist at Shp , return -1
int
removeshpage(char *Key,int *alloc)
{
	int i,j,mapped=-1;
	char A[16];
	struct proc *p=myproc();

	for(i=0;i<=15;i++)
		A[i]=0;	
	acquire(&Shared.lock);
	for(i=0;i<=31;i++){
		if(keycmp(Shared.Shp[i].key,Key)){
			for(j=0;j<=15;j++){
				if(Shared.Shp[i].shproc[j].proc==p){
					Shared.Shp[i].shproc[j].proc=0;
					mapped=Shared.Shp[i].shproc[j].mapped;
					removepshpage(mapped);
					Shared.Shp[i].shproc[j].mapped=0;
				}
			}
			Shared.Shp[i].sharing--;
			if(Shared.Shp[i].sharing==0){
				keyassign(Shared.Shp[i].key,A,16);
				Shared.Shp[i].pa=0;
				*alloc=1;
				release(&Shared.lock);
				return mapped;
			}
			else if(Shared.Shp[i].sharing>0){
				release(&Shared.lock);
				return mapped;
			}
		}
	}
	release(&Shared.lock);
	return -1;
	
}
//Return 1 if sem_init should be called or 0 instead.
int 
sema_shp(char *Key)
{
	int i;
	acquire(&Shared.lock);
	for(i=0;i<=31;i++){
		if(keycmp(Key,Shared.Shp[i].key)){
			if(Shared.Shp[i].sharing==0){
				release(&Shared.lock);	
				return 1;
			}
			else{
				release(&Shared.lock);
				return 0;
			}
		}
	}
	release(&Shared.lock);
	return -1;
}
void
printkey(char *A)
{
	int i;

	for(i=0;i<=15;i++)
		cprintf("%d,",A[i]);


}
void printShptable(void)
{
	int i;
	int j;
	char A[16];

	for(i=0;i<=15;i++)
		A[i]=0;
	acquire(&Shared.lock);
	for(i=0;i<=31;i++){
		if(keycmp(Shared.Shp[i].key,A))
			continue;
		cprintf("Key is");
		printkey(Shared.Shp[i].key);
		cprintf(" pa is %d sharing is %d \n",Shared.Shp[i].pa,Shared.Shp[i].sharing);
		for(j=0;j<=15;j++){
			if(Shared.Shp[i].shproc[j].proc==0)
				continue;
			cprintf("process is %s \t and mapped at %d \n",Shared.Shp[i].shproc[j].proc->name,Shared.Shp[i].shproc[j].mapped);
		}
	}
	release(&Shared.lock);
}

