#pragma once

#include <dragon/graphics/Object.h>
#include <dragon/graphics/UIElement.h>
#include <dragon/predef.h>
#include <vector>
#ifdef _cplusplus
    #include <vulkan/vulkan.hpp>
#else
    #include <vulkan/vulkan.h>
#endif
#include <glfw/glfw3.h>

struct dgEngine
{
    dgWindow window;

    std::vector<UIElement*> uiElements;
    std::vector<Object*> objects;

    VkInstance instance;
    VkApplicationInfo appInfo{};
    VkInstanceCreateInfo createInfo{};

    uint32_t glfwExtensionCount = 0;
    uint32_t extensionCount = 0;
    const char** glfwExtensions;
    VkResult createResult;
    std::vector<VkExtensionProperties> extensions;
    #ifndef DRAGON_FULL_POWER
        const std::vector<const char*> requestedValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        uint32_t layerCount;
        std::vector<VkLayerProperties> availableLayers;
        std::vector<const char*> availibleLayerNames;
    #endif
};

DGAPI dgEngine* dgCreateEngine(char* appName);