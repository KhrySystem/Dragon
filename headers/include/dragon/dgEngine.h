#pragma once

#include <glfw/glfw3.h>
#include <dragon/graphics/predef.h>
#include <dragon/graphics/Object.h>
#include <dragon/graphics/UIElement.h>
#include <vector>

struct dgEngine {
    GLFWwindow* window;
    #if defined(DRAGON_USE_VULKAN_GRAPHICS)
        VkInstance vulkan;
        VkApplicationInfo appInfo{};
        VkInstanceCreateInfo createInfo{};
        uint32_t glfwExtensionCount = 0;
        uint32_t extensionCount = 0;
        const char** glfwExtensions;
        VkResult createResult;
        std::vector<VkExtensionProperties> extensions;
    #elif defined(DRAGON_USE_OPENGL_GRAPHICS)
        unsigned int vertexShader;
        unsigned int fragmentShader;
    #endif
    
    std::vector<Object*> objects;
    std::vector<UIElelment*> uiElements;
};