cmake_minimum_required(VERSION 3.23)

# Set compile commands export and C standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define project
project(learn_2d_game_physics_programming)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/x64")

# Define source files
set(src_files
    src/Physics/Vec2.h
    src/Physics/Vec2.cpp
    src/Graphics.h
    src/Graphics.cpp
    src/Application.h
    src/Application.cpp
    src/Main.cpp
)

# Add executable
add_executable(learn_2d_game_physics_programming ${src_files})

# Find and link SDL2
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)
target_link_libraries(
    learn_2d_game_physics_programming
    PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2::SDL2_gfx
)
