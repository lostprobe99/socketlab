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