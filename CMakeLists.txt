cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=gnu++17 -fdiagnostics-color=auto -Wall")

add_subdirectory(src)

set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}")
