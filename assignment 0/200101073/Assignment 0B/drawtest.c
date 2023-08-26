#include "user.h"
#include "types.h"
#include "stat.h"
int main(){
    static char buf[2000];
    printf(1,"drawtest system call returns %d\n",draw((void*)buf,2000));

    printf(1,"%s",buf);
    exit();
}