aux_source_directory(. LOG_SRC)

add_library(log ${LOG_SRC})

target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})