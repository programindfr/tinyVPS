#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "run.h"



void
run_vm(char *newargv[]) /* newargv must end with NULL - start a vm disk image with qemu */
{
    char *newenvp[2] = { "DISPLAY=:0.0", NULL };

    execve("/usr/bin/qemu-system-x86_64", newargv, newenvp);
    perror("execve");   /* execve() returns only on error */
}


void
drun_vm(char *uuid)
{
    char imagename[41]; /* 36 + 4 + 1 */
    strcpy(imagename, uuid);
    strcat(imagename, ".raw");

    char drivename[53]; /* 16 + 36 + 1 */
    strcpy(drivename, "file=");
    strcat(drivename, imagename);
    strcat(drivename, ",format=raw");

    char *newargv[13] = { "/usr/bin/qemu-system-x86_64", "-drive", drivename, "-m", "2G", "-enable-kvm", "-smp", "4", "-device", "e1000,netdev=net0", "-netdev", "user,id=net0,hostfwd=tcp::5555-:22", NULL };

    run_vm(newargv);
}