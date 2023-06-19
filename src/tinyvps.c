#include <stdio.h>
#include <unistd.h>
#include "tinyvps.h"
#include "create.h"
#include "run.h"



int
main(int argc, char *argv[])
{
    vm newVm = {
        .uuid = "",
        .disk = 5,
        .ram = 2,
        .core = 4,
        .ssh = 5555
    };

    int createVmFlag = 0;
    int runVmFlag = 0;
    char *runVmValue = NULL;

    int c;
    while ((c = getopt(argc, argv, "cr:")) != -1)
    {
        switch (c)
        {
        case 'c':
            createVmFlag = 1;
            dcreate_vm();
            break;
        
        case 'r':
            runVmFlag = 1;
            runVmValue = optarg;
            break;
        
        default:
            break;
        }
    }
    
    printf("createVmFlag = %d, runVmFlag = %d\n", createVmFlag, runVmFlag);

    return 0;
}