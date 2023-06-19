#include <uuid/uuid.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "create.h"



void
create_vm(char *newargv[])  /* newargv must end with NULL - create a vm disk image with https://fai-project.org/doc/man/fai-diskimage.html */
{
    char *newenvp[] = { "FAI_BASEFILEURL=https://fai-project.org/download/basefiles/", NULL };

    execve("/usr/sbin/fai-diskimage", newargv, newenvp);
    perror("execve");   /* execve() returns only on error */
}


void
dcreate_vm(void)    /* create a vm disk image named with uuid and setup a default config */
{
    uuid_t binuuid; /* uuid in binary format */
    char uuid[37];  /* 36 char uuid and \0 */

    uuid_generate_random(binuuid);
    uuid_unparse(binuuid, uuid);

    char imagename[41]; /* 36 + 4 + 1 */
    strcpy(imagename, uuid);
    strcat(imagename, ".raw");

    char *newargv[] = { "/usr/sbin/fai-diskimage", "-vu", uuid, "-S5G", "-cDEBIAN,DHCPC,DEMO,FAIBASE,CLOUD,BOOKWORM,SSH_SERVER,STANDARD,FAIME,GRUB_PC,AMD64", imagename, NULL };  /* looking for -c in doc */

    create_vm(newargv);   
}