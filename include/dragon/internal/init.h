#pragma once 

#include "../message/engine.h"

#include "engine.h"
#include "createInfo.h"

#include "../predef/predef.h"

DGAPI DgBool32 dgCreateEngine(DgEngine* pEngine, DgCreateInfo* pCreateInfo, void((*pCallback)(DgMessage*)), int verbosity);
DGAPI void dgUpdateEngine(DgEngine* pEngine);
DGAPI DgBool32 dgCanEngineBeTerminated(DgEngine* pEngine);
DGAPI void dgTerminateEngine(DgEngine* pEngine);