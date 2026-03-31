cmake_minimum_required (VERSION 3.14)

set(SOURCES
    day02.cpp
)

add_executable(day02 ${SOURCES})
target_link_libraries(day02 PRIVATE common nlohmann_json::nlohmann_json)
