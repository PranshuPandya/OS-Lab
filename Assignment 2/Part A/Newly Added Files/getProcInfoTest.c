#include "user.h"
#include "types.h"
#include "stat.h"
#include "processInfo.h"
int main(int argc, char *argv[])
{
    // declaring the user memory space for a process
    if(argc != 2){
        printf(1,"please enter the pid of the process to get info\n");
        exit();
    }
    int pid = atoi(argv[1]);

    set_burst_time(5000);

    struct processInfo *p = malloc(sizeof(struct processInfo));

    // getProcInfo() system call is used from user.h which returns 0 if required process is found in the system and -1 otherwise.
    int value = getProcInfoStruct(pid, p);

    printf(1, "\nProcess Information\n------------------------------------\n");
    if (value == -1)
    {
        printf(1, "Process Not Found\n");
    }
    else
    {
        // printing the information for the required process.
        printf(1, "Parent Process ID: %d\nProcess Size: %d\nNumber of Context Switches: %d\n", p->ppid, p->psize, p->numberContextSwitches);
    }
    // printf(1, "\nBurst time of the current process: %d\n", get_burst_time());
    exit();
}