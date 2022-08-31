#pragma once

#ifdef _cplusplus
#include <Dragon/BoostInclude.hpp>
extern "C" {
#endif

#include "predef.h"

DGAPI DG_BOOL dgInit();
#include "dgEngine.h"
DGAPI void dgDestroyEngine();

#ifdef _cplusplus
}
#endif