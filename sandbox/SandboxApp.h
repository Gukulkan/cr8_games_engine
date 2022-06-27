#ifndef CR8_GAMES_SANDBOXAPP_H
#define CR8_GAMES_SANDBOXAPP_H


#include "EngineWindow.h"

class SandboxApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

private:
    EngineWindow engineWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
};


#endif //CR8_GAMES_SANDBOXAPP_H
