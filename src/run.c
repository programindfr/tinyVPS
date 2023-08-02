#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "run.h"
#include "tinyvps.h"



void
run_vm(char *newargv[], char *newenvp[]) /* newargv and newenvp must end with NULL - start a vm disk image with qemu */
{
    execve("/usr/bin/qemu-system-x86_64", newargv, newenvp);
    perror("execve");   /* execve() returns only on error */
}


void
drun_vm(vm *newVm)
{
    char drivename[57]; /* 20 + 36 + 1 */
    strcpy(drivename, "file=");
    strcat(drivename, newVm->uuid);
    strcat(drivename, ".raw,format=raw");

    char *newargv[] = { "/usr/bin/qemu-system-x86_64", "-drive", drivename, "-m", newVm->ram, "-enable-kvm", "-smp", newVm->core, "-device", "e1000,netdev=net0", "-netdev", "user,id=net0,hostfwd=tcp::5555-:22", NULL };

    char *newenvp[2] = { "DISPLAY=:0.0", NULL };    // { NULL } with -nographic

    run_vm(newargv, newenvp);
}