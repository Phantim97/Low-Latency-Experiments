cmake_minimum_required(VERSION 3.8)

project("Low Latency" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

find_package(OpenMP)

include_directories(${CMAKE_SOURCE_DIR}/ScopedTimer)

add_executable("PercentileSearch" "main.cpp""numeric_types.h")

if(OpenMP_CXX_FOUND)
    target_link_libraries("PercentileSearch" PUBLIC OpenMP::OpenMP_CXX)
endif()