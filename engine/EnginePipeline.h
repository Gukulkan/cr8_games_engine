#ifndef CR8_GAMES_ENGINEPIPELINE_H
#define CR8_GAMES_ENGINEPIPELINE_H

#include <string>
#include <vector>

#include "EngineDevice.h"
#include "vulkan/vulkan.h"

struct PipelineConfigInfo {
    VkViewport viewport{};
    VkRect2D scissor{};
    VkPipelineViewportStateCreateInfo viewportInfo{};
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class EnginePipeline {
public:
    EnginePipeline(      EngineDevice& device,
                         const std::string& vertFilepath,
                         const std::string& fragFilepath,
                         const PipelineConfigInfo& configInfo);
    ~EnginePipeline();


    EnginePipeline(const EnginePipeline&) = delete;
    void operator=(const EnginePipeline&) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

private:
    static std::vector<char> readFile(const std::string& filepath);

    void createGraphicsPipeline(
            const std::string& vertFilepath,
            const std::string& fragFilepath,
            const PipelineConfigInfo& configInfo);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    EngineDevice& engineDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};


#endif //CR8_GAMES_ENGINEPIPELINE_H
