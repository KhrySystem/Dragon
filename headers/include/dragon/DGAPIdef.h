#pragma once

#ifdef DRAGON_STATIC
	#define DGAPI
#else
	#define DGAPI __declspec(dllexport)
#endif
