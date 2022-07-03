#ifndef INTERNAL_HPP
	#define INTERNAL_HPP

	#if defined(DRAGON_BUILD_DLL) && defined(_WIN32)
 		#define DGAPI __declspec(dllexport)
	#elif defined(DRAGON_BUILD_DLL) && defined(__GNUC__)
 		#define DGAPI __attribute__((visibility("default")))
	#else
		#define DGAPI
	#endif

	// allows for time unit override
	#ifndef DG_TIME_UNIT
		/*
		Optional time unit override. Disabled on dgAsyncWindow
		*/
		#define DG_TIME_UNIT std::chrono::nanoseconds
	#endif
#endif