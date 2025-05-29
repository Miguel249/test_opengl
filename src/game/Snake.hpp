#pragma once
#include <glm.hpp>
#include <vector>

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake {
public:
    Snake(int cols, int rows);

    void update(float deltaTime);

    void setDirection(Direction newDirection);

    void grow();

    void reset();

    // Getters
    [[nodiscard]] const std::vector<glm::vec2> &getBody() const { return body; }
    [[nodiscard]] glm::vec2 getHeadPosition() const { return body.empty() ? glm::vec2(0) : body[0]; }
    [[nodiscard]] Direction getCurrentDirection() const { return currentDirection; }

    // Game logic
    [[nodiscard]] bool checkSelfCollision() const;

    [[nodiscard]] bool isOutOfBounds() const;

    mutable bool directionChanged;
private:
    std::vector<glm::vec2> body; // Posiciones en coordenadas de grid
    Direction currentDirection;
    Direction nextDirection;

    // Movement timing
    float moveTimer;
    static constexpr float MOVE_INTERVAL = 0.2f;

    // Grid properties
    int gridCols;
    int gridRows;

    void move();

    static bool isOppositeDirection(Direction dir1, Direction dir2);

    static glm::vec2 getMovementVector(Direction dir);
};
