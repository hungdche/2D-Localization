
cmake_minimum_required(VERSION 3.10)
project(simulator)

find_package(SDL2 REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
)

set(HEADERS
    include/core/math.hpp
    include/core/physics.hpp
    include/objects/object.h
    include/objects/vehicle.h
    include/graphics/canvas.h
    include/graphics/color.hpp
)

set(SOURCES
    src/objects/vehicle.cpp
    src/graphics/canvas.cpp
)

add_library(SOURCES_LIB STATIC ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} sim.cpp)

target_link_libraries( ${PROJECT_NAME}
    SOURCES_LIB
    ${SDL2_LIBRARIES}
    ${EIGEN3_LIBRARIES}
)
