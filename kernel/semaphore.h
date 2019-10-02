struct semaphore {
    int val;
}

void sem_init(struct semaphore *sp, int val);
void sem_P(struct semaphore *sp);
void sem_V(struct semaphore *sp);
