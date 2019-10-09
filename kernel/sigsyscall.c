#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"
#include "proc.h"

int sys_sigsend(void) {
    int pid, sig;
    argint(0, &pid);
    argint(1, &sig);
    return sigsend(pid, sig);
}

int sys_sigsethandler(void) {
    int sig, handler;
    argint(0, &sig);
    if (argptr(1, (void*)&handler, sizeof(*handler)) < 0)
        return -1
    return sigsethandler(sig, handler);
}

void sys_sigreturn(void) {

}

int sys_siggetmask(void) {
    return myproc()->maskedsigs;
}

int sys_sigsetmask(void) {
    int *maskp;
    int temp;
    if (argptr(0, (void*)&maskp, sizeof(*maskp)) < 0)
        return -1;
    temp = myproc()->maskedsigs;
    myproc()->maskedsigs = *maskp;
    *maskp = temp;
    return 0;
}

int sys_sigpause(void) {
    int sig;
    argint(0, &sig);
    return sigpause();
}
