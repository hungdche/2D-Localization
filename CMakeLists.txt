
cmake_minimum_required(VERSION 3.10)

project(simulator)

option(USE_EMSDK "Using Emscripten" OFF)

if(USE_EMSDK)
    set(CMAKE_CXX_FLAGS "-s USE_SDL=2")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    add_definitions(-DUSE_EMSDK)
endif(USE_EMSDK)

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
    include/core/common.hpp
    include/core/physics.hpp
    include/sensors/noise.hpp
    include/sensors/ray.hpp
    include/sensors/sensors.h
    include/objects/object.h
    include/objects/vehicle.h
    include/objects/simcar.h
    include/objects/obstacle.h
    include/core/engine.h
)

set(SOURCES
    src/sensors/sensors.cpp
    src/objects/vehicle.cpp
    src/objects/simcar.cpp
    src/objects/obstacle.cpp
    src/core/engine.cpp
)

add_library(SOURCES_LIB SHARED ${HEADERS} ${SOURCES})

add_executable(${PROJECT_NAME} sim.cpp)

target_link_libraries( ${PROJECT_NAME}
    SOURCES_LIB
    ${SDL2_LIBRARIES}
    ${EIGEN3_LIBRARIES}
)

if(USE_EMSDK)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS 
        "-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] --bind" 
        "-s MALLOC=emmalloc" 
        "-s WASM_BIGINT=1" 
        "-s USE_SDL=2" 
        "-s ALLOW_MEMORY_GROWTH=1")
endif(USE_EMSDK)


