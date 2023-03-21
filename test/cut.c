#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char * cut(const char * s)
{
    const char * last, *p = s;
    while(*p)
    {
        if(*p == '/' || *p == '\\')
            last = p;
        p++;
    }

    return last + 1;
}

int main(int argc, char ** argv)
{
    printf("%s\n", cut(__FILE__));

    return 0;
}