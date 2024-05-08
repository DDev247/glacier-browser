
cmake_minimum_required(VERSION 3.8.0)

add_library(glad-debug "debug/src/glad.c" "debug/include/glad/glad.h" "debug/include/KHR/khrplatform.h")
add_library(glad "release/src/glad.c" "release/include/glad/glad.h" "release/include/KHR/khrplatform.h")

target_include_directories(glad-debug PRIVATE "debug/include/")
target_include_directories(glad PRIVATE "release/include/")
