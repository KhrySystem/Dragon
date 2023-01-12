#pragma once

#include <string>

#include "dragon_core.h"

typedef struct {
	void* pEngine;
	int code;
	std::string message;
	void* pCallbackData;
} DgMessage;