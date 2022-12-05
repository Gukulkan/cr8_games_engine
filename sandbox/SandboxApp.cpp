
#include "SandboxApp.h"
#include "EngineWindow.h"
#include "EngineSimpleRenderSystem.h"
#include "EngineKeyboardMovementController.h"
#include "EngineBuffer.h"

#include <chrono>
#include <array>

struct GlobalUbo {
    glm::mat4 projectionView{1.f};
    glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
};

SandboxApp::SandboxApp() {
    loadGameObjects();
}

SandboxApp::~SandboxApp() {}

void SandboxApp::run() {
    engine::EngineBuffer globalUboBuffer{
            engineDevice,
            sizeof(GlobalUbo),
            EngineSwapChain::MAX_FRAMES_IN_FLIGHT,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            engineDevice.properties.limits.minUniformBufferOffsetAlignment,
    };
    globalUboBuffer.map();

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
            int frameIndex = engineRenderer.getFrameIndex();
            engine::FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera};

            // update
            GlobalUbo ubo{};
            ubo.projectionView = camera.getProjection() * camera.getView();
            globalUboBuffer.writeToIndex(&ubo, frameIndex);
            globalUboBuffer.flushIndex(frameIndex);

            // render
            engineRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
            engineRenderer.endSwapChainRenderPass(commandBuffer);
            engineRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(engineDevice.device());
}

void SandboxApp::loadGameObjects() {
    std::shared_ptr<EngineModel> engineModel = EngineModel::createModelFromFile(engineDevice, "models/flat_vase.obj");
    auto flatVase = EngineGameObject::createGameObject();
    flatVase.model = engineModel;
    flatVase.transform.translation = {-.5f, .5f, 2.5f};
    flatVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.push_back(std::move(flatVase));

    engineModel = EngineModel::createModelFromFile(engineDevice, "models/smooth_vase.obj");
    auto smoothVase = EngineGameObject::createGameObject();
    smoothVase.model = engineModel;
    smoothVase.transform.translation = {.5f, .5f, 2.5f};
    smoothVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.push_back(std::move(smoothVase));
}
