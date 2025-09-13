# 添加 simple_log_test 可执行文件
add_executable(test_simple_log test_simple_log.c)
target_link_libraries(test_simple_log simple_log)
target_link_libraries(test_simple_log cunit)

# 添加 simple_log_sink_test 可执行文件
add_executable(test_simple_log_sink test_simple_log_sink.c)
target_link_libraries(test_simple_log_sink simple_log)
target_link_libraries(test_simple_log_sink cunit) 
target_compile_options(test_simple_log PRIVATE -g)

include(CTest)
add_test(NAME test_simple_log COMMAND test_simple_log)