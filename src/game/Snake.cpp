#include "Snake.hpp"

#include <iostream>

Snake::Snake(const int cols, const int rows)
    : currentDirection(Direction::NONE), nextDirection(Direction::NONE), directionChanged(false),
      moveTimer(0.0f), gridCols(cols), gridRows(rows) {
    reset();
}

void Snake::update(const float deltaTime) {
    moveTimer += deltaTime;

    if (moveTimer >= MOVE_INTERVAL && nextDirection != Direction::NONE) {
        currentDirection = nextDirection;
        move();
        moveTimer = 0.0f;
        directionChanged = false;
    }

}

void Snake::setDirection(const Direction newDirection) {
    if (directionChanged) return;

    if (newDirection != Direction::NONE &&
        !isOppositeDirection(currentDirection, newDirection)) {
        nextDirection = newDirection;
        directionChanged = true;
    }
}

void Snake::move() {
    if (body.empty()) return;

    const glm::vec2 movement = getMovementVector(currentDirection);
    const glm::vec2 newHead = body[0] + movement;

    // Add new head
    body.insert(body.begin(), newHead);

    // Remove tail (for now, we'll handle growth separately)
    body.pop_back();
}

void Snake::grow() {
    if (body.empty()) return;

    // Add a new segment at the tail
    const glm::vec2 tail = body.back();
    body.push_back(tail);
}

void Snake::reset() {
    body.clear();
    // Start at center of grid
    glm::vec2 centerPos(gridCols / 2, gridRows / 2);
    glm::vec2 center2Pos(gridCols / 2, gridRows / 2 - 1);
    glm::vec2 center3Pos(gridCols / 2, gridRows / 2 - 2);
    // glm::vec2 center4Pos(gridCols / 2, gridRows / 2 - 3);
    // glm::vec2 center5Pos(gridCols / 2, gridRows / 2 - 4);
    body.emplace_back(centerPos);
    body.emplace_back(center2Pos);
    body.emplace_back(center3Pos);
    // body.emplace_back(center4Pos);
    // body.emplace_back(center5Pos);
    currentDirection = Direction::NONE;
    nextDirection = Direction::NONE;
    moveTimer = 0.0f;
}

bool Snake::checkSelfCollision() const {
    if (body.size() <= 1) return false;

    const glm::vec2 &head = body[0];
    for (size_t i = 1; i < body.size(); ++i) {
        if (head.x == body[i].x && head.y == body[i].y) {
            return true;
        }
    }
    return false;
}

bool Snake::isOutOfBounds() const {
    if (body.empty()) return false;

    const glm::vec2 &head = body[0];
    return head.x < 0 || head.x >= gridCols ||
           head.y < 0 || head.y >= gridRows;
}

bool Snake::isOppositeDirection(const Direction dir1, const Direction dir2) {
    return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
           (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
           (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
           (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
}

glm::vec2 Snake::getMovementVector(const Direction dir) {
    switch (dir) {
        case Direction::UP: return { 0, 1 };
        case Direction::DOWN: return { 0, -1 };
        case Direction::LEFT: return { -1, 0 };
        case Direction::RIGHT: return { 1, 0 };
        default: return { 0, 0 };
    }
}
