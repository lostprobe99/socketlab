include_directories(./)

aux_source_directory(. SRCS)

add_library(simple_log SHARED ${SRCS})