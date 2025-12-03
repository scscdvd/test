cmake_minimum_required(VERSION 3.10)
project(MyProject  VERSION 1.0 LANGUAGES C CXX)

# 设置 C 标准 / C++ 标准
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)  # .a .lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)  # .so .dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)  # 可执行文件
# # 头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 子目录
add_subdirectory(src)

