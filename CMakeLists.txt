
cmake_minimum_required(VERSION 3.10)

project(simulator)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "-s USE_SDL=2 -s USE_SDL_TTF=2 --embed-file UbuntuMono-R.ttf")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    include_directories(${PROJECT_SOURCE_DIR}/external/eigen)
else()
    find_package(Eigen3 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif(EMSCRIPTEN)

option (TEST_EKF OFF)

find_package(SDL2 REQUIRED)
find_package(yaml-cpp REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/font
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/eigen
    ${SDL2_INCLUDE_DIRS}
    ${YAML_INCLUDE_DIRS}
)

set(HEADERS
    include/core/common.hpp
    include/core/physics.hpp
    include/core/parser.hpp
    include/core/algorithm.h
    include/sensors/noise.hpp
    include/sensors/ray.hpp
    include/sensors/sensors.hpp
    include/objects/object.h
    include/objects/vehicle.h
    include/objects/simcar.h
    include/objects/obstacle.h
    include/core/engine.h
    external/eigen/Eigen/Dense
)

set(SOURCES
    src/core/algorithm.cpp
    src/objects/vehicle.cpp
    src/objects/simcar.cpp
    src/objects/obstacle.cpp
    src/sensors/imu.hpp
    src/sensors/raydar.hpp
    src/core/engine.cpp
)

if(TEST_EKF)
    add_definitions(-DTEST_EKF)
    add_executable(ekf src/core/algorithm.cpp)

    target_link_libraries( ekf PUBLIC
        Eigen3::Eigen
        ${SDL2_LIBRARIES}
    )
else()
    add_executable(${PROJECT_NAME} sim.cpp ${HEADERS} ${SOURCES} )

    target_link_libraries( ${PROJECT_NAME} PUBLIC
        ${SDL2_LIBRARIES}
        ${YAML_CPP_LIBRARIES}
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
            Eigen3::Eigen
            SDL2_ttf::SDL2_ttf
        )
    endif(EMSCRIPTEN)

endif()




