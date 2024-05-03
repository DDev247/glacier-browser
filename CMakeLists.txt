
cmake_minimum_required(VERSION 3.8.0)
project(glacier VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

# add dependencies
add_subdirectory(dependencies)
find_package(OpenGL REQUIRED)
include_directories("C:\\VulkanSDK\\1.3.280.0\\Include")

# add executables
add_executable(glacier-vk "src/vk/main.cpp" "include/vk/application.h" "src/vk/application.cpp")
target_compile_definitions(glacier-vk PUBLIC GLACIER_VERSION=\"${PROJECT_VERSION}\")

add_executable(glacier "src/main.cpp" "include/application.h" "src/application.cpp" "src/display_element.cpp" "include/display_element.h" "include/logger.h" "src/logger.cpp"  "include/colors.h" "src/colors.cpp" "src/navigation_element.cpp" "include/navigation_element.h")
target_compile_definitions(glacier PUBLIC GLACIER_VERSION=\"${PROJECT_VERSION}\")

# add link libraries
target_link_libraries(glacier-vk "C:\\VulkanSDK\\1.3.280.0\\Lib\\vulkan-1.lib")
target_link_libraries(glacier-vk glfw glm)

target_link_libraries(glacier glfw glm OpenGL::GL)
target_include_directories(glacier PRIVATE "dependencies/glfw/include" PRIVATE "dependencies/glm/")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
