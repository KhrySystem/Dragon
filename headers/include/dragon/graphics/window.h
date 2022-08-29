#pragma once 

#include <dragon/DGAPIdef.h>

DGAPI dgEngine* dgCreateEngine(const char* appName);
DGAPI DGBOOL dgInitWindow(dgEngine* engine, dgWindowCreateParams params);
DGAPI void dgUpdateEngine(dgEngine* engine);
DGAPI DGBOOL dgShouldWindowClose(dgEngine* engine);