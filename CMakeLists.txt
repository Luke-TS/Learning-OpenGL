cmake_minimum_required(VERSION 3.16)
project(gl_example C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

find_package(glfw3 CONFIG REQUIRED)
find_package(glm   CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

add_library(glad glad/src/gl.c)
target_include_directories(glad PUBLIC glad/include PRIVATE assimp::assimp)

add_executable(gl_example
    src/main.cpp
    src/shader.cpp
    src/texture.cpp
)

target_link_libraries(gl_example
    PRIVATE
        glad
        glfw
        OpenGL::GL
        glm::glm
)

set(CMAKE_BUILD_TYPE Debug)

target_compile_definitions(gl_example PRIVATE
    TEXTURES="${PROJECT_SOURCE_DIR}/assets/textures"
    SHADERS="${PROJECT_SOURCE_DIR}/shaders"
)
