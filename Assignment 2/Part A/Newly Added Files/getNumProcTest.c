#include "user.h"
#include "types.h"
#include "stat.h"
#include "processInfo.h"
int main(int argc, char *argv[])
{
	// static char buf[2000];
	// printf(1,"drawtest system call returns %d\n",draw((void*)buf,2000));
	printf(1, "getNumProc\n-----------------\n");
	printf(1, "The number of process: %d\n\n", getNumProc());
	exit();
}