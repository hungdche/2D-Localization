
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

find_package(SDL2 REQUIRED)
find_package(yaml-cpp REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/font
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/localization
    ${PROJECT_SOURCE_DIR}/include/main_engine
    ${PROJECT_SOURCE_DIR}/include/objects
    ${PROJECT_SOURCE_DIR}/include/sensors
    ${PROJECT_SOURCE_DIR}/include/utils
    ${PROJECT_SOURCE_DIR}/external/eigen
    ${PROJECT_SOURCE_DIR}/external/imgui
    ${SDL2_INCLUDE_DIRS}
    ${YAML_INCLUDE_DIRS}
)

set(SOURCES
    src/objects/obstacle.cpp
    src/objects/simcar.cpp
    src/objects/vehicle.cpp
    src/sensors/imu.cpp
    src/sensors/raydar.cpp
    src/localization/algorithm.cpp
    src/main_engine/engine.cpp
)

set(LIBS
    ${SDL2_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
)

add_executable(${PROJECT_NAME} src/sim.cpp ${SOURCES} )

target_link_libraries( ${PROJECT_NAME} PUBLIC ${LIBS})

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




