
#include "SandboxApp.h"
#include "EngineWindow.h"
#include "EngineSimpleRenderSystem.h"
#include "EngineKeyboardMovementController.h"

#include <chrono>
#include <array>

SandboxApp::SandboxApp() {
    loadGameObjects();
}

SandboxApp::~SandboxApp() {}

void SandboxApp::run() {
    EngineSimpleRenderSystem simpleRenderSystem{engineDevice, engineRenderer.getSwapChainRenderPass()};

    EngineCamera camera{};

    auto viewerObject = EngineGameObject::createGameObject();
    EngineKeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!engineWindow.shouldClose()) {
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(engineWindow.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = engineRenderer.getAspectRatio();
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

void SandboxApp::loadGameObjects() {
    std::shared_ptr<EngineModel> engineModel = EngineModel::createModelFromFile(engineDevice, "models/colored_cube.obj");
    auto gameObj = EngineGameObject::createGameObject();
    gameObj.model = engineModel;
    gameObj.transform.translation = {.0f, .0f, 2.5f};
    gameObj.transform.scale = glm::vec3(3.f);
    gameObjects.push_back(std::move(gameObj));
}
