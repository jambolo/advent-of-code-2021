cmake_minimum_required (VERSION 3.14)

set(SOURCES
    day21-1.cpp
    day21-2.cpp
)

add_executable(day21-1 day21-1.cpp)
add_executable(day21-2 day21-2.cpp)

target_link_libraries(day21-1 PRIVATE common)
target_link_libraries(day21-2 PRIVATE common)
