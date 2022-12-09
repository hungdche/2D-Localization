
cmake_minimum_required(VERSION 3.10)

project(simulator)

set (CMAKE_CXX_STANDARD 17)
if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "-s USE_SDL=2 -s USE_SDL_TTF=2 --embed-file UbuntuMono-R.ttf")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    add_subdirectory(external/eigen)
else()
    find_package(Eigen3 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif(EMSCRIPTEN)

find_package(SDL2 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/font
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

set(HEADERS
    include/core/common.hpp
    include/core/physics.hpp
    include/core/algorithm.h
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
    src/core/algorithm.cpp
    src/sensors/sensors.cpp
    src/objects/vehicle.cpp
    src/objects/simcar.cpp
    src/objects/obstacle.cpp
    src/core/engine.cpp
)

add_executable(${PROJECT_NAME} sim.cpp ${HEADERS} ${SOURCES} )

target_link_libraries( ${PROJECT_NAME} PUBLIC
    Eigen3::Eigen
    ${SDL2_LIBRARIES}
)

if(EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS 
        "-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] --bind" 
        "-s ABORTING_MALLOC=0"
        "-s WASM_BIGINT=1" 
        "-s USE_SDL=2 -s USE_SDL_TTF=2" 
        # "--embed-file SFUIText-Regular.ttf"
        "-s ALLOW_MEMORY_GROWTH"
    )
else()
    target_link_libraries( ${PROJECT_NAME} PUBLIC
        SDL2_ttf::SDL2_ttf
    )
endif(EMSCRIPTEN)


