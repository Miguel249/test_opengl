#include "Game.hpp"
#include <iostream>
#include "input/InputManager.hpp"
#include <chrono>

Game::Game()
    : window(nullptr), deltaTime(0.0f), isRunning(false) {
    lastTime = std::chrono::high_resolution_clock::now();
}

Game::~Game() {
    shutdown();
}

bool Game::initialize() {
    if (!initializeWindow()) {
        return false;
    }

    if (!initializeOpenGL()) {
        return false;
    }

    initializeComponents();
    isRunning = true;
    return true;
}

bool Game::initializeWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW3" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    return true;
}

bool Game::initializeOpenGL() {
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    return true;
}

void Game::initializeComponents() {
    // Initialize grid
    constexpr int gridCols = 9;
    constexpr int gridRows = 9;
    constexpr glm::vec3 cellScale{ 0.2f, 0.2f, 1.0f };

    grid = std::make_unique<Grid>(gridCols, gridRows, cellScale);
    grid->generateOffsets();

    // Initialize snake
    snake = std::make_unique<Snake>(gridCols, gridRows);

    // Initialize renderer
    renderer = std::make_unique<Renderer>();
    const std::string resourcesDir = RESOURCES_DIR;
    renderer->initialize(resourcesDir, *grid);

    // Initialize input manager
    inputManager = std::make_unique<InputManager>(window);
}

void Game::run() {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    const int refreshRate = mode->refreshRate;
    const double targetFrameTime = 1.0 / refreshRate;

    while (isRunning && !glfwWindowShouldClose(window)) {
        calculateDeltaTime();

        update();
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(targetFrameTime);
    }
}

void Game::update() const {
    inputManager->processInput(*snake);
    snake->update(deltaTime);

    // Check game over conditions
    if (snake->checkSelfCollision() || snake->isOutOfBounds()) {
        snake->reset();
    }
}

void Game::render() const {
    Renderer::clear();
    renderer->renderGrid(*grid);
    renderer->renderSnake(*snake, *grid);
}

void Game::calculateDeltaTime() {
    const auto currentTime{ std::chrono::high_resolution_clock::now() };
    deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
}

void Game::shutdown() {
    renderer.reset();
    snake.reset();
    grid.reset();
    inputManager.reset();

    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void Game::framebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
}
