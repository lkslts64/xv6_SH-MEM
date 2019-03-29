#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "semt.h"

void
sem_ini(struct sem_t *sem,int value)
{
  initlock(&sem->lk, "sem lock");
  sem->locked = value;
  sem->pid = 0; 
}
void sem_v(struct sem_t *sem)
{
  acquire(&sem->lk);
  while (sem->locked) {
    sleep(sem, &sem->lk);
  }
  sem->locked = 1;
  sem->pid = myproc()->pid;
  release(&sem->lk);
}

void
sem_p(struct sem_t *sem)
{

  acquire(&sem->lk);
  sem->locked = 0;
  sem->pid = 0;
  wakeup(sem);
  release(&sem->lk);
}
