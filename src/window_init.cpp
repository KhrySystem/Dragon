#include <dragon/dragon.h>

DGAPI DgResult dgCreateWindow(DgWindowCreateInfo createInfo, DgWindow* pWindow) {
    if(createInfo.pEngine == nullptr) return DG_ARGUMENT_INVALID;
    if(pWindow == nullptr) pWindow = (DgWindow*)malloc(sizeof(DgWindow));
    createInfo.pEngine->windowCount++;
    createInfo.pEngine->pWindows = (DgWindow**)realloc(createInfo.pEngine->pWindows, (createInfo.pEngine->windowCount) * sizeof(DgWindow*));

    pWindow->pWindow = glfwCreateWindow(createInfo.width, createInfo.height, createInfo.pTitle, NULL, NULL);
    createInfo.pEngine->pWindows[createInfo.pEngine->windowCount - 1] = pWindow;
    return DG_SUCCESS;
}