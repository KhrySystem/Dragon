#pragma once

#include <glfw/glfw3.h>

#define dgMonitor GLFWmonitor*

struct dgWindowCreateParams {
    int width;
    int height;
    bool isFullscreen;
    bool isBorderless;
    dgMonitor monitor = NULL;
    const char* title;
};