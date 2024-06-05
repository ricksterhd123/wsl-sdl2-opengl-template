cmake_minimum_required(VERSION 3.10)

project(mdyn VERSION 1.0)

find_package(GLEW REQUIRED)

add_executable(mdyn main.c)

target_link_libraries(mdyn PRIVATE GL GLEW SDL2 SDL2main)
