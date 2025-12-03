aux_source_directory(. UDP_SRC)

add_library(udp ${UDP_SRC})

target_link_libraries(udp
            log)
target_include_directories(udp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
