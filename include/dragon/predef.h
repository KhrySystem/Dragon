#pragma once

#include <windows.h>
#include <glfw/glfw3.h>
#include <cstdlib>
#include <string>

#include <AL/al.h>

#ifdef _cplusplus
    #pragma message "Including hpp Vulkan."
    #include <vulkan/vulkan.hpp>
#else
    #include <vulkan/vulkan.h>
#endif

#define DGAPI __declspec(dllexport)

#define dgWindow GLFWwindow*
#define dgMonitor GLFWmonitor*


DGAPI unsigned int __dgGetMaximumProcesses__();
#ifndef DRAGON_MAXIMUM_PROCESSES
    #define DRAGON_MAXIMUM_PROCESSES __dgGetMaximumProcesses__()
#endif

#define DRAGON_VERSION_MAJOR 0
#define DRAGON_VERSION_MINOR 0
#define DRAGON_VERSION_REVISION 1
#define DRAGON_VERSION_PATCH 0
DGAPI void dgPrintVersionInfo();

#define DG_BOOL bool
#define DG_TRUE GLFW_TRUE
#define DG_FALSE GLFW_FALSE
#define DG_NULL VK_NULL_HANDLE