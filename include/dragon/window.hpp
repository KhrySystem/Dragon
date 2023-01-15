#pragma once

#include <string>

#include "dragon_core.h"

typedef struct {
	GLFWwindow* window;
	VkSurfaceKHR surface;
	VkSurfaceFormatKHR surfaceFormat;
	VkPresentModeKHR presentMode;
	VkExtent2D extent2D;
} DgWindow;
