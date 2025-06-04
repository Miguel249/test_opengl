#pragma once
#include <chrono>
#include "game/Food.hpp"
#include "core/Renderer.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include "input/InputManager.hpp"

class Game {
public:
    Game();
    ~Game();

    bool initialize();
    void run();
    void shutdown();

private:
    // Window management
    GLFWwindow* window;
    static constexpr unsigned int SCREEN_WIDTH = 800;
    static constexpr unsigned int SCREEN_HEIGHT = 600;

    // Game components
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Snake> snake;
    std::unique_ptr<Food> food;
    std::unique_ptr<Grid> grid;
    std::unique_ptr<InputManager> inputManager;

    // Timing
    float deltaTime;
    std::chrono::high_resolution_clock::time_point lastTime;

    // Game state
    bool isRunning;

    // Static callbacks
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    bool initializeWindow();

    static bool initializeOpenGL();
    void initializeComponents();

    void update() const;
    void render() const;
    void calculateDeltaTime();
};
