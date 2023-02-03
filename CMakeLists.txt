cmake_minimum_required(VERSION 3.16)

project(socketlab LANGUAGES C CXX)

if(MSVC)
    # 为 msvc 指定源文件为 utf8
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
endif()

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE ./include)

include_directories(${INCLUDE})

add_library(common_tool STATIC ${INCLUDE}/common.c)
if(WIN32)
    target_link_libraries(common_tool PRIVATE wsock32 ws2_32)
endif()

add_library(minish STATIC ${INCLUDE}/minish.c)
target_link_libraries(minish PRIVATE readline)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

aux_source_directory(src sources)
aux_source_directory(test TEST)
list(APPEND sources ${TEST})
aux_source_directory(src/ft FT)
list(APPEND sources ${FT})

# message(STATUS "Found ${sources} source files\n")
link_libraries(m)
foreach(main IN LISTS sources)
    message(STATUS "Found source file ${main}")
    get_filename_component(mainname ${main} NAME_WE)
    add_executable(${mainname} ${main})
    if(WIN32)
        # 连接 winsock2.lib
        target_link_libraries(${mainname} wsock32 ws2_32)
    endif()
    target_link_libraries(${mainname} ${CONAN_LIBS})
    target_link_libraries(${mainname} common_tool)
    target_link_libraries(${mainname} readline)
    target_link_libraries(${mainname} minish)
    if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
        target_link_libraries(${mainname} pthread)
    endif()
endforeach()