#pragma once

#include <dragon/graphics/WindowCreateParams.h>
#include <dragon/dgEngine.h>
#include <dragon/predef.h>

DGAPI DG_BOOL dgInitWindow(dgEngine* engine, WindowCreateParams params);
DGAPI DG_BOOL dgShouldWindowClose(dgEngine* engine);