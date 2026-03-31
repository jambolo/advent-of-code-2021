cmake_minimum_required (VERSION 3.14)

set(SOURCES
    day01.cpp
)

add_executable(day01 ${SOURCES})
target_link_libraries(day01 PRIVATE common CLI11::CLI11)
