#pragma once

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