#pragma once

#include <functional>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/efx-creative.h>
#include <AL/efx-presets.h>
#include <AL/efx.h>

#include <CL/cl.hpp>

#include <glfw/glfw3.h>

#include <openxr/openxr.h>

#include <vulkan/vulkan.hpp>

typedef VkBool32 DgBool32;
#define DG_TRUE GLFW_TRUE
#define DG_FALSE GLFW_FALSE
typedef uint_fast64_t DgUInt64;
typedef int_fast64_t DgInt64;
typedef uint32_t DgUInt32;
typedef int32_t DgInt32;

#ifndef NDEBUG
	const DgBool32 DRAGON_MESSAGE_ENABLED = DG_TRUE;
	#ifndef __DEPRECEATED
		const DgBool32 DRAGON_SEND_DEPRECEATED = DG_FALSE;
	#else
		const DgBool32 DRAGON_SEND_DEPRECEATED = DG_TRUE;
	#endif
	#ifndef __EXCEPTIONS
		const DgBool32 DRAGON_SEND_EXCEPTIONS = DG_FALSE;
	#else 
		const DgBool32 DRAGON_SEND_EXCEPTIONS = DG_TRUE;
	#endif
#else
	const DgBool32 DRAGON_MESSAGE_ENABLED = DG_FALSE;
#endif

#if INTPTR_MAX == INT32_MAX
    #define DG_BUS_32
#elif INTPTR_MAX == INT64_MAX
    #define DG_BUS_64
#else
    #error "Environment bus size unknown."
#endif

#define DGAPI __declspec(dllexport)

#define DRAGON_AUDIO_BUFFER_COUNT 16

#define DRAGON_AUDIO_ENABLED    0x00000000
#define DRAGON_CL_ENABLED       0x00000001
#define DRAGON_GRAPHICS_ENABLED 0x00000002
#define DRAGON_MESSAGE_ENABLED  0x00000003
#define DRAGON_PARTICLE_ENABLED 0x00000004
#define DRAGON_PHYSICS_ENABLED  0x00000005
#define DRAGON_XR_ENABLED       0x00000006
#define DRAGON_TERMINATE_ALL	0x00000007
#define DRAGON_TERMINATE_FEW	0x00000008

#include "predef/core.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	//define something for Windows (32-bit and 64-bit, this part is common)
	#ifndef APIENTRY
		#include <Windows.h>
	#endif
	#define DG_PLAT_WINDOWS
	#ifdef _WIN64
		//define something for Windows (64-bit only)
		#define DG_PLAT_WIN64
	#else
		//define something for Windows (32-bit only)
		#define DG_PLAT_WIN32
	#endif
#elif __APPLE__
	#include <TargetConditionals.h>
	#define DG_PLAT_APPLE
	#if TARGET_IPHONE_SIMULATOR
		// iOS, tvOS, or watchOS Simulator
		#define DG_PLAT_IOS
	#elif TARGET_OS_MACCATALYST
		// Mac's Catalyst (ports iOS API into Mac, like UIKit).
		#define DG_PLAT_CATALYST
	#elif TARGET_OS_IPHONE
		// iOS, tvOS, or watchOS device
		#define DG_PLAT_IOS
	#elif TARGET_OS_MAC
		// Other kinds of Apple platforms
		#import <Cocoa/Cocoa.h>
		#define DG_PLAT_MAC
	#else
		#error "Unknown Apple platform"
	#endif
#elif __ANDROID__
	// Below __linux__ check should be enough to handle Android,
	// but something may be unique to Android.
	#define DG_PLAT_ANDROID
#elif __linux__
	// linux
	#define DG_PLAT_LINUX
#elif __unix__ // all unices not caught above
	// Unix
	#define DG_PLAT_UNIX
#elif defined(_POSIX_VERSION)
	// POSIX
	#define DG_PLAT_POSIX
#else
	#error "Unknown compiler"
#endif

namespace Dragon {
	namespace Audio {
		#include "audio/outputDevice.hpp"
		#include "audio/engine.hpp"
	}
	namespace CL {
		namespace Physics {
			#include "cl/physics/engine.hpp"
		}
		#include "cl/engine.hpp"
	}
	namespace Graphics {
		namespace Particle {
			#include "graphics/particle/engine.hpp"
		}
		namespace XR {
			#include "graphics/xr/engine.hpp"
		}
		#include "graphics/engine.hpp"
	}

	#include "internal/createInfo.hpp"

	namespace Message {
		#include "message/engine.hpp"
	}

	#include "internal/engine.hpp"
	#include "internal/init.hpp"

	namespace Graphics {
		#include "graphics/init.hpp"
	}

	namespace Message {
		#include "message/callbacks.hpp"
	}
}