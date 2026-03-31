cmake_minimum_required (VERSION 3.14)

set(SOURCES
    expect.h
    expect.cpp
    load.h
    load.cpp
    PathFinder.h
    PathFinder.cpp
    setup.h
    setup.cpp
)

set(INTERFACE_INCLUDE_PATHS
    ${PROJECT_SOURCE_DIR}
)

add_library(common ${SOURCES})
target_include_directories(common INTERFACE ${INTERFACE_INCLUDE_PATHS})
target_link_libraries(common PRIVATE CLI11::CLI11)
