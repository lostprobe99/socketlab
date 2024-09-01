#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include <sys/socket.h> // for getnameinfo
#include <netdb.h>  // for getnameinfo
#include <sys/types.h>
#include <ifaddrs.h>    // for getifaddrs, freeifaddrs

int main(int argc, char ** argv)
{
    struct ifaddrs *itfs, *itf;
    int family;
    char host[255] = {0};
    // struct sockaddr_dl hardware;

    if(getifaddrs(&itfs) < 0)
    {
        perror("[getifaddrs]");
        return -1;
    }

    itf = itfs;
    while(itf)
    {
        family = itf->ifa_addr->sa_family;
        printf("%-8s %s (%d)\n", itf->ifa_name
             , (family == AF_PACKET) ? "AF_PACKET"
             : (family == AF_INET) ? "AF_INET"
             : (family == AF_INET6) ? "AF_INET6"
             : "???"
             , family);
        if(family == AF_INET || family == AF_INET6)
        {
            if( getnameinfo(itf->ifa_addr
                , (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6)
                , host, sizeof(host), NULL, 0, NI_NUMERICHOST) != 0)
            {
                perror("[getnameinfo]");
                return -1;
            }
            printf("\t\taddr: [%s]\n", host);
        }
        itf = itf->ifa_next;
    }

    freeifaddrs(itfs);

    return 0;
}