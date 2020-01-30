#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

// Lab1: Updated
int
sys_exit(void)
{

  int exitstatus;

  if (argint(0, &exitstatus) < 0) return -1; // If we don't get our argument

  exit(exitstatus);

  return 0;  // not reached
}

// Lab1: Updated
int
sys_wait(void)
{

  char* exitstatus; // The argptr function only works with a char pointer so we use a char here

  if (argptr(0, &exitstatus, 8) < 0) return -1; // If we don't get our argument

  return wait( (int*) exitstatus); // Cast the char ptr back into a int ptr
}

// Lab1: created to enable waitpid
int sys_waitpid(void) {
  int pid, options;
  char* exitstatus; // The argptr function only works with a char pointer so we use a char here

  if((argint(0, &pid) < 0) || (argptr(1, &exitstatus, 8) < 0) || (argint(2, &options) < 0)) return -1; // Check that we get all our arguments

  return waitpid(pid, (int*) exitstatus, options);
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
