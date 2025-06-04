#include "Food.hpp"
#include <random>
#include <algorithm>

Food::Food(const int cols, const int rows): gridCols(cols), gridRows(rows) {
    position = glm::vec2(0, 0);
}

void Food::spawnFood(const Snake &snake) {
    const auto &snakeBody = snake.getBody();
    std::vector<glm::vec2> availablePositions;

    for (int x = 0; x < gridCols; ++x) {
        for (int y = 0; y < gridRows; ++y) {
            glm::vec2 pos = {x, y};
            const bool isOccupied = std::ranges::any_of(snakeBody,
                                                        [&](const glm::vec2 &segment) {
                                                            return segment == pos;
                                                        });
            if (!isOccupied) {
                availablePositions.push_back(pos);
            }
        }
    }

    if (availablePositions.empty()) {
        position = glm::vec2(-1, -1);
        return;
    }

    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(availablePositions.size() - 1));
    position = availablePositions[dist(rng)];
}

bool Food::isEaten(const glm::vec2 &snakeHead) const {
    return snakeHead == position;
}
