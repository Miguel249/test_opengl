#pragma once

#include "Snake.hpp"
#include <glm.hpp>

class Food {
public:
    Food(int cols, int rows);

    void spawnFood(const Snake & snake);

    [[nodiscard]] glm::vec2 getPosition() const { return position; }

    [[nodiscard]] bool isEaten(const glm::vec2 &snakeHead) const;
private:
    glm::vec2 position{};

    int gridCols;
    int gridRows;
};

