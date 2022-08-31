#pragma once

#include <dragon/audio/AudioChannel.h>
#include <dragon/audio/AudioEngine.h>
#include <dragon/audio/AudioListener.h>
#include <dragon/audio/AudioSource.h>
#include <dragon/collision/CollisionAPI.h>
#include <dragon/collision/Fluid.h>
#include <dragon/collision/RigidBody.h>
#include <dragon/collision/RigidBodyMesh.h>
#include <dragon/collision/SoftBody.h>
#include <dragon/compute/ComputeEngine.h>
#include <dragon/graphics/dgBaseObj.h>
#include <dragon/graphics/Object.h>
#include <dragon/graphics/UIElement.h>
#include <dragon/graphics/Window.h>
#include <dragon/graphics/WindowCreateParams.h>
#ifdef DRAGON_BOOST_FOUND
    #include <dragon/BoostInclude.h>
#endif
#include <dragon/predef.h>
#include <vector>
#ifdef _cplusplus
    #include <vulkan/vulkan.hpp>
#else
    #include <vulkan/vulkan.h>
#endif
#include <glfw/glfw3.h>

struct dgEngine {
    dgWindow window;

    const char* appName;

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

    uint32_t deviceCount = 0;
    VkPhysicalDevice device = VK_NULL_HANDLE;

    #ifndef DRAGON_FULL_POWER
        const std::vector<const char*> requestedValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        uint32_t layerCount;
        std::vector<VkLayerProperties> availableLayers;
        std::vector<const char*> availibleLayerNames;
    #endif
    dgEngine* shared;
};

DGAPI dgEngine* dgCreateEngine(const char* appName);
DGAPI DG_BOOL dgShareResources(dgEngine* e1, dgEngine* e2);