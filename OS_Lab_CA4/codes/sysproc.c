#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int my_variable;

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

int sys_sem_init(void)
{
  int sem_id, value;
  char *name;
  if (argint(0, &sem_id) < 0 || argint(1, &value) < 0 || argstr(2, &name) < 0)
    return -1;

  if (sem_id < 0 || sem_id >= NSEM)
    return -1;

  sem_init(sem_id, value, name);
  return 0;
}


int
sys_sem_acquire(void)
{
  int sem_id;
  if(argint(0, &sem_id) < 0)
    return -1;

  if(sem_id < 0 || sem_id >= NSEM)
    return -1;

  sem_acquire(sem_id);
  return 0;
}

int
sys_sem_release(void)
{
  int sem_id;
  if(argint(0, &sem_id) < 0)
    return -1;

  if(sem_id < 0 || sem_id >= NSEM)
    return -1;

  sem_release(sem_id);
  return 0;
}

int sys_setvar(void)
{
  int value;
  if (argint(0, &value) < 0)
    return -1;
  my_variable = value;
  return 0;
}

int sys_getvar(void)
{
  return my_variable;
}

int sys_modvar(void)
{
  int value;
  if (argint(0, &value) < 0)
    return -1;
  my_variable += value;
  return 0;
}