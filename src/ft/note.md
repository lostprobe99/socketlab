[TOC]

## recv

```c
    #include <sys/types.h>
    #include <sys/socket.h>
    ssize_t recv(int sockfd, void *buf, size_t len, int flags);
    ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                    struct sockaddr *src_addr, socklen_t * addrlen);
```
`recv(sockfd, buf, len, 0)` 等价于 `read(sockfd, buf, len)`

`nm`：查看`.o`，`.a`，`.so`中的符号


```c
/* Divide S into tokens separated by characters in DELIM.  */
extern char *strtok (char *__restrict __s, const char *__restrict __delim)
     __THROW __nonnull ((2));
```
其中当`__delim`为`NULL`时，编译时会产生 warning