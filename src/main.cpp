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

    const Shader triangleShaderBlue("shader/triangle.vert","shader/color1.frag");
    const Shader triangleShaderYellow("shader/triangle.vert","shader/color2.frag");

    constexpr float triangle1Vertices[] = {
        // Positions                // Colors
        0.0f, -0.25f, 0.0f,       0.5f, 0.1f, 0.8f,
        0.3f, 0.1f, 0.0f,       0.0f, 1.0f, 0.0f,
        0.3f, -0.8f, 0.0f,      0.0f, 0.0f, 1.0f,
        0.7f, -0.8f, 0.0f,    0.5f, 0.1f, 0.8f,
        0.7f, 0.9f, 0.0f,      1.0f, 1.0f, 0.0f,
        0.3f, 0.9f, 0.0f,      0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,
        -0.3f, 0.1f, 0.0f,       0.0f, 1.0f, 0.0f,
        -0.3f, -0.8f, 0.0f,      0.0f, 0.0f, 1.0f,
        -0.7f, -0.8f, 0.0f,    0.5f, 0.1f, 0.8f,
        -0.7f, 0.9f, 0.0f,      1.0f, 1.0f, 0.0f,
        -0.3f, 0.9f, 0.0f,      0.0f, 1.0f, 1.0f,
    };

    constexpr unsigned int triangle1VerticesIndexLeft[]{
        // Lado izquierdo de la M
        0, 6, 7,
        7, 6, 11,
        9, 10, 11,
        8, 9, 11,
    };

    constexpr unsigned int triangle1VerticesIndexRight[]{
        // Lado derecho de la M
        0, 1, 6,
        6, 1, 5,
        5, 2, 3,
        5, 4, 3,
    };

    // Creacion de Vertex Array Object y Vertex Buffer Object para triangulo 1
    unsigned int VAO1[2]{};
    unsigned int VBO1[2]{};
    unsigned int EBO[2]{};
    glGenVertexArrays(2, VAO1);
    glGenBuffers(2, VBO1);
    glGenBuffers(2, EBO);

    // Viculamos el VAO del triangulo 1
    glBindVertexArray(VAO1[0]);

    // Configuramos el VBO del triangulo 1

    // Asignar vertices al VBO1 (llenar el buffer con los datos del triangulo 1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO1[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1Vertices), triangle1Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle1VerticesIndexLeft), triangle1VerticesIndexLeft,
                 GL_STATIC_DRAW);

    // Configurar los atributos del VBO del triangulo 1
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(VAO1[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1Vertices), triangle1Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle1VerticesIndexRight), triangle1VerticesIndexRight,
                 GL_STATIC_DRAW);

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

        glBindVertexArray(VAO1[0]);
        glDrawElements(GL_TRIANGLES, sizeof(triangle1VerticesIndexLeft) / sizeof(unsigned int),GL_UNSIGNED_INT,
                       nullptr);

        triangleShaderBlue.use();

        glBindVertexArray(VAO1[1]);
        glDrawElements(GL_TRIANGLES, sizeof(triangle1VerticesIndexRight) / sizeof(unsigned int),GL_UNSIGNED_INT,
                       nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAO1);
    glDeleteBuffers(2, VBO1);
    glDeleteBuffers(2, EBO);

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
