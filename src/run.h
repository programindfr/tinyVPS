#ifndef __RUN_H__
#define __RUN_H__

#include "tinyvps.h"

void run_vm(char *newargv[], char *newenvp[]);
void drun_vm(vm *newVm);

#endif