cmake_minimum_required(VERSION 3.8)

project("Low Latency" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP)

include_directories(${CMAKE_SOURCE_DIR}/ScopedTimer)

add_executable("FIO" "main.cpp" "goal.c" "goal.h")

if(OpenMP_CXX_FOUND)
    target_link_libraries("FIO" PUBLIC OpenMP::OpenMP_CXX)
endif()