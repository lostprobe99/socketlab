include_directories(./include/)

aux_source_directory(./src SRCS)

add_library(NetPackets SHARED ${SRCS})