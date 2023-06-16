#include <unistd.h>
#include <stdio.h>
#include "run.h"



void
run_vm(char *newargv[]) /* newargv must end with NULL - start a vm disk image with qemu */
{
    char *newenvp[] = { NULL };

    execve("qemu", newargv, newenvp);
    perror("execve");   /* execve() returns only on error */
}


void
drun_vm(char *uuid[])
{
    char *newargv[1] = {};
    run_vm(newargv);
}