#pragma once

#include "dragon_core.hpp"

// Global State variables for Dragon
struct __DgTypes_T {
    std::vector<std::string> vAvailableExtensions;
    std::vector<std::string> vAvailableLayers;
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
};

typedef struct __DgTypes_T __DgTypes;

extern __DgTypes __dghints;

DGAPI void dgGLFWFramebufferSizeCallback(GLFWwindow* pWindow, int width, int height);