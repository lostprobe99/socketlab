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