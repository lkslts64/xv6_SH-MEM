//----------------------------

#include "spinlock.h"

struct sem_t {
  struct spinlock lk; // spinlock protecting this sleep lock
  int locked;
  
  // For debugging:
  char *name;        // Name of lock.
  int pid;           // Process holding lock
};

