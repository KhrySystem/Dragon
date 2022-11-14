#pragma once

#include "../dragon_core.h"
#include "../internal/vector.h"

#include "outputDevice.h"

typedef struct DgAudioEngine {
	DgVector outputDevices;
} DgAudioEngine;
