#ifndef __TINYVPS_H__
#define __TINYVPS_H__

typedef struct tinyvps
{
    char uuid[37];      // uuid
    int disk;           // disk size in G
    int ram;            // ram size in G
    int core;           // core number
    int ssh;            // ssh host port
} vm;

#endif