cmake_minimum_required(VERSION 3.20)

project(main)

find_package(MPI REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE
  MPI_C)
