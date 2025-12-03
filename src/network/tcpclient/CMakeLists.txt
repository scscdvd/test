aux_source_directory(. TCP_CLIENT_SRC)

add_library(tcpclient ${TCP_CLIENT_SRC})

target_link_libraries(tcpclient
            log)

target_include_directories(tcpclient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})