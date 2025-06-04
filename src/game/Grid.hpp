#pragma once
#include <glm.hpp>
#include <vector>

class Grid {
public:
    Grid(int gridCols, int gridRows, const glm::vec3& scale);
    
    void generateOffsets();
    
    // Getters
    [[nodiscard]] int getCols() const { return cols; }
    [[nodiscard]] int getRows() const { return rows; }
    [[nodiscard]] const glm::vec3& getCellScale() const { return cellScale; }
    [[nodiscard]] float getCellStep() const { return cellStep; }
    [[nodiscard]] const std::vector<glm::vec3>& getOffsets() const { return gridOffsets; }
    
    // Coordinate conversion
    [[nodiscard]] glm::vec3 gridToWorldPosition(const glm::vec2& gridPos) const;
    [[nodiscard]] glm::vec2 worldToGridPosition(const glm::vec3& worldPos) const;
    [[nodiscard]] bool isValidGridPosition(const glm::vec2& gridPos) const;

private:
    int cols;
    int rows;
    glm::vec3 cellScale;
    float cellStep;
    std::vector<glm::vec3> gridOffsets;
};