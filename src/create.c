#include <uuid/uuid.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "concat.h"
#include "create.h"
#include "tinyvps.h"



void
create_vm(char *newargv[], char *newenvp[])  /* newargv and newenvp must end with NULL - create a vm disk image with https://fai-project.org/doc/api.html */
{
    execve("/usr/bin/sh", newargv, newenvp);
    perror("execve");   /* execve() returns only on error */
}


void
dcreate_vm(vm *newVm)    /* create a vm disk image named with uuid and setup a default config */
{
    if (newVm->uuid[0] = '\n')
    {
        uuid_t binuuid; /* uuid in binary format */
        char uuid[37];  /* 36 char uuid and \0 */
        uuid_generate_random(binuuid);
        uuid_unparse(binuuid, uuid);
        strcpy(newVm->uuid, uuid);
    }

    char *url = concat(
        23, newVm->baseurl,
        "type=", newVm->type,
        ";disksize=", newVm->disksize,
        ";format=", newVm->format,
        ";hostname=", newVm->uuid,
        ";username=", newVm->username,
        ";userpw=", newVm->userpw,
        ";partition=", newVm->partition,
        ";keyboard=", newVm->keyboard,
        ";suite=", newVm->suite,
        ";", newVm->variables,
        ";", newVm->varend
    );
    char *newargv[4] = { "/usr/bin/sh", "wget", url, NULL };
    free(url);

    char *newenvp[1] = { NULL };

    create_vm(newargv, newenvp);   
}