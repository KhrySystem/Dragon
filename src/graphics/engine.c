#include <dragon/dragon.h>

DGAPI DgBool32 dgCreateWindow(DgEngine* pEngine, uint8_t width, uint8_t height, const char* title) {
    GLFWwindow* window;
    if(VECTOR_TOTAL(pEngine->windows) != 0) {
        window = glfwCreateWindow(width, height, title, NULL, (GLFWwindow*)dgVectorGet(&pEngine->windows, 0));
    } else {
        window = glfwCreateWindow(width, height, title, NULL, NULL);
    }
    if(window != NULL) {
        VECTOR_ADD(pEngine->windows, window);
        return DG_TRUE;
    } 
    
    DgMessage message;
    message.code = 0x8F1200000000FFFF + (width << 24) + (height << 16);
    message.message = "%s%i%s%i%s%i%s";
    sprintf(message.message, "GLFWwindow* creation failed when called with (", width, ", ", height, ", ", title, ")");
    dgSendMessage(pEngine, &message);
    return DG_FALSE;
}