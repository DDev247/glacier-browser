
cmake_minimum_required(VERSION 3.0.0)
project(glacier VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_library(glacier src/blaze.cpp src/blaze_html.cpp)
target_compile_definitions(glacier PUBLIC GLACIER_VERSION=\"${PROJECT_VERSION}\")

# add libraries
add_subdirectory(dependencies)
find_package(OpenGL REQUIRED)

target_link_libraries(glacier blaze glfw OpenGL::GL)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
