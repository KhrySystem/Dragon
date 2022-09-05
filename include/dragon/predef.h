#pragma once

#include <cstdlib>
#include <string>

#include <windows.h>
#include <glfw/glfw3.h>
#include <AL/al.h>
#ifdef DRAGON_USE_OPENCL
    #include <CL/cl.hpp>
    #define CL_KERNEL kernel
#endif
#ifdef _cplusplus
    #pragma message "Including hpp Vulkan."
    #include <vulkan/vulkan.hpp>
    #if _cplusplus == 1
        #define DRAGON_CXX_VERSION -1
    #elif _cplusplus == 199711L
        #define DRAGON_CXX_VERSION 0
    #elif _cplusplus == 201103L
        #define DRAGON_CXX_VERSION 11
    #elif _cplusplus == 201402L
        #define DRAGON_CXX_VERSION 14
    #elif _cplusplus == 201703L
        #define DRAGON_CXX_VERSION 17
    #elif _cplusplus == 202002L
        #define DRAGON_CXX_VERSION 20
    #endif
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

typedef bool DG_BOOL;
#define DG_TRUE GLFW_TRUE
#define DG_FALSE GLFW_FALSE
#define DG_NULL VK_NULL_HANDLE