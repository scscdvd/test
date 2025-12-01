
aux_source_directory(. TCP_SERVER_SRC)

add_library(tcpserver ${TCP_SERVER_SRC})

target_include_directories(tcpserver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
