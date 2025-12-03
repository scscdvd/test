# 搜索当前目录所有源文件
aux_source_directory(. SRC_LIST)

# 子模块
add_subdirectory(network)
add_subdirectory(system)
add_subdirectory(log)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
# 生成可执行文件
add_executable(test ${SRC_LIST})

# 链接子模块库
target_link_libraries(test
    system
)
