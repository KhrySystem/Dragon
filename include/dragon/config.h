#pragma once
// Boost::Predef
// Responsible for figuring out what operating system, compiler, all that fun so I don't have to
#include <boost/predef.h>

// GLFW Predefines
// Toggles so we use Vulkan instead of OpenGL
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

// GLFW
// Responsible for window systems
#include <GLFW/glfw3.h>

// Vulkan
// This is effectively the entire library, so keep this around
#ifdef __cplusplus
    #include <vulkan/vulkan.hpp>
#else
    #include <vulkan/vulkan.h>
#endif

// CGLM
// linear algebra library. I'm too lazy to write something as optimized as this
#include <cglm/cglm.h>

/**
 * @brief Macro for defining the export/import attribute of a function 
 */
#ifndef __cplusplus
    #if BOOST_OS_WINDOWS && defined(DRAGON_BUILD_DLL)
        #define DGAPI __declspec(dllexport)
    #elif BOOST_OS_WINDOWS && defined(DRAGON_DLL)
        #define DGAPI __declspec(dllimport)
    #elif defined(__GNUC__) && defined(DRAGON_BUILD_DLL)
        #define DGAPI __attribute__((visibility("default")))
    #else
        #define DGAPI 
    #endif
#else 
    #if BOOST_OS_WINDOWS && defined(DRAGON_BUILD_DLL)
        #define DGAPI extern "C" __declspec(dllexport)
    #elif BOOST_OS_WINDOWS && defined(DRAGON_DLL)
        #define DGAPI extern "C" __declspec(dllimport)
    #elif defined(__GNUC__) && defined(DRAGON_BUILD_DLL)
        #define DGAPI extern "C" __attribute__((visibility("default")))
    #else
        #define DGAPI extern "C" 
    #endif
#endif

#ifdef DRAGON_BUILD_DLL
    #if BOOST_OS_WINDOWS
        BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
    #else
        __attribute__((constructor)) void dllMain();
    #endif
#endif