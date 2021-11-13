cmake_minimum_required(VERSION 2.6)
project(HPC)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_C_COMPILER mpicc)

add_executable(main main.cpp)
add_executable(first-mpi-program first-mpi-program.cpp)
add_executable(simple-send-and-recv simple-send-and-recv.cpp)