#pragma once
#include <GLFW/glfw3.h>

#include "game/Snake.hpp"

class InputManager {

public:
    explicit InputManager(GLFWwindow* win);

    void processInput(Snake& snake) const;
    [[nodiscard]] bool shouldClose() const;

private:
    GLFWwindow* window;
    [[nodiscard]] Direction getDirectionFromInput() const;
};
