#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <builders/shader.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

// settings
constexpr unsigned int SCR_WIDTH  = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW3" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window{glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr)};
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    const Shader triangleShaderYellow("shader/triangle.vert","shader/color1.frag");

    constexpr float triangleVertices[] = {
        // Positions                // Colors
        0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  // V1
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // V2
        0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f   // V3
    };

    // Creacion de Vertex Array Object y Vertex Buffer Object para triangulo 1
    unsigned int VAO1{};
    unsigned int VBO1{};
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    // Viculamos el VAO del triangulo 1
    glBindVertexArray(VAO1);

    // Configuramos el VBO del triangulo 1

    // Asignar vertices al VBO1 (llenar el buffer con los datos del triangulo 1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    // Configurar los atributos del VBO del triangulo 1
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.63f, 0.50f, 0.50f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        triangleShaderYellow.use();

        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
}
