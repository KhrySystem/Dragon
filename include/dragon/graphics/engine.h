#pragma once

#include "../dragon_core.h"
#include "particle/engine.h"
#include "xr/engine.h"

typedef struct DgGraphicsEngine_T {
	DgVector windows;

	DgGraphicsParticleEngine particle;
	DgGraphicsXREngine xr;
} DgGraphicsEngine;
