#pragma once

#ifdef _cplusplus
#include <dragon/BoostInclude.hpp>
extern "C" {
#endif

#include <dragon/predef.h>

DGAPI DG_BOOL dgInit();
#include "dgEngine.h"
DGAPI void dgDestroyEngine();

#ifdef _cplusplus
}
#endif