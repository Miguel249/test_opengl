#include "Renderer.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Renderer::Renderer() : snakeHeadTexture(0), snakeBodyTexture(0), snakeBodyTurnTexture(0), snakeTailTexture(0),
                       cellTexture(0) {
}

Renderer::~Renderer() {
    if (snakeHeadTexture)
        glDeleteTextures(1, &snakeHeadTexture);
    if (snakeBodyTexture)
        glDeleteTextures(1, &snakeBodyTexture);
    if (snakeBodyTurnTexture)
        glDeleteTextures(1, &snakeBodyTurnTexture);
    if (snakeTailTexture)
        glDeleteTextures(1, &snakeTailTexture);
    if (cellTexture)
        glDeleteTextures(1, &cellTexture);
}

bool Renderer::initialize(const std::string &resourcesDir, const Grid &grid) {
    // Initialize shader
    shader = std::make_unique<Shader>(
        (resourcesDir + "/shaders/triangle.vert").c_str(),
        (resourcesDir + "/shaders/color1.frag").c_str()
    );

    // Load textures
    snakeHeadTexture = loadTexture(resourcesDir + "/textures/snake_head.png");
    snakeBodyTexture = loadTexture(resourcesDir + "/textures/snake_body.png");
    snakeBodyTurnTexture = loadTexture(resourcesDir + "/textures/snake_body_turn.png");
    snakeTailTexture = loadTexture(resourcesDir + "/textures/snake_tail.png");
    cellTexture = loadTexture(resourcesDir + "/textures/snake_cell.png");

    if (!snakeHeadTexture || !cellTexture || !snakeBodyTexture || !snakeBodyTurnTexture) {
        std::cerr << "Failed to load textures" << std::endl;
        return false;
    }

    // Setup mesh
    setupMesh(grid);

    setupSnakeMesh();

    // Configure shader
    shader->use();
    shader->setInt("texture1", 0);

    return true;
}

void Renderer::setupMesh(const Grid &grid) {
    const std::vector<VertexAttribute> attributes{
        { 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0, 0 },
        { 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 3 * sizeof(float), 0 }
    };

    const std::vector<VertexAttribute> instanceAttributes{
        { 2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0, 1 },
    };

    mesh = std::make_unique<Mesh>(
        quadVertices,
        sizeof(quadVertices),
        attributes,
        quadIndices,
        sizeof(quadIndices),
        grid.getOffsets().data(),
        static_cast<GLsizeiptr>(grid.getOffsets().size() * sizeof(glm::vec3)),
        instanceAttributes
    );
}

void Renderer::setupSnakeMesh() {
    const std::vector<VertexAttribute> attributes{
        { 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0, 0 },
        { 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 3 * sizeof(float), 0 }
    };

    snakeMesh = std::make_unique<Mesh>(
        quadVertices,
        sizeof(quadVertices),
        attributes,
        quadIndices,
        sizeof(quadIndices)
    );
}

void Renderer::clear() {
    glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::renderGrid(const Grid &grid) const {
    shader->use();
    shader->setVec3("cordMove", glm::vec3(0.0f));
    shader->setVec3("scalar", grid.getCellScale());
    shader->setBool("isHead", false);

    glBindTexture(GL_TEXTURE_2D, cellTexture);

    mesh->bind();
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr,
                            grid.getCols() * grid.getRows());
}

void Renderer::renderSnake(const Snake &snake, const Grid &grid) {
    shader->use();
    shader->setVec3("scalar", grid.getCellScale());

    const auto &body = snake.getBody();

    // Detectar nuevo giro y guardarlo
    const Direction headCurrentDir = snake.getCurrentDirection();
    // std::cout << "Head current dir: " << static_cast<int>(headCurrentDir) << std::endl;
    // std::cout << "Head last dir: " << static_cast<int>(headLastDir) << std::endl;
    if (headCurrentDir != Direction::NONE) {
        if (headLastDir != headCurrentDir) {
            // Guardar la posición donde ocurrió el giro
            // std::cout << "Entro" << std::endl;
            turnInfo.position = snake.getHeadPosition();
            turnInfo.fromDir = headLastDir;
            turnInfo.toDir = headCurrentDir;
            turnPositionsQueue.push(turnInfo);

            headLastDir = headCurrentDir;
        }
    }else {
        headLastDir = Direction::NONE;
    }

    for (size_t i = 0; i < body.size(); ++i) {
        glm::vec3 worldPos = grid.gridToWorldPosition(body[i]);
        const glm::vec2 bodyPos = body[i];
        shader->setVec3("cordMove", worldPos);

        const bool isHead = (i == 0);
        bool isTurn = false;

        // Para segmentos del cuerpo (no la cabeza)
        if (!isHead && i < body.size() - 1 && !turnPositionsQueue.empty()) {
            // std::queue<TurnInfo> tempQueue = turnPositionsQueue;
            const TurnInfo &turnInfo = turnPositionsQueue.front();
            if (bodyPos == turnInfo.position ) {
                isTurn = true;
                // break;
            }
            // while (!turnPositionsQueue.empty()) {
            //     // tempQueue.pop();
            // }
        } else if (i == body.size() - 1 && !turnPositionsQueue.empty()) {
            while (!turnPositionsQueue.empty()) {
                const TurnInfo &frontTurn = turnPositionsQueue.front();
                if (body.back() == frontTurn.position) {
                    std::cout << "Borro" << std::endl;
                    turnPositionsQueue.pop();
                } else {
                    break;
                }
            }
        }

        // Limpiar giros que ya pasaron la cola
        // while (!turnPositionsQueue.empty()) {
        //     const TurnInfo &frontTurn = turnPositionsQueue.front();
        //     const glm::vec2 &tailPos = body.back();
        //
        //     if (tailPos.x == frontTurn.position.x && tailPos.y == frontTurn.position.y) {
        //         turnPositionsQueue.pop();
        //     }else {
        //         break;
        //     }
        // }

        GLuint texture{};
        if (isHead) {
            shader->setBool("rotated", true);
            texture = snakeHeadTexture;
        } else if (body.size() - 1 == i) {
            shader->setBool("rotated", isTurn);
            texture = snakeTailTexture;
        } else {
            shader->setBool("rotated", isTurn);
            texture = snakeBodyTexture;
        }

        glBindTexture(GL_TEXTURE_2D, texture);

        // Set direction for head rotation if needed
        if (isHead) {
            const Direction dir = snake.getCurrentDirection();
            glm::vec2 dirVec(0.0f);
            switch (dir) {
                case Direction::UP:
                    dirVec = glm::vec2(0, 1);
                    break;
                case Direction::DOWN:
                    dirVec = glm::vec2(0, -1);
                    break;
                case Direction::LEFT:
                    dirVec = glm::vec2(-1, 0);
                    break;
                case Direction::RIGHT:
                    dirVec = glm::vec2(1, 0);
                    break;
                default:
                    break;
            }
            shader->setVec2("direction", dirVec);
        }

        snakeMesh->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void Renderer::setViewport(const int width, const int height) {
    glViewport(0, 0, width, height);
}

GLuint Renderer::loadTexture(const std::string &path, const bool flipVertically) {
    GLuint textureID = 0;
    glGenTextures(1, &textureID);

    if (flipVertically) {
        stbi_set_flip_vertically_on_load(true);
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format;
        switch (nrChannels) {
            case 1: format = GL_RED;
                break;
            case 3: format = GL_RGB;
                break;
            case 4: format = GL_RGBA;
                break;
            default:
                std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
                stbi_image_free(data);
                return 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }

    stbi_image_free(data);
    return textureID;
}
