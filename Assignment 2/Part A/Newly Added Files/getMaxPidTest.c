#include "user.h"
#include "types.h"
#include "stat.h"
#include "processInfo.h"
int main(int argc, char *argv[])
{

    printf(1, "getMaxPid\n-----------------\n");
    printf(1, "The maxpid of till now: %d\n\n", getMaxPid());
    exit();
}