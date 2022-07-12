#ifndef CR8_GAMES_SANDBOXAPP_H
#define CR8_GAMES_SANDBOXAPP_H

#include "EngineDevice.h"
#include "EngineWindow.h"
#include "EnginePipeline.h"
#include "EngineSwapChain.h"
#include "EngineModel.h"

#include <memory>
#include <vector>

class SandboxApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    SandboxApp();

    ~SandboxApp();

    SandboxApp(const SandboxApp &) = delete;

    SandboxApp &operator=(const SandboxApp &) = delete;

    void run();

private:
    void loadModels();

    void createPipelineLayout();

    void createPipeline();

    void createCommandBuffers();

    void drawFrame();

    EngineWindow engineWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    EngineDevice engineDevice{engineWindow};
    EngineSwapChain engineSwapChain{engineDevice, engineWindow.getExtent()};
    std::unique_ptr<EnginePipeline> enginePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<EngineModel> engineModel;

};


#endif //CR8_GAMES_SANDBOXAPP_H
