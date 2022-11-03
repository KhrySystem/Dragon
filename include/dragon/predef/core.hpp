#pragma once

#ifdef VK_MAKE_API_VERSION
	#define DRAGON_VERSION VK_MAKE_API_VERSION(1, 0, 0, 0)
#else
	#define DRAGON_VERSION VK_MAKE_VERSION(1, 0, 0)
#endif
