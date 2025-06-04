#include "InputManager.hpp"

InputManager::InputManager(GLFWwindow* win) : window(win) {}

void InputManager::processInput(Snake& snake) const {
    if (const Direction newDirection = getDirectionFromInput(); newDirection != Direction::NONE) {
        snake.setDirection(newDirection);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool InputManager::shouldClose() const {
    return glfwWindowShouldClose(window);
}

Direction InputManager::getDirectionFromInput() const {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        return Direction::UP;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        return Direction::DOWN;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        return Direction::LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        return Direction::RIGHT;
    }
    
    return Direction::NONE;
}