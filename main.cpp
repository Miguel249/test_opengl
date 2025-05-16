#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <renderer/Shader.cpp>
#include <iostream>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <chrono>
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window, float deltaTime);

GLuint loadTexture(const char *path, bool flipVertically = true);

glm::vec2 calculateHalfSize(const float *vertices, unsigned int length, glm::vec3 scalar = { 1.0f, 1.0f, 1.0f });

// settings
constexpr unsigned int SCR_WIDTH{ 800 };
constexpr unsigned int SCR_HEIGHT{ 600 };

glm::vec3 cordsMovement{ 0.0f, 0.0f, 0.0f };
auto lastTime{ std::chrono::high_resolution_clock::now() };

glm::vec2 direction{ 0.0f, 0.0f };
float cellSize{};

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
    GLFWwindow *window{ glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr) };
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const std::string shadersDir = SHADERS_DIR;
    const Shader triangleShader1((shadersDir + "/triangle.vert").c_str(),
                                 (shadersDir + "/color1.frag").c_str());

    constexpr glm::vec3 scalar{ 0.2f, 0.2f, 1.0f };

    constexpr float triangleVertices[] = {
        // Positions     //Texture Coords
        0.5f, -0.5f, 0.0f, 0.8515625f, 0.0f, // V1
        -0.5f, -0.5f, 0.0f, 0.1484375f, 0.0f, // V2
        -0.5f, 0.5f, 0.0f, 0.1484375f, 1.0f, // V3
        0.5f, 0.5f, 0.0f, 0.8515625f, 1.0f // V4
    };

    constexpr int triangleIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    constexpr int gridCols = 9;
    constexpr int gridRows = 9;

    std::vector<glm::vec3> gridOffsets;

    for (int row = 0; row < gridRows; ++row) {
        for (int col = 0; col < gridCols; ++col) {
            float x = (static_cast<float>(col) - static_cast<float>(gridCols / 2)) * scalar.x;
            float y = (static_cast<float>(row) - static_cast<float>(gridRows / 2)) * scalar.y;
            if (col == 8 && row == 8) {
                cellSize = x + y;
            }
            std::cout << "COL: " << col << ", ROW: " << row << std::endl;
            std::cout << x << ", " << y << ", " << x + y << std::endl;
            gridOffsets.emplace_back(x, y, 0.0f);
        }
    }

    unsigned int VAO1{};
    unsigned int VBO1{};
    unsigned int instanceVBO{};
    unsigned int EBO1{};

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &instanceVBO);
    glGenBuffers(1, &EBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, gridOffsets.size() * sizeof(glm::vec3), gridOffsets.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void *>(nullptr));
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    const std::string assetsDir = ASSETS_DIR;
    const unsigned int texture1{ loadTexture((assetsDir + "/textures/snake_head.png").c_str()) };
    const unsigned int cellTexture{ loadTexture((assetsDir + "/textures/snake_cell.png").c_str()) };

    triangleShader1.use();

    triangleShader1.setInt("texture1", 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    const int refreshRate = mode->refreshRate;

    const double targetFrameTime = 1.0 / refreshRate;

    //render loop
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        processInput(window, deltaTime);

        glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        triangleShader1.use();

        // --- DIBUJAR CUADRÍCULA DE FONDO ---
        triangleShader1.setVec3("cordMove", glm::vec3(0.0f)); // sin movimiento
        triangleShader1.setVec3("scalar", scalar); // mismo tamaño

        glBindTexture(GL_TEXTURE_2D, cellTexture);
        glBindVertexArray(VAO1);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, gridCols * gridRows);

        // --- DIBUJAR CABEZA DE LA SERPIENTE ---
        triangleShader1.setVec3("cordMove", cordsMovement); // posición de la cabeza
        triangleShader1.setVec3("scalar", scalar);

        glBindTexture(GL_TEXTURE_2D, texture1); // textura de la cabeza
        glDrawElements(GL_TRIANGLES, sizeof(triangleIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(targetFrameTime);
    }

    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &EBO1);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, const float deltaTime) {
    constexpr float baseSpeed{ 1.0f };
    const float speed{ baseSpeed * deltaTime };
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cordsMovement.y += speed;
        if (cordsMovement.y >= cellSize) {
            cordsMovement.y = cellSize;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cordsMovement.y -= speed;
        if (cordsMovement.y <= 0.0f) {
            cordsMovement.y = 0.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cordsMovement.x -= speed;
        if (cordsMovement.x <= 0.0f) {
            cordsMovement.x = 0.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cordsMovement.x += speed;
        if (cordsMovement.x >= cellSize) {
            cordsMovement.x = cellSize;
        }
    }
}

void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

unsigned int loadTexture(const char *path, const bool flipVertically) {
    unsigned int textureID{};
    glGenTextures(1, &textureID);

    if (flipVertically) {
        stbi_set_flip_vertically_on_load(true);
    }

    int width{}, height{}, nrChannels{};
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        unsigned int format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
            stbi_image_free(data);
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        textureID = 0;
    }

    stbi_image_free(data);
    return textureID;
}

glm::vec2 calculateHalfSize(const float *vertices, const unsigned int length, const glm::vec3 scalar) {
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();

    for (int i = 0; i < length; ++i) {
        const float x = vertices[i * 5]; // (3 pos + 2 UV)
        const float y = vertices[i * 5 + 1];

        if (x < minX) minX = x * scalar.x;
        if (x > maxX) maxX = x * scalar.x;
        if (y < minY) minY = y * scalar.y;
        if (y > maxY) maxY = y * scalar.y;
    }

    const float halfSizeX = (maxX - minX) / 2.0f;
    const float halfSizeY = (maxY - minY) / 2.0f;

    return glm::vec2{ halfSizeX, halfSizeY };
}
