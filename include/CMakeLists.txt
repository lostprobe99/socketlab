add_library(socklib SHARED common.c util.c )
if(WIN32)
    target_link_libraries(socklib PRIVATE wsock32 ws2_32)
endif()

add_library(minish SHARED minish.c)
target_link_libraries(minish PRIVATE readline)