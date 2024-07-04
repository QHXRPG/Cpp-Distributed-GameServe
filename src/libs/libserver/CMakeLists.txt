cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_COMPILER "/usr/local/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")

if (CMAKE_BUILD_TYPE STREQUAL Release) 
	set(MyProjectName server)
else()
	set(CMAKE_BUILD_TYPE Debug)
	set(MyProjectName serverd)
endif()

message(STATUS "CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "ProjectName:${MyProjectName}")

project(${MyProjectName})

aux_source_directory(. SRCS)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../../libs")
include_directories(/usr/local/include)
include_directories(/usr/local/include/common)
include_directories(/usr/local/include/common/json)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -DEPOLL -lyaml-cpp")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_library(${MyProjectName} STATIC ${SRCS})
