# CMakeList.txt : CMake project for Platformer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project (Platformer)

set(HEADERS 
    "include/Shader.h"
)
set(SRC 
    "src/Shader.cpp"
    "src/Platformer.cpp"
    "opt/glad/src/glad.c"
)

# Add source to this project's executable.
add_executable (Platformer ${HEADERS} ${SRC})

# include GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("opt/glfw-3.3/")

include_directories("opt/glfw-3.3/include/")

target_link_libraries(Platformer "glfw")

# include GLAD
include_directories("opt/glad/include/")
# include GLM
include_directories("opt/glm/")
# include STBI
include_directories("opt/stbi/")

# include local
include_directories("include/")

# add resource files to binary directory
configure_file("shaders/player.vert" "shaders/player.vert" COPYONLY)
configure_file("shaders/player.frag" "shaders/player.frag" COPYONLY)