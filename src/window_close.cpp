#include <dragon/dragon.h>

DGAPI void dgDestroyWindow(DgWindow* pWindow, DgEngine* pEngine) {
    glfwDestroyWindow(pWindow->pWindow);

    free(pWindow);
}