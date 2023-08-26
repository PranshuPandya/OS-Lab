#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
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
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_draw(void)
{
  void *buf;
  uint sz;
  argptr(0, (void *)&buf, sizeof(buf));
  argptr(1, (void *)&sz, sizeof(sz));

  char txt[] = "\\      .--..--..--..--..--..--.\n\
    .' \\  (`._   (_)     _   \n\
  .'    |  '._)         (_)  |\n\
  \\ _.')\\      .----..---.   /\n\
  |(_.'  |    /    .-\\-.  \\  |\n\
  \\     0|    |   ( O| O) | o|\n\
   |  _  |  .--.____.'._.-.  |\n\
   \\ (_) | o         -` .-`  |\n\
    |    \\   |`-._ _ _ _ _\\ /\n\
    \\    |   |  `. |_||_|   |\n\
    | o  |    \\_      \\     |     -.   .-.\n\
    |.-.  \\     `--..-'   O |     `.`-' .'\n\
  _.'  .' |     `-.-'      /-.__   ' .-'\n\
.' `-.` '.|='=.='=.='=.='=|._/_ `-'.'\n\
`-._  `.  |________/\\_____|    `-.'\n\
   .'   ).| '=' '='\\/ '=' |\n\
   `._.`  '---------------'\n\
           //___\\   //___\n\
             ||       ||\n\
    PPP      ||_.-.   ||_.-.\n\
            (_.--__) (_.--__)";
  ;

  if (sizeof(txt) > sz)
  {
    return -1;
  }

  strncpy((char *)buf, txt, sz);
  return sizeof(txt);
}

int sys_thread_create(void)
{
	  int fcn, arg;
	  char *stack;

	  //argint(int n, int *ip)
	  //argptr(int n, char **pp, int size)
	  if(argint(0, &fcn) < 0 || fcn < 0 || fcn >= myproc()->sz ||
			  argint(1, &arg) < 0 || arg < 0 || arg >= myproc()->sz ||
			  argptr(2, &stack, PGSIZE) < 0
			  ){ //buffer belongs to current process

		  return -1;
	  }

	  return thread_create((void(*)(void*))fcn, (void *)arg, stack);

	  /*
	   refer to bootmain:
	   void (*entry)(void);
	   entry = (void(*)(void))(elf->entry); //uint entry
  	   entry();
	   */
}
//int thread_create(void(*fcn)(void*), void *arg, void*stack)

int sys_thread_join(void)
{
	  return thread_join();
}
//int thread_join(void)

int sys_thread_exit(void)
{
	  thread_exit();
	  return 0;
}