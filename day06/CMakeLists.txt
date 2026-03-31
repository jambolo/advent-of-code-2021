cmake_minimum_required (VERSION 3.14)

set(SOURCES
    day06.cpp
)

add_executable(day06 ${SOURCES})
target_link_libraries(day06 PRIVATE common nlohmann_json::nlohmann_json)
