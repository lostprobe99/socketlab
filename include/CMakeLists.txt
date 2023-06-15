
add_library(netlib SHARED common.c)
if(WIN32)
    target_link_libraries(netlib PRIVATE wsock32 ws2_32)
endif()

add_library(minish SHARED minish.c)
target_link_libraries(minish PRIVATE readline)

add_library(buffer SHARED buffer.c)