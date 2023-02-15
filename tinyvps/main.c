#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <uuid/uuid.h>



void create_vm_disk_img();



int
main(int argc, char *argv[])
{
    return 0;
}


void
create_vm_disk_img()
{
    char *newargv[] = { NULL, NULL };
    char *newenvp[] = { "FAI_BASEFILEURL=https://fai-project.org/download/basefiles/", NULL };
    uuid_t binuuid; /* uuid in binary format */
    char uuid[37];

    uuid_generate_random(binuuid);
    uuid_unparse(binuuid, uuid);

    execve("./", newargv, newenvp); /* exact command needed */
    perror("execve");   /* execve() returns only on error */
}