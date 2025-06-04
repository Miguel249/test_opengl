#pragma once
#include <glm.hpp>
#include <queue>
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
    struct TurnInfo {
        glm::vec2 position;
        Direction fromDir;
        Direction toDir;
    };

    Direction lastDirection = Direction::NONE;

    std::queue<TurnInfo> turnPositionsQueue;

    Snake(int cols, int rows);

    void update(float deltaTime);

    void setDirection(Direction newDirection);

    void grow();

    void reset();

    void popTurnQueue() {
        if (!turnPositionsQueue.empty()) {
            turnPositionsQueue.pop();
        }
    }

    // Getters
    [[nodiscard]] const std::vector<glm::vec2> &getBody() const { return body; }
    [[nodiscard]] glm::vec2 getHeadPosition() const { return body.empty() ? glm::vec2(0) : body[0]; }
    [[nodiscard]] Direction getCurrentDirection() const { return currentDirection; }
    [[nodiscard]] std::queue<TurnInfo> getTurnQueue() const { return turnPositionsQueue; }

    // Game logic
    [[nodiscard]] bool checkSelfCollision() const;

    [[nodiscard]] bool isOutOfBounds() const;

private:
    std::vector<glm::vec2> body;
    int pendingGrowth = 0;
    Direction currentDirection;
    Direction nextDirection;
    bool directionChanged;

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
