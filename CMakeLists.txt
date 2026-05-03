cmake_minimum_required(VERSION 3.14)
project(warcraft)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG v4.5.0
)
FetchContent_MakeAvailable(raylib_cpp)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/engine/logging/Logger.cpp
    src/engine/core/Time.cpp
    src/engine/core/Game.cpp
    src/engine/input/Input.cpp
    src/engine/graphics/Sprite.cpp
    src/engine/graphics/Animation.cpp
    src/engine/graphics/AnimationController.cpp
    src/engine/graphics/Camera2D.cpp
    src/engine/graphics/Tilemap.cpp
    src/engine/graphics/SpriteRenderer.cpp
    src/engine/pathfinding/Pathfinder.cpp
    src/game/WarcraftGame.cpp
    src/game/world/Map.cpp
    src/game/world/World.cpp
    src/game/entities/Unit.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${raylib_SOURCE_DIR}/src
    ${raylib_cpp_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
)
