include_directories(./ ./internal)

aux_source_directory(./internal INTERNAL)

add_library(simple_log SHARED simple_log.c ${INTERNAL})

add_executable(simple_log_test simple_log_test.c)
target_link_libraries(simple_log_test simple_log)