#ifndef CR8_GAMES_SANDBOXAPP_H
#define CR8_GAMES_SANDBOXAPP_H

#include "EngineDevice.h"
#include "EngineWindow.h"
#include "EnginePipeline.h"

class SandboxApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

private:
    EngineWindow engineWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    EngineDevice engineDevice{engineWindow};
    EnginePipeline enginePipeline{
            engineDevice,
            "shader/simple_shader.vert.spv",
            "shader/simple_shader.frag.spv",
            EnginePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
    };
};


#endif //CR8_GAMES_SANDBOXAPP_H
