
#include "SandboxApp.h"
#include "EngineWindow.h"
#include "EngineSimpleRenderSystem.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/gtc/constants.hpp>

#include <array>

SandboxApp::SandboxApp() {
    loadGameObjects();
}

SandboxApp::~SandboxApp() {}

void SandboxApp::run() {
    EngineSimpleRenderSystem simpleRenderSystem{engineDevice, engineRenderer.getSwapChainRenderPass()};

    EngineCamera camera{};
    camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

    while (!engineWindow.shouldClose()) {
        glfwPollEvents();

        float aspect = engineRenderer.getAspectRatio();
        // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        if (auto commandBuffer = engineRenderer.beginFrame()) {
            engineRenderer.beginSwapChainRenderPass(commandBuffer);

            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

            engineRenderer.endSwapChainRenderPass(commandBuffer);
            engineRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(engineDevice.device());
}

std::unique_ptr<EngineModel> createCubeModel(EngineDevice &device, glm::vec3 offset) {
    std::vector<EngineModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f},  {.9f, .9f, .9f}},
            {{-.5f, .5f,  .5f},   {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f},   {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f},  {.9f, .9f, .9f}},
            {{-.5f, .5f,  -.5f},  {.9f, .9f, .9f}},
            {{-.5f, .5f,  .5f},   {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f,  -.5f, -.5f},  {.8f, .8f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .8f, .1f}},
            {{.5f,  -.5f, .5f},   {.8f, .8f, .1f}},
            {{.5f,  -.5f, -.5f},  {.8f, .8f, .1f}},
            {{.5f,  .5f,  -.5f},  {.8f, .8f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f},  {.9f, .6f, .1f}},
            {{.5f,  -.5f, .5f},   {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f},   {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f},  {.9f, .6f, .1f}},
            {{.5f,  -.5f, -.5f},  {.9f, .6f, .1f}},
            {{.5f,  -.5f, .5f},   {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f,  -.5f},  {.8f, .1f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .1f, .1f}},
            {{-.5f, .5f,  .5f},   {.8f, .1f, .1f}},
            {{-.5f, .5f,  -.5f},  {.8f, .1f, .1f}},
            {{.5f,  .5f,  -.5f},  {.8f, .1f, .1f}},
            {{.5f,  .5f,  .5f},   {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f},  {.1f, .1f, .8f}},
            {{.5f,  .5f,  0.5f},  {.1f, .1f, .8f}},
            {{-.5f, .5f,  0.5f},  {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f},  {.1f, .1f, .8f}},
            {{.5f,  -.5f, 0.5f},  {.1f, .1f, .8f}},
            {{.5f,  .5f,  0.5f},  {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f,  .5f,  -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f,  -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f,  -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f,  .5f,  -0.5f}, {.1f, .8f, .1f}},

    };
    for (auto &v: vertices) {
        v.position += offset;
    }
    return std::make_unique<EngineModel>(device, vertices);
}

void SandboxApp::loadGameObjects() {
    std::shared_ptr<EngineModel> engineModel = createCubeModel(engineDevice, {.0f, .0f, .0f});
    auto cube = EngineGameObject::createGameObject();
    cube.model = engineModel;
    cube.transform.translation = {.0f, .0f, 2.5f};
    cube.transform.scale = {.5f, .5f, .5f};
    gameObjects.push_back(std::move(cube));
}
