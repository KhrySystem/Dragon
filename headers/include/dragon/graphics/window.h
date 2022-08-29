#pragma once 

#include <Dragon/dragon.h>

DGAPI dgEngine* dgCreateEngine(const char* appName);
DGAPI DGBOOL dgInitWindow(dgEngine* engine, dgWindowCreateParams params);
DGAPI void dgUpdateWindow(dgEngine* engine);
DGAPI DGBOOL dgShouldWindowClose(dgEngine* engine);