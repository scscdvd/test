
aux_source_directory(. TCP_SERVER_SRC)

add_library(tcpserver STATIC ${TCP_SERVER_SRC})
target_link_libraries(tcpserver
            log)
# add_library(tcpserver SHARED ${TCP_SERVER_SRC})
target_include_directories(tcpserver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
