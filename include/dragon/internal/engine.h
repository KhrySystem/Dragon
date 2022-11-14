#pragma once

#include "../audio/engine.h"
#include "../graphics/engine.h"
#include "../message/engine.h"

typedef struct DgEngine_T {
	const char* name;

	VkInstance vkInstance;

	DgAudioEngine* audio;
	DgGraphicsEngine* graphics;
	DgMessageEngine* message;
} DgEngine;