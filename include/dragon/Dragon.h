#pragma once

#ifdef _cplusplus
	#include <dragon/BoostInclude.hpp>
	extern "C" {
#endif

#include <dragon/predef.h>
DGAPI DG_BOOL dgInit();

#include <dragon/dgEngine.h>
DGAPI void dgPrintVersionInfo(dgEngine* e);
DGAPI void dgDestroyEngine(dgEngine* e);

#ifdef _cplusplus
}
#endif