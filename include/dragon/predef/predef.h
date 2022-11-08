#pragma once

#include "al.h"
#include "core.h"
#include "graphics.h"
#include "opencl.h"
#include "platform.h"
#include "sli.h"

typedef VkBool32 DgBool32;
#define DG_TRUE GLFW_TRUE
#define DG_FALSE GLFW_FALSE
typedef uint_fast64_t DgUInt64;
typedef int_fast64_t DgInt64;
typedef uint32_t DgUInt32;
typedef int32_t DgInt32;

#ifndef DRAGON_MESSAGE_ENABLED
	#define DRAGON_MESSAGE_ENABLED DG_TRUE
#endif
#ifndef DRAGON_SEND_DEPRECIATED
	#if DRAGON_MESSAGE_ENABLED
		#define DRAGON_SEND_DEPRECIATED DG_TRUE
	#else
		#define DRAGON_SEND_DEPRECIATED DG_FALSE
	#endif
#endif
#ifndef DRAGON_SEND_EXCEPTIONS
	#if DRAGON_MESSAGE_ENABLED
		#define DRAGON_SEND_EXCEPTIONS DG_TRUE
	#else
		#define DRAGON_SEND_EXCEPTIONS DG_FALSE
	#endif
#endif

#if defined(_WIN32) && defined(DRAGON_BUILD_DLL)
    #define DGAPI __declspec(dllexport)
#elif defined(_WIN32) && defined(DRAGON_CALL_DLL)
    #define DGAPI __declspec(dllimport)
#elif defined(__GNUC__) && defined(DRAGON_BUILD_DLL)
    #define DGAPI __attribute__((visibility("default")))
#else
    #define DGAPI
#endif

#define DRAGON_AUDIO_BUFFER_COUNT 16

#define DRAGON_AUDIO_ENABLED    0x00000000
#define DRAGON_GRAPHICS_ENABLED 0x00000002
#define DRAGON_PARTICLE_ENABLED 0x00000004
#define DRAGON_PHYSICS_ENABLED  0x00000005
#define DRAGON_XR_ENABLED       0x00000006
#define DRAGON_TERMINATE_ALL	0x00000007
#define DRAGON_TERMINATE_FEW	0x00000008
