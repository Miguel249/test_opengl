cmake_minimum_required(VERSION 3.31)
project(test_opengl)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(external/glfw)

include_directories(external/glad/include)
include_directories(external/glfw/include)

file(GLOB src
     "src/*.cpp"
     "external/glad/src/glad.c"
)

add_executable(test_opengl ${src})
target_link_libraries(test_opengl glfw ${CMAKE_DL_LIBS})
