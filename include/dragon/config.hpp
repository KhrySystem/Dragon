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
#include <vulkan/vulkan.hpp>

// CGLM
// linear algebra library. I'm too lazy to write something as optimized as this
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Vulkan Memory Allocator
// Helps with some device memory info. Again, I'm too lazy to write all this from scratch.
#include <vk_mem_alloc.h>

/**
 * @brief Macro for defining the export/import attribute of a function 
 */
#if BOOST_OS_WINDOWS && defined(DRAGON_BUILD_DLL)
    #define DGAPI __declspec(dllexport)
#elif BOOST_OS_WINDOWS && defined(DRAGON_DLL)
    #define DGAPI __declspec(dllimport)
#elif defined(__GNUC__) && defined(DRAGON_BUILD_DLL)
    #define DGAPI __attribute__((visibility("default")))
#else
    #define DGAPI 
#endif

#if !defined(NDEBUG) || defined(_DEBUG)
    #include <iostream>
#endif

#include <atomic>
#include <algorithm> 
#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <vector>
