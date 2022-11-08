#pragma once

#include "../predef/predef.h"
#include "particle/engine.h"
#include "xr/engine.h"

typedef struct DgGraphicsEngine {
	VkInstance vkInstance;
	DgVector windows;

	DgGraphicsParticleEngine particle;
	DgGraphicsXREngine xr;
} DgGraphicsEngine;
