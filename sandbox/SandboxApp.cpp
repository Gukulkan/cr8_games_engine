
#include "SandboxApp.h"
#include "EngineWindow.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <array>
#include <cassert>
#include <stdexcept>

struct SimplePushConstantData {
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

SandboxApp::SandboxApp() {
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

SandboxApp::~SandboxApp() { vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr); }

void SandboxApp::run() {
    while (!engineWindow.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(engineDevice.device());
}

void SandboxApp::loadModels() {
    std::vector<EngineModel::Vertex> vertices{
            {{0.0f,  -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f,  0.5f},  {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}}};
    engineModel = std::make_unique<EngineModel>(engineDevice, vertices);
}

void SandboxApp::createPipelineLayout() {

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

void SandboxApp::recreateSwapChain() {
    auto extent = engineWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = engineWindow.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(engineDevice.device());

    if (engineSwapChain == nullptr) {
        engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent);
    } else {
        engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent, std::move(engineSwapChain));
        if (engineSwapChain->imageCount() != commandBuffers.size()) {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }

    createPipeline();
}

void SandboxApp::createPipeline() {
    assert(engineSwapChain != nullptr && "Cannot create pipeline before swap chain");
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    EnginePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = engineSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    enginePipeline = std::make_unique<EnginePipeline>(
            engineDevice,
            "shader/simple_shader.vert.spv",
            "shader/simple_shader.frag.spv",
            pipelineConfig);
}

void SandboxApp::createCommandBuffers() {
    commandBuffers.resize(engineSwapChain->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = engineDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(engineDevice.device(), &allocInfo, commandBuffers.data()) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void SandboxApp::freeCommandBuffers() {
    vkFreeCommandBuffers(
            engineDevice.device(),
            engineDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
    commandBuffers.clear();
}


void SandboxApp::recordCommandBuffer(int imageIndex) {
    static int frame = 30;
    frame = (frame + 1) % 100;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = engineSwapChain->getRenderPass();
    renderPassInfo.framebuffer = engineSwapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(engineSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(engineSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, engineSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    enginePipeline->bind(commandBuffers[imageIndex]);
    engineModel->bind(commandBuffers[imageIndex]);
    for (int j = 0; j < 4; j++) {
        SimplePushConstantData push{};
        push.offset = {-0.5f + frame * 0.02f, -0.4f + j * 0.25f};
        push.color = {0.0f, 0.0f, 0.2f + 0.2f * j};

        vkCmdPushConstants(
                commandBuffers[imageIndex],
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);
        engineModel->draw(commandBuffers[imageIndex]);
    }
    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void SandboxApp::drawFrame() {
    uint32_t imageIndex;
    auto result = engineSwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    recordCommandBuffer(imageIndex);
    result = engineSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        engineWindow.wasWindowResized()) {
        engineWindow.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }
}

