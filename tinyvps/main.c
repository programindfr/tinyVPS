#include <stdio.h>
#include <errno.h>
#include <unistd.h>



void create_vm_disk_img();



int
main(int argc, char *argv[])
{
    return 0;
}


void
create_vm_disk_img()
{
    int rc; //handle return code
    char *newargv[] ={ NULL, NULL };
    char *newenvp[] = { NULL };

    rc = execve("", newargv, newenvp); //exact command needed

    if (rc == -1)
        fprintf(stderr, "error %d occur\n", errno);
}