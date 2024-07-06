cmake_minimum_required(VERSION 2.8)

# 设置项目名称和构建类型
if (CMAKE_BUILD_TYPE STREQUAL Release) 
    set(MyProjectName robots)
else()
    set(CMAKE_BUILD_TYPE Debug)
    set(MyProjectName robotsd)
endif()

message(STATUS "CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "ProjectName:${MyProjectName}")

project(${MyProjectName})

# 添加源文件目录
aux_source_directory(. SRCS)
aux_source_directory(../../libs/libserver SRCS)
aux_source_directory(../../libs/libserver/protobuf SRCS)

# 包含头文件目录
include_directories(../../libs)
include_directories(/usr/local/include)
include_directories(/usr/local/include/common)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../bin")

# 设置编译选项
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -llog4cplus -DEPOLL")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 设置链接目录
link_directories(${CMAKE_BINARY_DIR}/../../../libs)

# 查找 yaml-cpp 库
find_package(yaml-cpp REQUIRED)

# 添加可执行文件
add_executable(${MyProjectName} ${SRCS})

# 链接库
target_link_libraries(${MyProjectName} yaml-cpp)
