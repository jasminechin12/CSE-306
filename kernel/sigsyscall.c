#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"
#include "proc.h"

struct spinlock siglock;

int sys_sigsend(void) {
    int pid, sig;
    argint(0, &pid);
    argint(1, &sig);
    return sigsend(pid, sig);
}

int sys_sigsethandler(void) {
    int sig;
    argint(0, &sig);
    // ??????
}

void sys_sigreturn(void) {

}

int sys_siggetmask(void) {
    return siggetmask();
}

int sys_sigsetmask(void) {
    int *maskp;
    argint(0, maskp);
    return sigsetmask(maskp);
}

int sys_sigpause(void) {
    int sig;
    argint(0, &sig);
    return sigpause();
}
