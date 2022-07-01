#ifndef INTERNAL_HPP
	#define INTERNAL_HPP

	#if defined(DRAGON_BUILD_DLL) && defined(_WIN32)
 		#define DGAPI __declspec(dllexport)
	#elif defined(DRAGON_BUILD_DLL) && defined(__GNUC__)
 		#define DGAPI __attribute__((visibility("default")))
	#else
		#define DGAPI
	#endif
#endif