cmake_minimum_required(VERSION 2.8)

# 设置项目名称和构建类型
if (CMAKE_BUILD_TYPE STREQUAL Release) 
    set(MyProjectName server)
else()
    set(CMAKE_BUILD_TYPE Debug)
    set(MyProjectName serverd)
endif()

message(STATUS "CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "ProjectName:${MyProjectName}")

# 设置项目名称
project(${MyProjectName})

# 添加源文件目录
aux_source_directory(. SRCS)

# 包含头文件目录
include_directories(/usr/local/include/common)

# 手动设置 yaml-cpp 库和头文件路径
set(YAML_CPP_INCLUDE_DIR "/usr/include/windows/yaml-cpp")
set(YAML_CPP_LIBRARY "/usr/lib64/libyaml-cpp.so")

# 包含 yaml-cpp 头文件目录
include_directories(${YAML_CPP_INCLUDE_DIR})

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../../libs")

# 设置编译选项
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -DEPOLL")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 添加静态库
add_library(${MyProjectName} STATIC ${SRCS})

# 链接 yaml-cpp 库
target_link_libraries(${MyProjectName} ${YAML_CPP_LIBRARY})

