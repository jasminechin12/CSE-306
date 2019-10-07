
struct semaphore {
  unsigned int val;
  struct spinlock lock;
  // void * thread[NPROC];
  // unsigned int next;
  // unsigned int end;
};

void sem_init(struct semaphore *sp, int val);
void sem_P(struct semaphore *sp);
void sem_V(struct semaphore *sp);

