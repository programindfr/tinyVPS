#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "concat.h"



char*
concat(int count, ...)    /* https://stackoverflow.com/a/11394336 */
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1;    // room for NULL
    va_start(ap, count);
    for (i=0; i<count; i++)
    {
        len += strlen(va_arg(ap, char*));
    }
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(len, sizeof(char));
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for (i=0; i<count; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}