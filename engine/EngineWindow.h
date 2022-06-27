#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <string>

class EngineWindow {
public:
    EngineWindow(int w, int h, std::string name);

    ~EngineWindow();

    EngineWindow(const EngineWindow &) = delete;

    EngineWindow &operator=(const EngineWindow &) = delete;

    bool shouldClose() { return glfwWindowShouldClose(window); }

private:
    void initWindow();

    const int width;
    const int height;

    std::string windowName;
    GLFWwindow *window;
};
