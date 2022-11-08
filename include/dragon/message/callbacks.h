#pragma once

#include "../internal/engine.h"

#include "engine.h"

DGAPI const char* dgVkResultAsString(VkResult result);
DGAPI void dgSendMessage(DgEngine* pEngine, DgMessage* pMessage);


