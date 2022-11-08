#pragma once

#include "../audio/engine.h"
#include "../graphics/engine.h"
#include "../message/engine.h"

typedef struct DgEngine {
	const char* name;
	DgVector windows;

	DgAudioEngine* audio;
	DgGraphicsEngine* graphics;
	DgMessageEngine* message;
} DgEngine;