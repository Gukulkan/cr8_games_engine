cmake_minimum_required(VERSION 3.22)
project(engine VERSION 1.0.1 DESCRIPTION "CR8 Engine core")

set(CMAKE_CXX_STANDARD 23)

set(VULKAN_SDK_PATH /Users/gukulkan/VulkanSDK/1.3.216.0/macOS)
set(GLFW_PATH /usr/local/Cellar/glfw/3.3.7)


if (DEFINED VULKAN_SDK_PATH)
    message(STATUS "Found DEFINED Vulkan: ${VULKAN_SDK_PATH}")
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/include")
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/lib") 
    set(Vulkan_FOUND "True")
else()
    find_package(Vulkan REQUIRED FATAL_ERROR)
    message(STATUS "Found Vulkan: ${VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
    message(STATUS "Using vulkan Vulkan_INCLUDE_DIRS at: ${Vulkan_INCLUDE_DIRS}")
endif()


if (DEFINED GLFW_PATH)
    set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
    set(GLFW_LIB "${GLFW_PATH}/lib")
else()
    find_package(glfw3 3.3 REQUIRED)
    set(GLFW_LIB glfw)
    message(STATUS "Found GLFW")
endif()
if (NOT GLFW_LIB)
    message(FATAL_ERROR "Could not find glfw library!")
else()
    message(STATUS "Using glfw lib at: ${GLFW_LIB}")
endif()


add_library(${PROJECT_NAME}
        EngineWindow.cpp EngineWindow.h
        EnginePipeline.cpp EnginePipeline.h
        EngineDevice.cpp EngineDevice.h
        EngineSwapChain.cpp EngineSwapChain.h
        EngineModel.cpp EngineModel.h
        EngineGameObject.cpp EngineGameObject.h
        EngineRenderer.cpp EngineRenderer.h
        EngineSimpleRenderSystem.cpp EngineSimpleRenderSystem.h
        EngineCamera.cpp EngineCamera.h
        EngineKeyboardMovementController.cpp EngineKeyboardMovementController.h
        EngineBuffer.cpp EngineBuffer.h
        vendor/tinyobjectloader/tiny_obj_loader.h
        EngineUtils.h)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${Vulkan_INCLUDE_DIRS}>
            $<BUILD_INTERFACE:${GLFW_INCLUDE_DIRS}>
        PRIVATE
        .
        )

target_link_directories(${PROJECT_NAME} PUBLIC
        ${Vulkan_LIBRARIES}
        ${GLFW_LIB}
        )

target_link_libraries(${PROJECT_NAME} glfw vulkan)


target_compile_features(${PROJECT_NAME}
        PRIVATE
            cxx_std_17
        )

