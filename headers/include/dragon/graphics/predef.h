#pragma once

#if defined(DRAGON_USE_VULKAN) && defined(DRAGON_USE_OPENGL)
	#define DRAGON_USE_VULKAN_GRAPHICS
#elif defined(DRAGON_USE_VULKAN) && !defined(DRAGON_USE_OPENGL)
	#define DRAGON_USE_VULKAN_GRAPHICS
#elif !defined(DRAGON_USE_VULKAN) && defined(DRAGON_USE_OPENGL)
	#define DRAGON_USE_OPENGL_GRAPHICS
#else
	#error "Neither Vulkan nor OpenGL was found on the system. Failing."
#endif