#pragma once

#include "dragon_core.h"

#ifdef __cplusplus
extern "C" {
#endif

struct DgWindow {
    GLFWwindow* pWindow;
    VkSurfaceKHR* surface;
};

typedef DgWindow DgWindow;

#ifdef __cplusplus
}
#endif