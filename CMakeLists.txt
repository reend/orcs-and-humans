cmake_minimum_required(VERSION 3.14)
project(warcraft)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib_cpp)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/engine/logging/Logger.cpp
    src/engine/core/Time.cpp
    src/engine/input/Input.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${raylib_SOURCE_DIR}/src
    ${raylib_cpp_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
)
