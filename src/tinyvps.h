#ifndef __TINYVPS_H__
#define __TINYVPS_H__

typedef struct tinyvps
{
    char baseurl[64];       // API base url
    char type[32];          // install type
    char disksize[3];       // disk size int <=99 G
    char format[32];        // image format
    char uuid[37];          // uuid of vm
    char username[64];      // vm username
    char userpw[64];        // vm user password
    char partition[32];     // partition setting
    char keyboard[3];       // keyboard language
    char suite[32];         // vm distro
    char variables[128];    // some settings
    char varend[7];         // idk the purpuse of this var
    char ram[7];            // ram size with unit (M/G)
    char core[3];           // core number
    char port[10][2][6];    // port forwarding host/guest, [ [ "host", "gest" ], ... ]
    int portCount;
} vm;

#endif