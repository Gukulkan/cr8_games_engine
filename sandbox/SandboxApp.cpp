
#include "SandboxApp.h"
#include "EngineWindow.h"

void SandboxApp::run() {
    while (!engineWindow.shouldClose()) {
        glfwPollEvents();
    }
}