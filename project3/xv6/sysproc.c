#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "shpage.h"
#include "semt.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}
int
sys_shmget(void)
{
  int addr;
  char *A;
  int alloc=0;
  struct proc *curproc=myproc();

  if(argptr(0, &A,16) < 0)
    return -1;
  if((addr=addshpage(A,&alloc))<0)
	return -1;
  if(sh_allocuvm(curproc->pgdir,addr,A,alloc) < 0)
	return -1;
  return addr;
}
int
sys_shmremove(void)
{
  char *A;
  int addr;
  int alloc=0;
  struct proc *p=myproc();	

  if(argptr(0, &A,16) < 0)
    return -1;
  if((addr=removeshpage(A,&alloc))<0)
    return -1;
  if(sh_deallocuvm(p->pgdir,addr,alloc)<0)
    return -1;
  if(alloc)
   return 0;
  else
   return 1;
}
int
sys_check_sem(void)
{
	char *A;
	int i;

	if(argptr(0, &A,16) < 0)
    		return -1;
	i=sema_shp(A);
	return i;
}
int
sys_sem_init(void)		//-----ta semaphores tha briskontai ston pirina stin domh Shared(prwin Shp)-----------------------------
{
	struct sem_t *sem;
	int i;
	if(sem_argptr(0,&sem,100)<0 || argint(1,&i) <0)
		return -1;
	sem_ini(sem,i);
	return 0;


}
int
sys_sem_up(void)
{
	struct sem_t *sem;
	if(sem_argptr(0,&sem,100)<0)
		return -1;
	sem_p(sem);
	return 0;

}
int
sys_sem_down(void)
{
	struct sem_t *sem;
	if(sem_argptr(0,&sem,100)<0)
		return -1;
	sem_v(sem);
	return 0;
}
int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
