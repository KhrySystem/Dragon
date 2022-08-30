#pragma once
#ifdef _cplusplus
extern "C" {
#endif


#ifdef DRAGON_USE_VULKAN
	#define GLFW_INCLUDE_VULKAN
	#include <vulkan/vulkan.hpp>
#endif

#ifdef DRAGON_USE_OPENGL
	#define GLFW_INCLUDE_OPENGL
	#ifdef DRAGON_GL_GLX_FOUND
	#endif
	#ifdef DRAGON_GL_EGL_FOUND
	#endif
#endif

#ifndef DRAGON_MAXIMUM_PROCESSES
	#if DRAGON_CXX_STANDARD >= 11
		#include <thread>
		const unsigned int DRAGON_MAXIMUM_PROCESSES = std::thread::hardware_concurrency() - 1;
	#else
		#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

		#elif __APPLE__
			#include <TargetConditionals.h>
			#if TARGET_IPHONE_SIMULATOR
				// iOS, tvOS, or watchOS Simulator
			#elif TARGET_OS_MACCATALYST
				// Mac's Catalyst (ports iOS API into Mac, like UIKit).
			#elif TARGET_OS_IPHONE
				// iOS, tvOS, or watchOS device
			#elif TARGET_OS_MAC

			#else
				#error "Unknown Apple platform"
			#endif
		#elif __ANDROID__
			// Below __linux__ check should be enough to handle Android,
			// but something may be unique to Android.
		#elif __linux__
			// linux
		#elif __unix__ // all unices not caught above
			// Unix
		#elif defined(_POSIX_VERSION)
			// POSIX
		#else
			#error "Unknown compiler"
		#endif
	#endif
#endif

#include <dragon/dg_icl.h>
#include <glfw/glfw3.h>

template<typename T> int len(T* obj) {
	return (int)(sizeof(obj)/sizeof(obj[1]));
}

#define DGBOOL bool
#define DGTRUE 1
#define DGFALSE 0


DGAPI DGBOOL dgInit();
DGAPI void dgDestroyEngine(dgEngine* engine);

#define DRAGON_STATIC_OBJECT GL_STATIC_DRAW
#define DRAGON_DYNAMIC_OBJECT GL_DYNAMIC_DRAW
#define DRAGON_STREAM_OBJECT GL_STREAM_DRAW

#ifdef _cplusplus
}
#endif