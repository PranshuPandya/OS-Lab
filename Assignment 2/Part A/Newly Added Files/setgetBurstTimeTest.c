#include "user.h"
#include "types.h"
#include "stat.h"
#include "processInfo.h"
int main(int argc, char *argv[])
{

	// argv[0] is program name and argv[1] contains the process id from command line.
	// int pid = atoi(argv[1]);
	printf(1,"Setting Burst time to 5\n");
	set_burst_time(5);

	// declaring the user memory space for a process
	// struct processInfo *p = malloc(sizeof(struct processInfo));
	printf(1, "\nBurst time of the current process: %d\n", get_burst_time());
	exit();
}