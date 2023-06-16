#include <stdio.h>
#include <unistd.h>
#include "create.h"
#include "run.h"



int
main(int argc, char *argv[])
{
    int createVmFlag = 0;
    int runVmFlag = 0;

    int c;
    while ((c = getopt(argc, argv, "cr")) != -1)
    {
        switch (c)
        {
        case 'c':
            createVmFlag = 1;
            break;
        
        case 'r':
            runVmFlag = 1;
            break;
        
        default:
            break;
        }
    }
    
    printf("createVmFlag = %d, runVmFlag = %d\n", createVmFlag, runVmFlag);

    return 0;
}