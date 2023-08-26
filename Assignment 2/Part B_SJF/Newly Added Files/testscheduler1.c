#include "types.h"
#include "stat.h"
#include "user.h"

#include "processInfo.h"

void cpubounddelay(int val)
{
    int *data = (int *)malloc(sizeof(int) * 10000);
    if (data <= 0)
        printf(1, "Memory not allocated due to some error\n");

    for (int i4 = 0; i4 < val * 1000; i4++)
    {
        // for (int k = 0; k < 570; k++)
        for (int j = 0; j < 10000; j++)
            data[j]++;
        // printf(1, "\n-->%d %d\n", i,t[i]);
    }
}
void iobounddelay(int val)
{
    for (int i2 = 0; i2 < val* 10; i2++)
    {
        sleep(1);
    }
}
int main()
{
    {

        printf(1, "---------------------------Test 1 --------------------------------------\n");
        printf(1, "Process Type      Burst Time      Context Switches       PID\n");
        // array containing burst times
        int t[10] = {40, 70, 10, 90, 60, 30, 20, 80, 100, 50};
        long x = 0;
        // set_burst_time(1);

        for (int i = 0; i < 10; i++)
        {
            if (fork() == 0)
            {
                // printf(1,"child %d",i);
                x = set_burst_time(t[i]);

                if (x < 0)
                {
                    printf(1, "Counldn't set burst time for process %d\n", getpid());
                }

                // CPU bound process
                if (i % 2 == 0)
                {
                    cpubounddelay(t[i]);

                    printf(1, "CPU Bound  ");
                }

                // IO bound process
                else
                {
                    // mimicking IO wait
                    iobounddelay(t[i]);
                    printf(1, "IO Bound   ");
                }

                x = get_burst_time();
                struct processInfo *info;
                info = (struct processInfo *)malloc(sizeof(struct processInfo));
                getProcInfoStruct(getpid(), info);
                printf(1, "            %d               %d                %d\n", x, info->numberContextSwitches, getpid());
                exit();
            }
        }

        while (wait() != -1)
            ;
    }
    {
        printf(1, "\n---------------------------Test 2 --------------------------------------\n");
        printf(1, "Process Type      Burst Time      Context Switches      PID\n");
        // array containing burst times
        int t[10] = {30, 29, 28, 27, 26, 25, 24, 23, 22, 21};
        long x = 0;
        // set_burst_time(1);

        for (int i = 0; i < 10; i++)
        {
            if (fork() == 0)
            {
                // printf(1,"child %d",i);
                x = set_burst_time(t[i]);

                if (x < 0)
                {
                    printf(1, "Counldn't set burst time for process %d\n", getpid());
                }

                // CPU bound process
                if (i % 2 == 0)
                {
                    cpubounddelay(t[i]);

                    printf(1, "CPU Bound  ");
                }

                // IO bound process
                else
                {
                    // mimicking IO wait
                    iobounddelay(t[i]);
                    printf(1, "IO Bound   ");
                }

                x = get_burst_time();
                struct processInfo *info;
                info = (struct processInfo *)malloc(sizeof(struct processInfo));
                getProcInfoStruct(getpid(), info);
                printf(1, "            %d               %d                %d\n", x, info->numberContextSwitches, getpid());
                exit();
            }
        }

        while (wait() != -1)
            ;
    }
    {
        printf(1, "\n---------------------------Test 3 --------------------------------------\n");
        printf(1, "Process Type      Burst Time      Context Switches      PID\n");
        // array containing burst times
        int t[10] = {30, 28, 28, 28, 26, 25, 22, 22, 22, 22};
        long x = 0;
        // set_burst_time(1);

        for (int i = 0; i < 10; i++)
        {
            if (fork() == 0)
            {
                // printf(1,"child %d",i);
                x = set_burst_time(t[i]);

                if (x < 0)
                {
                    printf(1, "Counldn't set burst time for process %d\n", getpid());
                }

                // CPU bound process
                if (i % 2 == 0)
                {
                    cpubounddelay(t[i]);

                    printf(1, "CPU Bound  ");
                }

                // IO bound process
                else
                {
                    // mimicking IO wait
                    iobounddelay(t[i]);
                    printf(1, "IO Bound   ");
                }

                x = get_burst_time();
                struct processInfo *info;
                info = (struct processInfo *)malloc(sizeof(struct processInfo));
                getProcInfoStruct(getpid(), info);
                printf(1, "            %d               %d                %d\n", x, info->numberContextSwitches, getpid());
                exit();
            }
        }

        while (wait() != -1)
            ;
    }
    exit();
}