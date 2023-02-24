#include <stdio.h>
#include <unistd.h>
#include <uuid/uuid.h>



void add(char *strout, char *strin1, char *strin2);
void create_vm(char *newargv[]);
void dcreate_vm(void);
void run_vm(char *uuid);
void drun_vm(void);
void run_vms(char *uuids[]);



int
main(int argc, char *argv[])
{
    return 0;
}


void
add(char *strout, char *strin1, char *strin2)   /* concat two string in the first /!\ buffer overflow can occur */
{
    int i = 0;
    int j = 0;

    while (strin1[i] != '\0') {
        strout[i] = strin1[i];
        i++;
    }

    while (strin2[j] != '\0') {
        strout[i + j] = strin2[j];
        j++;
    }

    strout[i + j] = '\0';
}


void
create_vm(char *newargv[])  /* create a vm disk image with https://fai-project.org/doc/man/fai-diskimage.html */
{
    char *newenvp[] = { "FAI_BASEFILEURL=https://fai-project.org/download/basefiles/", NULL };

    execve("fai-diskimage", newargv, newenvp);
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
    add(imagename, uuid, ".raw");   /* concat uuid an ".raw" in imagename */

    char *newargv[] = { "fai-diskimage", "-vu", uuid, "-S5G", "-cDEBIAN,STRETCH64,AMD64,FAIBASE,GRUB_PC,DHCPC,DEMO,CLOUD,LAST", imagename, NULL };  /* looking for -c in doc */

    create_vm(newargv);   
}


void
run_vm(char *uuid)
{
    char *newargv[] = { "qemu", NULL };
    char *newenvp[] = { NULL };

    execve("qemu", newargv, newenvp);
    perror("execve");
}