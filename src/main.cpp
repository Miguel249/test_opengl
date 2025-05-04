#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <builders/shader.h>
#include <iostream>
#include <glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <builders/stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

GLuint loadTexture(const char *path, bool flipVertically = true);

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
    GLFWwindow *window{ glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr) };
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

    const std::string projectDir = PROJECT_DIR;
    const Shader triangleShaderYellow((projectDir + "/shader/triangle.vert").c_str(),
                                      (projectDir + "/shader/color1.frag").c_str());

    constexpr float triangleVertices[] = {
        // Positions         // Colors      //Texture Coords
        0.175f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // V1
        -0.175f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // V2
        -0.175f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // V3
        0.175f, 0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f    // V4
    };

    constexpr int triangleIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int VAO1{ };
    unsigned int VBO1{ };
    unsigned int EBO1{ };\

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const unsigned int texture1{ loadTexture((projectDir + "/textures/container.jpg").c_str(), false) };
    const unsigned int texture2{ loadTexture((projectDir + "/textures/awesomeface.png").c_str()) };

    triangleShaderYellow.use();

    triangleShaderYellow.setInt("texture1", 0);

    triangleShaderYellow.setInt("texture2", 1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float offsetX{ 0.0f };
    float offsetY{ 0.0f };
    float lastFrame{ };

    glm::vec2 direction{ glm::normalize(glm::vec2(1.0f, 0.5f)) };

    constexpr float recHalfWidth{ 0.35f / 2.0f };
    constexpr float recHalfHeight{ 0.2f / 2.0f };

    //render loop
    while (!glfwWindowShouldClose(window)) {
        constexpr float speed{ 0.5f };
        const auto currentFrame = static_cast<float>(glfwGetTime());
        const float deltaTime{ std::min(currentFrame - lastFrame, 0.016f) };
        lastFrame = currentFrame;

        processInput(window);

        offsetX += direction.x * speed * deltaTime;
        offsetY += direction.y * speed * deltaTime;

        if (offsetX + recHalfWidth > 1.0f) {
            offsetX = 1.0f - recHalfWidth;
            direction.x *= -1.0f;
        } else if (offsetX - recHalfWidth < -1.0f) {
            offsetX = -1.0f + recHalfWidth;
            direction.x *= -1.0f;
        }

        if (offsetY + recHalfHeight > 1.0f) {
            offsetY = 1.0f - recHalfHeight;
            direction.y *= -1.0f;
        } else if (offsetY - recHalfHeight < -1.0f) {
            offsetY = -1.0f + recHalfHeight;
            direction.y *= -1.0f;
        }

        glClearColor(0.63f, 0.50f, 0.50f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        triangleShaderYellow.use();
        triangleShaderYellow.setFloat("horizontalOffset", offsetX);
        triangleShaderYellow.setFloat("verticalOffset", offsetY);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, sizeof(triangleIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);

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

unsigned int loadTexture(const char *path, const bool flipVertically) {
    unsigned int textureID{ };
    glGenTextures(1, &textureID);

    if (flipVertically) {
        stbi_set_flip_vertically_on_load(true);
    }

    int width, height, nrChannels;
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
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        textureID = 0;
    }

    stbi_image_free(data);
    return textureID;
}
