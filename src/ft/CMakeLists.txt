include_directories(.)

add_executable(ft-client ft-client.c)
target_link_libraries(ft-client socklib)
target_link_libraries(ft-client minish)

add_executable(ft-server ft-server.c)
target_link_libraries(ft-server socklib)