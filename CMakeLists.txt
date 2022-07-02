project(ACmgr)
cmake_minimum_required(VERSION 3.0)

# disable embedded wt shared library build
SET(SHARED_LIBS OFF CACHE BOOL "Disable shared libaries for wt")
SET(BUILD_EXAMPLES OFF CACHE BOOL "Disable building examples for wt")
SET(BUILD_TESTS OFF CACHE BOOL "Disable building tests for wt")

add_subdirectory(wt)

include_directories(${CMAKE_BINARY_DIR}/wt)
include_directories(wt/src)

# Boost
find_package( Boost 1.71 COMPONENTS date_time REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

# libjsoncpp
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

add_executable(ACmgr
        src/main.cpp
        src/cardentry.cpp src/cardentry.h
        src/mainview.cpp src/mainview.h
        )
target_link_libraries(ACmgr wt wthttp  ${Boost_LIBRARIES} ${JSONCPP_LIBRARIES})
