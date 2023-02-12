#ifndef FT_H
#define FT_H

enum ft_request_type {
    ERROR = -1,
    DATA = 0,
    LS = 1,
    PWD,
    CD,
    GET,
    PUT,
    EXIT,
    END = 127   // 表示一个响应的结束
};

char* ft_request_str[] = {
    "",
    "LS",
    "PWD",
    "CD",
    "GET",
    "PUT",
    "EXIT"
};


const char * _tcp_state_str_[] = {
    "",
    "TCP_ESTABLISHED",
    "TCP_SYN_SENT",
    "TCP_SYN_RECV",
    "TCP_FIN_WAIT1",
    "TCP_FIN_WAIT2",
    "TCP_TIME_WAIT",
    "TCP_CLOSE",
    "TCP_CLOSE_WAIT",
    "TCP_LAST_ACK",
    "TCP_LISTEN",
    "TCP_CLOSING"
};

long get_file_size(const char * filename)
{
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS) || defined(_WIN32)
    if(_access(filename, 04) == 0)  // 可读
        return _filelength(fileno(fopen(filename, "rb")));
#elif linux
    struct stat st;
    if(stat(filename, &st) == 0)
        return st.st_size;
#endif
    else
        return 0;
}

#endif // FT_H