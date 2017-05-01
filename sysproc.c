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

int
sys_exit(void)
{
	int status;
	if(argint(0, &status) < 0)
		return -1;
  //exit(0);
  exit(status);
  return 0;  // not reached
}

int
sys_wait(void)
{
	//updated to take the status from the USER SPACE call stack wait(0); argument is 0, gets referenced by int* status
	int *status;
	//if(argptr(0, void* &status, sizeof(int*))<0)
	if(argptr(0, (char**) &status, sizeof(int)) < 0)
		return -1;
	return wait(status);
  //return wait(0);
}

//My added waitpid implementation
int
sys_waitpid(void)
{
	//updated to take the status from the USER SPACE call stack wait(0); argument is 0, gets referenced by int* status
	int *status;
	int pid;
	int options;

	//int main(int argc, int argv[])
	//argv[0] -> is the same 0 as argint(0)
	//store that in pid by doing &pid
	//the 0 is the the

	//int *status
	//argptr(_,&status,_)
	if(argint(0, &pid) < 0)
		return -1;

	if(argptr(1, (char**) &status, sizeof(int))<0)
		return -1;

	if(argint(2, &options) < 0)
		return -1;

	return waitpid(pid, status, options); //this will call int waitpid(int, int*, int)
}


// int
// sys_setpriority(void)
// {
// 	int priority;
// 	if(argint(0, &priority) < 0) //store argument at address of priority of first argument
// 		return -1;
// 	setpriority(priority);
// 	return 0; //not reached
// }

int
sys_setpriority(void)
{
  return setpriority(20);
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
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
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
    if(proc->killed){
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


int
sys_hello(void)
{
	hello();
	return 0;
}
