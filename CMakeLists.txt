cmake_minimum_required(VERSION 2.6)
project(HPC)

# 好耶，CMake 配置好了！

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_C_COMPILER mpicc)

file(GLOB APP_CPP_SOURCES ./*.cpp)
foreach (cppsourcefile ${APP_CPP_SOURCES})
    file(RELATIVE_PATH filename ${CMAKE_CURRENT_SOURCE_DIR} ${cppsourcefile})
    string(REPLACE ".cpp" "" oname ${filename})
    message("Adding ${filename}")
    add_executable(${oname} ${cppsourcefile})
endforeach (cppsourcefile ${APP_CPP_SOURCES})

file(GLOB APP_SOURCES ./*.c)
foreach (csourcefile ${APP_SOURCES})
    file(RELATIVE_PATH filename ${CMAKE_CURRENT_SOURCE_DIR} ${csourcefile})
    string(REPLACE ".c" "" oname ${filename})
    message("Adding ${filename}")
    add_executable(${oname} ${csourcefile})
endforeach (csourcefile ${APP_SOURCES})