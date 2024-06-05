cmake_minimum_required(VERSION 3.10)

project(wsl-sdl2-opengl-template VERSION 1.0)

find_package(GLEW REQUIRED)

add_executable(wsl-sdl2-opengl-template main.c)

target_link_libraries(wsl-sdl2-opengl-template PRIVATE GL GLEW SDL2 SDL2main)
