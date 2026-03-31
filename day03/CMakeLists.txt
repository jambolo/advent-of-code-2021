cmake_minimum_required (VERSION 3.14)

set(SOURCES
    day03.cpp
)

add_executable(day03 ${SOURCES})
target_link_libraries(day03 PRIVATE common nlohmann_json::nlohmann_json)
