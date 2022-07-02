project(ACmgr)
cmake_minimum_required(VERSION 3.0)

add_subdirectory(wt)
include_directories(wt/src)

add_executable(ACmgr main.cpp)
target_link_libraries(ACmgr wt wthttp)
