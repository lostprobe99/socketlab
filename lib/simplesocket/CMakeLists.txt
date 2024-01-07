include_directories(./include/simplesocket)

aux_source_directory(./src SRCS)

add_library(SimpleSocket SHARED ${SRCS})