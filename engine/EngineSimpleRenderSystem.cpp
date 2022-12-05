
#include "EngineSimpleRenderSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>


struct SimplePushConstantData {
    glm::mat4 transform{1.f};
    glm::mat4 normalMatrix{1.f};
};

EngineSimpleRenderSystem::EngineSimpleRenderSystem(EngineDevice &device, VkRenderPass renderPass)
        : engineDevice{device} {
    createPipelineLayout();
    createPipeline(renderPass);
}

EngineSimpleRenderSystem::~EngineSimpleRenderSystem() {
    vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
}

void EngineSimpleRenderSystem::createPipelineLayout() {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void EngineSimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    EnginePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    enginePipeline = std::make_unique<EnginePipeline>(
            engineDevice,
            "shader/simple_shader.vert.spv",
            "shader/simple_shader.frag.spv",
            pipelineConfig);
}

void EngineSimpleRenderSystem::renderGameObjects(
        engine::FrameInfo &frameInfo, std::vector<EngineGameObject> &gameObjects) {
    enginePipeline->bind(frameInfo.commandBuffer);

    auto projectionView = frameInfo.camera.getProjection() * frameInfo.camera.getView();

    for (auto &obj: gameObjects) {
        SimplePushConstantData push{};
        auto modelMatrix = obj.transform.mat4();
        push.transform = projectionView * modelMatrix;
        push.normalMatrix = obj.transform.normalMatrix();

        vkCmdPushConstants(
                frameInfo.commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);
        obj.model->bind(frameInfo.commandBuffer);
        obj.model->draw(frameInfo.commandBuffer);
    }
}