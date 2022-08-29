#pragma once

#include <glfw/glfw3.h>
#include <dragon/graphics/predef.h>
#include <vector>

struct dgEngine {
    GLFWwindow* window;
    #ifdef DRAGON_USE_VULKAN_GRAPHICS
        VkInstance vulkan;
        VkApplicationInfo appInfo{};
        VkInstanceCreateInfo createInfo{};
        uint32_t glfwExtensionCount = 0;
        uint32_t extensionCount = 0;
        const char** glfwExtensions;
        VkResult createResult;
        std::vector<VkExtensionProperties> extensions;
    #endif
};