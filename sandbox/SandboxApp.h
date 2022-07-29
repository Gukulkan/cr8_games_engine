#ifndef CR8_GAMES_SANDBOXAPP_H
#define CR8_GAMES_SANDBOXAPP_H

#include "EngineDevice.h"
#include "EngineWindow.h"
#include "EngineGameObject.h"
#include "EngineRenderer.h"

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
    void loadGameObjects();

    EngineWindow engineWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    EngineDevice engineDevice{engineWindow};
    EngineRenderer engineRenderer{engineWindow, engineDevice};

    std::vector<EngineGameObject> gameObjects;
};


#endif //CR8_GAMES_SANDBOXAPP_H
