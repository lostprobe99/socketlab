cmake_minimum_required(VERSION 3.16)

project(socketlab)

if(MSVC)
    # 为 msvc 指定源文件为 utf8
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(INC ./inc)

include_directories(${INC})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c)
aux_source_directory(src sources)

message(STATUS "Found ${sources} source files\n")

foreach(main IN LISTS sources)
    get_filename_component(mainname ${main} NAME_WE)
    add_executable(${mainname} ${main})
    if(WIN32)
        # 连接 winsock2.lib
        target_link_libraries(${mainname} wsock32 ws2_32)
    endif()
    target_link_libraries(${mainname} ${CONAN_LIBS})
endforeach()

enable_testing()
add_subdirectory(test)
aux_source_directory(test tests)

foreach(main IN LISTS tests)
    get_filename_component(mainname ${main} NAME_WE)
    add_executable(${mainname} ${main})
    add_test(NAME ${mainname} COMMAND ${mainname})  # COMMAND 要和 executable 的名字对应
    target_link_libraries(${mainname} ${CONAN_LIBS})
endforeach()
