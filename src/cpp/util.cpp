#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void errif(bool cond, char * s)
{
    if(cond)
    {
        perror(s);
        exit(EXIT_FAILURE);
    }
}