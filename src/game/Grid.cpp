#include "Grid.hpp"

Grid::Grid(const int gridCols, const int gridRows, const glm::vec3& scale)
    : cols(gridCols), rows(gridRows), cellScale(scale) {
    cellStep = cellScale.x; // Assuming uniform step
}

void Grid::generateOffsets() {
    gridOffsets.clear();
    gridOffsets.reserve(cols * rows);
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            float x = (static_cast<float>(col) - static_cast<float>(cols - 1) / 2.0f) * cellScale.x;
            float y = (static_cast<float>(row) - static_cast<float>(rows - 1) / 2.0f) * cellScale.y;
            gridOffsets.emplace_back(x, y, 0.0f);
        }
    }
}

glm::vec3 Grid::gridToWorldPosition(const glm::vec2& gridPos) const {
    const float x = (gridPos.x - static_cast<float>(cols - 1) / 2.0f) * cellScale.x;
    const float y = (gridPos.y - static_cast<float>(rows - 1) / 2.0f) * cellScale.y;
    return {x, y, 0.0f};
}

glm::vec2 Grid::worldToGridPosition(const glm::vec3& worldPos) const {
    const float gridX = (worldPos.x / cellScale.x) + static_cast<float>(cols - 1) / 2.0f;
    const float gridY = (worldPos.y / cellScale.y) + static_cast<float>(rows - 1) / 2.0f;
    return {gridX, gridY};
}

bool Grid::isValidGridPosition(const glm::vec2& gridPos) const {
    return gridPos.x >= 0 && gridPos.x < static_cast<float>(cols) &&
           gridPos.y >= 0 && gridPos.y < static_cast<float>(rows);
}