#include "types.h"
#include "user.h"

#define CRCT(x) 4096 - x

int numGenerator(int num)
{
    int smallNum = num % 10;
    return (64 * smallNum + 73 * smallNum * smallNum + 20 * smallNum * smallNum * smallNum + 69 * smallNum * smallNum * smallNum * smallNum);
    /*
    20, 64, 73 are our roll numbers (~^__^)~
    */
}

void validator(int ind, int num, int *correct)
{
    if (numGenerator(ind) == num)
        *correct += 4;
    return;
}

int main(int argc, char *argv[])
{

    int i = 1;
    while (i <= 20)
    {
        if (fork() == 0)
        {
            int *ptr[10];
            printf(1, "child process number: %d\n", i);
            int j = 0;
            while (j < 10)
            {
                ptr[j] = (int *)malloc(4096);
                int k = 0;
                while (k < 1024)
                {
                    ptr[j][k] = numGenerator(k);
                    k++;
                }
                j++;
            }
            j = 0;
            while (j < 10)
            {
                int correctBytes = 0;
                int k = 0;
                while (k < 1024)
                {
                    validator(k, ptr[j][k], &correctBytes);
                    k++;
                }
                printf(1, "Iteration number %d: Number of Incorrect Bytes: %d\n", j + 1, CRCT(correctBytes));
                j++;
            }
            printf(1, "\n");
            exit();
        }
        i++;
    }

    i = 0;
    while (i < 20)
    {
        wait();
        i++;
    }
    exit();
}