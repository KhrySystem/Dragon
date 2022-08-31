#pragma once

#include <windows.h>
#include <glfw/glfw3.h>
#include <cstdlib>
#include <string>

#ifdef _cplusplus
    #include <vulkan/vulkan.hpp>
#else
    #include <vulkan/vulkan.h>
#endif


#ifndef DGAPI
    #define DGAPI __declspec(dllexport)
#endif

#ifndef DRAGON_VERSION
    #define DRAGON_VERSION_MAJOR 0
    #define DRAGON_VERSION_MINOR 0
    #define DRAGON_VERSION_REVISION 1
    #define DRAGON_VERSION_PATCH 0
    #ifndef dgPrintVersionInfo
        DGAPI void dgPrintVersionInfo();
    #endif
#endif

#ifndef DG_BOOL
    #define DG_BOOL bool
#endif

#ifndef DG_TRUE
    #define DG_TRUE GLFW_TRUE
#endif

#ifndef DG_FALSE
    #define DG_FALSE GLFW_FALSE
#endif