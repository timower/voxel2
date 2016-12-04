cmake_minimum_required(VERSION 3.3)
project(unnamed)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC -std=c++11 -Wall -Werror -ggdb")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(deps/glfw)
include_directories(deps/glfw/include)

find_package(gl3w REQUIRED)
include_directories(deps/gl3w/include)

link_directories(deps/glew/lib)
include_directories(deps/glew/include)

link_directories(deps/SOIL/lib/)
include_directories(deps/SOIL/src)

include_directories(deps/glm)

set(EXECUTABLE_FILES src/unity.cpp )
add_executable(main ${EXECUTABLE_FILES})
target_link_libraries(main gl3w glfw GL libSOIL.a)
