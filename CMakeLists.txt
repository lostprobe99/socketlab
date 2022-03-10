cmake_minimum_required(VERSION 3.16)

project(netlab)

if(MSVC)
    # 为 msvc 指定源文件为 utf8
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
endif()

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c)

message(STATUS "Found ${sources} source files")

foreach(main IN LISTS sources)
    get_filename_component(mainname ${main} NAME_WE)
    add_executable(${mainname} ${main})
    if(WIN32)
        # 连接 winsock2.lib
        target_link_libraries(${mainname} wsock32 ws2_32)
    endif()
endforeach()
