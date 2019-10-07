#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "semaphore.h"



void sem_init(struct semaphore *sp, int val) {
  // int j;
  sp->val = val;
  initlock(&sp->lock, "sem");
  // for (j = 0; j < NPROC; j++)
  //   sp->thread[j] = 0;
  // sp->next = sp->end = 0;
}

void sem_P(struct semaphore *sp) {
  acquire(&sp->lock);

  while (sp->val <= 0) {
    // sp->thread[sp->end] = proc;
    // sp->end = (sp->end + 1) % NPROC;
    sleep(sp, &sp->lock);
  }
  sp->val = sp->val - 1;

  release(&sp->lock);
}

void sem_V(struct semaphore *sp) {
  acquire(&sp->lock);
  sp->val = sp->val + 1;

  // if (sp->thread[sp->next]) {
  //   wakeup(sp->thread[sp->next]);
  //   sp->thread[sp->next] = 0;
  //   sp->next = (sp->next + 1) % NPROC;
  // }
  wakeup(sp);
  release(&sp->lock);
}