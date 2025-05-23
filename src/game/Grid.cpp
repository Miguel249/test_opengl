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
            float x = (static_cast<float>(col) - static_cast<float>(cols / 2)) * cellScale.x;
            float y = (static_cast<float>(row) - static_cast<float>(rows / 2)) * cellScale.y;
            gridOffsets.emplace_back(x, y, 0.0f);
        }
    }
}

glm::vec3 Grid::gridToWorldPosition(const glm::vec2& gridPos) const {
    const float x = (gridPos.x - static_cast<float>(cols / 2)) * cellScale.x;
    const float y = (gridPos.y - static_cast<float>(rows / 2)) * cellScale.y;
    return glm::vec3(x, y, 0.0f);
}

glm::vec2 Grid::worldToGridPosition(const glm::vec3& worldPos) const {
    const float gridX = (worldPos.x / cellScale.x) + static_cast<float>(cols / 2);
    const float gridY = (worldPos.y / cellScale.y) + static_cast<float>(rows / 2);
    return glm::vec2(gridX, gridY);
}

bool Grid::isValidGridPosition(const glm::vec2& gridPos) const {
    return gridPos.x >= 0 && gridPos.x < cols && 
           gridPos.y >= 0 && gridPos.y < rows;
}