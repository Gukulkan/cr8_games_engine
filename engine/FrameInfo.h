#pragma once

#include "EngineCamera.h"

// lib
#include <vulkan/vulkan.h>

namespace engine {
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        EngineCamera &camera;
    };
}  // namespace