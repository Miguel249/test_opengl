#pragma once
#include <glm.hpp>
#include <vector>

class Grid {
public:
    Grid(int gridCols, int gridRows, const glm::vec3& scale);
    
    void generateOffsets();
    
    // Getters
    int getCols() const { return cols; }
    int getRows() const { return rows; }
    const glm::vec3& getCellScale() const { return cellScale; }
    float getCellStep() const { return cellStep; }
    const std::vector<glm::vec3>& getOffsets() const { return gridOffsets; }
    
    // Coordinate conversion
    glm::vec3 gridToWorldPosition(const glm::vec2& gridPos) const;
    glm::vec2 worldToGridPosition(const glm::vec3& worldPos) const;
    bool isValidGridPosition(const glm::vec2& gridPos) const;

private:
    int cols;
    int rows;
    glm::vec3 cellScale;
    float cellStep;
    std::vector<glm::vec3> gridOffsets;
};