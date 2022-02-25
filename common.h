#ifndef COMMON_H
#define COMMON_H

#define ERROR_HANDLING(str)                         \
    do                                              \
    {                                               \
        char buf[256], *fmt = "%s:%d: %s\n";        \
        sprintf(buf, fmt, __FILE__, __LINE__, str); \
        perror(buf);                                \
        exit(1);                                    \
    } while (0)

#endif // COMMON_H