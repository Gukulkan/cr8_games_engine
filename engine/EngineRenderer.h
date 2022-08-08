#pragma once

#include "EngineDevice.h"
#include "EngineSwapChain.h"
#include "EngineWindow.h"

// std
#include <cassert>
#include <memory>
#include <vector>

class EngineRenderer {
public:
    EngineRenderer(EngineWindow &window, EngineDevice &device);
    ~EngineRenderer();

    EngineRenderer(const EngineRenderer &) = delete;
    EngineRenderer &operator=(const EngineRenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return engineSwapChain->getRenderPass(); }
    float getAspectRatio() const { return engineSwapChain->extentAspectRatio(); }
    bool isFrameInProgress() const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const {
        assert(isFrameStarted && "Cannot get frame index when frame not in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    EngineWindow &engineWindow;
    EngineDevice &engineDevice;
    std::unique_ptr<EngineSwapChain> engineSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex;
    bool isFrameStarted;

};

