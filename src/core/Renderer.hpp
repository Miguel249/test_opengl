#pragma once
#include <core/Mesh.hpp>
#include <core/Shader.hpp>
#include <memory>
#include <queue>

#include "game/Food.hpp"
#include "game/Grid.hpp"
#include "game/Snake.hpp"

class Renderer {
public:
    Renderer();

    ~Renderer();

    bool initialize(const std::string &resourcesDir, const Grid &grid);

    void renderGrid(const Grid &grid) const;

    void renderSnake(const Snake &snake, const Grid &grid) const;

    void renderFood(const Food &food, const Grid &grid) const;

    static void clear();

    static void setViewport(int width, int height);

private:
    struct TurnInfo {
        glm::vec2 position;
        Direction fromDir;
        Direction toDir;
    };

    std::unique_ptr<Shader> shader;
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<Mesh> snakeMesh;
    std::unique_ptr<Mesh> foodMesh;

    TurnInfo turnInfo{};
    std::queue<TurnInfo> turnPositionsQueue{};
    Direction headLastDir{Direction::NONE};

    GLuint snakeHeadTexture;
    GLuint snakeBodyTexture;
    GLuint snakeBodyTurnTexture;
    GLuint snakeTailTexture;
    GLuint foodTexture;
    GLuint cellTexture;

    // Quad data
    static constexpr float quadVertices[] = {
        // Positions     //Texture Coords
        0.5f, -0.5f, 0.0f, 0.8515625f, 0.0f, // V1
        -0.5f, -0.5f, 0.0f, 0.1484375f, 0.0f, // V2
        -0.5f, 0.5f, 0.0f, 0.1484375f, 1.0f, // V3
        0.5f, 0.5f, 0.0f, 0.8515625f, 1.0f // V4
    };

    static constexpr GLuint quadIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    static GLuint loadTexture(const std::string &path, bool flipVertically = true);

    void setupMesh(const Grid &grid);

    void setupSnakeMesh();

    void setupFoodMesh();
};
