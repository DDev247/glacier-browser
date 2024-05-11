
cmake_minimum_required(VERSION 3.8.0)
project(glacier VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE "GLVND")

include(CTest)
enable_testing()

# ---------------------------- Add dependencies ----------------------------

add_subdirectory(dependencies)
find_package(OpenGL REQUIRED)
include_directories("C:\\VulkanSDK\\1.3.280.0\\Include")

# ------------------------------- MinGW stuff ------------------------------

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(BUILD_FOR_WIN TRUE)
endif()
option(STATIC_BUILD "Build a static binary." ${BUILD_FOR_WIN})

if (STATIC_BUILD)
        set(CMAKE_EXE_LINKER_FLAGS "-static")
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" CONFIG)
        set(BUILD_SHARED_LIBS OFF)
endif()

# ----------------------------- Add executables ----------------------------

add_executable(glacier 
									"src/main.cpp"
	"include/application.h" 		"src/application.cpp"
	"include/logger.h" 				"src/logger.cpp"
	"include/colors.h" 				"src/colors.cpp"
	"include/text.h"				"src/text.cpp"
	"include/shader.h"				"src/shader.cpp"
	"include/stb_image.h"			"src/stb_image.cpp"
	"include/texture.h"				"src/texture.cpp"

	"include/display_element.h"		"src/display_element.cpp"
	"include/navigation_element.h"	"src/navigation_element.cpp"
	"include/colortest_element.h"	"src/colortest_element.cpp"
	"include/button_element.h"		"src/button_element.cpp"
	"include/input_element.h"		"src/input_element.cpp"
	
)
target_compile_definitions(glacier PUBLIC GLACIER_VERSION=\"${PROJECT_VERSION}\")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("-- Compiling as Debug")
	target_compile_definitions(glacier PUBLIC GLACIER_DEBUG)
endif()

target_link_libraries(glacier glfw glm glad freetype)
target_include_directories(glacier
	PRIVATE "dependencies/glfw/include"
	PRIVATE "dependencies/glm/"
	PRIVATE "dependencies/freetype/include"
	PRIVATE "dependencies/glad/release/include"
)


if (STATIC_BUILD)
	target_link_libraries(glacier "libssp.a" "libz")
endif()

# TODO: Make a better system, file(COPY) only works on configure time, not on build time

file(COPY "${PROJECT_SOURCE_DIR}/shaders/" DESTINATION "${CMAKE_BINARY_DIR}/shaders")
file(COPY "${PROJECT_SOURCE_DIR}/fonts/" DESTINATION "${CMAKE_BINARY_DIR}/fonts")
file(COPY "${PROJECT_SOURCE_DIR}/textures/" DESTINATION "${CMAKE_BINARY_DIR}/textures")

#add_custom_command(TARGET glacier POST_BUILD
    #COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/shaders/" "$<TARGET_FILE_DIR:glacier>/shaders"
    #COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/fonts/" "$<TARGET_FILE_DIR:glacier>/fonts"
    #COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/textures/" "$<TARGET_FILE_DIR:glacier>/textures"
#)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
