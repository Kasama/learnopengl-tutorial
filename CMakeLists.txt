cmake_minimum_required(VERSION 3.6)
project(tutorial)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(third_party/glfw)

set(CMAKE_CXX_STANDARD 14)

# Find and add OpenGL to libraries
find_package(OpenGL REQUIRED)
link_libraries(${OPENGL_LIBRARIES})

# show all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# include third party libs
include_directories(include/
        third_party/gl/include/
        third_party/glfw/include/
        third_party/glm/)

file(GLOB GL_SRCS third_party/gl/include/*.hpp third_party/gl/src/*.cpp)
file(GLOB GLM_SRCS	third_party/glm/glm/*.hpp
        third_party/glm/glm/detail/*.hpp
        third_party/glm/glm/gtc/*.hpp
        third_party/glm/glm/gtx/*.hpp
        third_party/glm/glm/simd/*.hpp)

# Inlclude local libs
file(GLOB INCS
        include/file.hpp
        include/defer.hpp
        include/utils.hpp)

# Include source files
file(GLOB SRCS
        src/main.cpp)

# Include custom shaders
file(GLOB SHADERS
        shaders/basic.frag
        shaders/basic.vert
        )

file(GLOB CONTENT
        )


file(GLOB THIRD_SRCS ${GL_SRCS} ${GLM_SRCS})

source_group("Content" FILES ${CONTENT})
source_group("Source" FILES ${SRCS})
source_group("Header" FILES ${INCS})
source_group("GLSL" FILES ${SHADERS})
source_group("Third\\GL" FILES ${GL_SRCS})
source_group("Third\\GLM" FILES ${GLM_SRCS})

# set main source
set(SOURCE_FILES src/main.cpp)

add_executable(${PROJECT_NAME} ${SRCS} ${INCS} ${SHADERS} ${THIRD_SRCS} ${CONTENT})

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${GL_LIBRARIES})

# Copy non-code stuff to bin directory
add_custom_target(CopyStuff
        COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/content ${CMAKE_CURRENT_BINARY_DIR}/content)

# Run CopyStuff before building project
add_dependencies(${PROJECT_NAME} CopyStuff)
