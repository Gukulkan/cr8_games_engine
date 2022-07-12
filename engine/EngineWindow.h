#pragma once

#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>

#include <string>

class EngineWindow {
public:
    EngineWindow(int w, int h, std::string name);

    ~EngineWindow();

    EngineWindow(const EngineWindow &) = delete;

    EngineWindow &operator=(const EngineWindow &) = delete;

    bool shouldClose() { return glfwWindowShouldClose(window); }

    VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
    void initWindow();

    int width;
    int height;

    std::string windowName;
    GLFWwindow *window;
};
