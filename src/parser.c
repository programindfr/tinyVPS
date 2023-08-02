#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "tinyvps.h"



void
port_parsing(char *value, vm *newVm)
{
    char port[10][12];
    int i = 0;

    char *sep;
    sep = strtok(value, ",");
    while (sep != NULL)
    {
        strcpy(port[i], sep);
        i++;
        sep = strtok(NULL, ",");
    }
    newVm->portCount = i;

    int k;
    for (int j=0; j<i; j++)
    {
        k = 0;
        sep = strtok(port[j], ":");
        while (sep != NULL)
        {
            strcpy(newVm->port[j][k], sep);
            k++;
            sep = strtok(NULL, ":");
        }
    }
}