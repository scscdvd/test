aux_source_directory(. SYSTEM_SRC)

add_library(system ${SYSTEM_SRC})

target_link_libraries(system
    tcpclient    
    tcpserver
    udp  
)

target_include_directories(system PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})