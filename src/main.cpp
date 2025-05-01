#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

// settings
constexpr unsigned int SCR_WIDTH  = 800;
constexpr unsigned int SCR_HEIGHT = 600;

auto vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

auto fragmentShaderSourceBlue = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
        "}\n\0";

auto fragmentShaderSourceYellow = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\n\0";

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

    // Creacion de Shader vertex
    unsigned int vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    // Asignar source del shader vertex
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    // Compilar el shader vertex
    glCompileShader(vertexShader);

    // Validacion de compilacion del shader vertex
    int success{};
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Creacion de Fragment Shader Blue
    unsigned int fragmentShaderBlue{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShaderBlue, 1, &fragmentShaderSourceBlue, nullptr);
    glCompileShader(fragmentShaderBlue);

    glGetShaderiv(fragmentShaderBlue, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderBlue, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::BLUE::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Creacion de Fragment Shader Yellow
    unsigned int fragmentShaderYellow{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, nullptr);
    glCompileShader(fragmentShaderYellow);

    glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderYellow, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::YELLOW::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgramBlue{glCreateProgram()};
    glAttachShader(shaderProgramBlue, vertexShader);
    glAttachShader(shaderProgramBlue, fragmentShaderBlue);
    glLinkProgram(shaderProgramBlue);
    glGetProgramiv(shaderProgramBlue, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramBlue, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM:BLUE::LINKING_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgramYellow{glCreateProgram()};
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);
    glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramYellow, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::YELLOW::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderBlue);
    glDeleteShader(fragmentShaderYellow);

    float triangle1Vertices[] = {
        0.0f, 0.0f, 0.0f,
        -0.6f, 0.0f, 0.0f,
        -0.3f, 0.6f, 0.0f,
    };

    float triangle2Vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.6f, 0.0f, 0.0f,
        0.3f, 0.6f, 0.0f,
    };

    // Creacion de Vertex Array Object y Vertex Buffer Object para triangulo 1
    unsigned int VAO1[2]{};
    unsigned int VBO1[2]{};
    glGenVertexArrays(2, VAO1);
    glGenBuffers(2, VBO1);

    // Viculamos el VAO del triangulo 1
    glBindVertexArray(VAO1[0]);

    // Configuramos el VBO del triangulo 1

    // Asignar vertices al VBO1 (llenar el buffer con los datos del triangulo 1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO1[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1Vertices), triangle1Vertices, GL_STATIC_DRAW);

    // Configurar los atributos del VBO del triangulo 1
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    // Viculamos el VAO del triangulo 2
    glBindVertexArray(VAO1[1]);

    // Configuramos el VBO del triangulo 2

    // Asignar vertices al VBO1 (llenar el buffer con los datos del triangulo 2)
    glBindBuffer(GL_ARRAY_BUFFER, VBO1[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2Vertices), triangle2Vertices, GL_STATIC_DRAW);

    // Configurar los atributos del VBO del triangulo 2
    // NOTA: Cuando la separacion entre atributos es 0, es decir estan empaquetados, el stride puede ser 0
    // Y dejar que openGL lo interprete
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.63f, 0.50f, 0.50f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramBlue);
        glBindVertexArray(VAO1[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAO1[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAO1);
    glDeleteBuffers(2, VBO1);
    glDeleteProgram(shaderProgramBlue);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
