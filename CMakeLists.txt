cmake_minimum_required(VERSION 2.6)
project(HPC)

# 好耶，CMake 配置好了！

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER mpiicpc)
set(CMAKE_C_COMPILER mpicc)

set(CMAKE_CXX_FLAGS "-Wall -fopenmp")
set(CMAKE_C_FLAGS "-Wall -fopenmp")

file(GLOB APP_CPP_SOURCES ./*.cpp)
foreach (CPP_SOURCE_FILE ${APP_CPP_SOURCES})
    file(RELATIVE_PATH SOURCE_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR} ${CPP_SOURCE_FILE})
    string(REPLACE ".cpp" "" OUTPUT_NAME ${SOURCE_FILE_NAME})
    message("Adding ${SOURCE_FILE_NAME}")
    add_executable(${OUTPUT_NAME} ${CPP_SOURCE_FILE})
endforeach (CPP_SOURCE_FILE ${APP_CPP_SOURCES})

file(GLOB APP_SOURCES ./*.c)
foreach (C_SOURCE_FILE ${APP_SOURCES})
    file(RELATIVE_PATH SOURCE_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR} ${C_SOURCE_FILE})
    string(REPLACE ".c" "" OUTPUT_NAME ${SOURCE_FILE_NAME})
    message("Adding ${SOURCE_FILE_NAME}")
    add_executable(${OUTPUT_NAME} ${C_SOURCE_FILE})
endforeach (C_SOURCE_FILE ${APP_SOURCES})