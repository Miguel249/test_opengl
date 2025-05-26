cmake_minimum_required(VERSION 3.31)
project(test_opengl VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar paquetes
find_package(OpenGL REQUIRED)

# Agregar subdirectorios de librerías externas
add_subdirectory(external/glfw)
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Definir ejecutable
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Incluir directorios
target_include_directories(${PROJECT_NAME} PRIVATE
        src/
        external/glad/include
        external/glfw/include
        external/glm
        external/stb
)

# Linkear librerías
target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        glfw
        glad
)

# Copiar recursos al directorio de build
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})


target_compile_definitions(test_opengl PRIVATE
        SOURCE_DIR="${CMAKE_SOURCE_DIR}/src"
        RESOURCES_DIR="${CMAKE_SOURCE_DIR}/resources"
)
