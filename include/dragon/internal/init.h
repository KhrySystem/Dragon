#pragma once
#ifdef _cplusplus
extern "C" {
#endif

#include "../message/engine.h"

#include "engine.h"
#include "createInfo.h"

#include "../dragon_core.h"

DGAPI DgBool32 dgCreateEngine(DgEngine* pEngine, DgCreateInfo* pCreateInfo, void((*pCallback)(DgMessage*)), int verbosity);
DGAPI void dgUpdateEngine(DgEngine* pEngine);
DGAPI DgBool32 dgCanEngineBeTerminated(DgEngine* pEngine);
DGAPI void dgTerminateEngine(DgEngine* pEngine);

#ifdef _cplusplus
}
#endif