cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(2024 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Goodies
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/helpers.cmake)
# Static analysis
find_static_analysis(CLANGTIDY CPPCHECK CPPLINT)

# Executables
add_subdirectory(aoc)
add_subdirectory(day01)
add_subdirectory(day02)
add_subdirectory(day03)
add_subdirectory(day04)
add_subdirectory(day05)
add_subdirectory(day06)
add_subdirectory(day07)
add_subdirectory(day08)
add_subdirectory(day09)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day12)
