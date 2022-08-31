#pragma once

#include <glfw/glfw3.h>
#include <dragon/graphics/Object.h>
#include <dragon/graphics/UIElement.h>
#include <vector>

#ifndef Object
    struct Object;
#endif
#ifndef UIElement
    struct UIElement;
#endif

struct dgEngine {
    GLFWwindow* window;
    VkInstance vulkan;
    VkApplicationInfo appInfo{};
    VkInstanceCreateInfo createInfo{};
    uint32_t glfwExtensionCount = 0;
    uint32_t extensionCount = 0;
    const char** glfwExtensions;
    VkResult createResult;
    std::vector<VkExtensionProperties> extensions;
    
    std::vector<Object*> objects;
    std::vector<UIElelment*> uiElements;
};