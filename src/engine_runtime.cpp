#include <dragon/dragon.h>
#include <iostream>

DGAPI DgBool32 dgCanEngineClose(DgEngine* pEngine) {
    for(int i = 0; i < pEngine->windowCount; i++) {
        if(!glfwWindowShouldClose(pEngine->pWindows[i]->pWindow)) {
            return DG_FALSE;
        }
    }
    return DG_TRUE;
}

DGAPI DgResult dgUpdateEngine(DgEngine* pEngine) {
    glfwPollEvents();
    return DG_SUCCESS;
}