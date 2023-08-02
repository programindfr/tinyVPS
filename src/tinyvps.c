// #include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "tinyvps.h"
#include "create.h"
#include "run.h"
#include "parser.h"



int
main(int argc, char *argv[])
{
    vm newVm = {
        .baseurl = "https://fai-project.org/cgi/faime.cgi?",
        .type = "cloud",
        .disksize = "3",
        .format = "raw.zst",
        .uuid = "",
        .username = "debian",
        .userpw = "debian",
        .partition = "ONE",
        .keyboard = "fr",
        .suite = "bookworm",
        .variables = "cl5=SSH_SERVER;cl6=STANDARD;cl8=REBOOT",
        .varend = "sbm=2",
        .ram = "512M",
        .core = "1",
        .port = { { "5555", "22" } },
        .portCount = 1
    };
    // https://fai-project.org/cgi/faime.cgi?
    // type=cloud
    // ;disksize=3
    // ;format=raw.zst
    // ;hostname=uuid
    // ;username=debian
    // ;userpw=debian
    // ;partition=ONE
    // ;keyboard=fr
    // ;suite=bookworm
    // ;cl5=SSH_SERVER;cl6=STANDARD;cl8=REBOOT
    // ;sbm=2

    int createVmFlag = 0;
    int runVmFlag = 0;

    int c;
    while ((c = getopt(argc, argv, "Cb:t:d:i:u:n:p:P:k:s:v:V:R:r:c:f:")) != -1)
    {
        switch (c)
        {
        case 'C':
            createVmFlag = 1;
            break;

        case 'b':
            strcpy(newVm.baseurl, optarg);
            break;

        case 't':
            strcpy(newVm.type, optarg);
            break;

        case 'd':
            strcpy(newVm.disksize, optarg);
            break;

        case 'i':
            strcpy(newVm.format, optarg);
            break;

        case 'u':
            strcpy(newVm.uuid, optarg);
            break;

        case 'n':
            strcpy(newVm.username, optarg);
            break;

        case 'p':
            strcpy(newVm.userpw, optarg);
            break;

        case 'P':
            strcpy(newVm.partition, optarg);
            break;

        case 'k':
            strcpy(newVm.keyboard, optarg);
            break;

        case 's':
            strcpy(newVm.suite, optarg);
            break;

        case 'v':
            strcpy(newVm.variables, optarg);
            break;

        case 'V':
            strcpy(newVm.varend, optarg);
            break;

        case 'R':
            runVmFlag = 1;
            strcpy(newVm.uuid, optarg);
            break;

        case 'r':
            strcpy(newVm.ram, optarg);
            break;

        case 'c':
            strcpy(newVm.core, optarg);
            break;

        case 'f':
            port_parsing(optarg, &newVm);
            break;

        default:
            break;
        }
    }

    if (createVmFlag)
    {
        dcreate_vm(&newVm);
    } else
    if (runVmFlag)
    {
        drun_vm(&newVm);
    }
    
    // printf("createVmFlag = %d, runVmFlag = %d\n", createVmFlag, runVmFlag);

    return 0;
}