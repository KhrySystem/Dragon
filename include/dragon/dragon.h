#pragma once
#ifdef _cplusplus
#include "dragon.hpp"
extern "C" {
#endif

#include "dragon_core.h"

#include "audio/engine.h"
#include "audio/outputDevice.h"
#include "compute/engine.h"
#include "graphics/engine.h"
#include "graphics/init.h"
#include "internal/createInfo.h"
#include "internal/engine.h"
#include "internal/gpu.h"
#include "internal/init.h"
#include "internal/vector.h"
#include "message/callbacks.h"
#include "message/engine.h"
#include "particle/engine.h"
#include "physics/engine.h"
#include "raycast/engine.h"
#include "xr/device.h"
#include "xr/engine.h"

#ifdef _cplusplus
}; // dragon/dragon.h
#endif