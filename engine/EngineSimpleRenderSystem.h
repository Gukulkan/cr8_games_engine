#pragma once

#include "EngineDevice.h"
#include "EngineGameObject.h"
#include "EnginePipeline.h"
#include "EngineCamera.h"
#include "FrameInfo.h"

// std
#include <memory>
#include <vector>

class EngineSimpleRenderSystem {
public:
    EngineSimpleRenderSystem(EngineDevice &device, VkRenderPass renderPass);
    ~EngineSimpleRenderSystem();

    EngineSimpleRenderSystem(const EngineSimpleRenderSystem &) = delete;
    EngineSimpleRenderSystem &operator=(const EngineSimpleRenderSystem &) = delete;

    void renderGameObjects(engine::FrameInfo &frameInfo, std::vector<EngineGameObject> &gameObjects);

private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    EngineDevice &engineDevice;

    std::unique_ptr<EnginePipeline> enginePipeline;
    VkPipelineLayout pipelineLayout;
};
