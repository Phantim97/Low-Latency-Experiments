cmake_minimum_required(VERSION 3.8)

project("Low Latency" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("$ENV{LIBS_PATH}/benchmarking/google benchmark/include")

find_package(OpenMP)

#find_package(benchmark REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/ScopedTimer)

add_executable("FIO" "main.cpp" "goal.c" "goal.h" "numeric_types.h")

if(OpenMP_CXX_FOUND)
    target_link_libraries("FIO" PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries("FIO" PUBLIC "Shlwapi.lib")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_link_libraries("FIO" PUBLIC "$ENV{LIBS_PATH}/benchmarking/google benchmark/build/src/Release/benchmark.lib")
    target_link_libraries("FIO" PUBLIC "$ENV{LIBS_PATH}/benchmarking/google benchmark/build/src/Release/benchmark_main.lib")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_link_libraries("FIO" PUBLIC "$ENV{LIBS_PATH}/benchmarking/google benchmark/build/src/Debug/benchmark.lib")
    target_link_libraries("FIO" PUBLIC "$ENV{LIBS_PATH}/benchmarking/google benchmark/build/src/Debug/benchmark_main.lib")
endif()