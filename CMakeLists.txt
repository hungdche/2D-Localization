
cmake_minimum_required(VERSION 3.10)
project(simulator)

find_package(SDL2 2.0.22 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

set(HEADERS
    include/core/common.hpp
    include/core/physics.hpp
    include/sensors/ray.hpp
    include/objects/object.h
    include/objects/vehicle.h
    include/objects/obstacle.h
    include/core/engine.h
)

set(SOURCES
    src/objects/vehicle.cpp
    src/objects/obstacle.cpp
    src/core/engine.cpp
)

add_library(SOURCES_LIB SHARED ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} sim.cpp)

target_link_libraries( ${PROJECT_NAME}
    SOURCES_LIB
    ${SDL2_LIBRARIES}
)
