include_directories(./ ./internal)

aux_source_directory(./internal INTERNAL)

add_library(simple_log SHARED simple_log.c ${INTERNAL})

add_subdirectory(./test)