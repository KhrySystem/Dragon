#include "engine.h"
#include "window.h"

struct DgWindowCreateInfo {
    uint width, height;
    const char* pTitle;
    DgBool32 isFullscreen, isBorderless;
    DgEngine* pEngine;
};
typedef DgWindowCreateInfo DgWindowCreateInfo;

DGAPI DgResult dgCreateWindow(DgWindowCreateInfo createInfo, DgWindow* pWindow);
DGAPI void dgDestroyWindow(DgWindow* pWindow, DgEngine* pEngine);