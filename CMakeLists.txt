cmake_minimum_required(VERSION 3.31)
project(test_opengl)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(external/glfw)

include_directories(
        external/glad/include
        external/glfw/include
        external/glm
        external/stb
        include
        src
)

set(SOURCES
        "main.cpp"
        "external/glad/src/glad.c"
        "src/renderer/Shader.cpp"
        "src/renderer/Mesh.cpp"
)

add_executable(test_opengl ${SOURCES})
target_link_libraries(test_opengl glfw ${CMAKE_DL_LIBS})

target_compile_definitions(test_opengl PRIVATE
        SOURCE_DIR="${CMAKE_SOURCE_DIR}/src"
        ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets"
        SHADERS_DIR="${CMAKE_SOURCE_DIR}/shaders"
)
