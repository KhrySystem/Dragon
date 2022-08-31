#include <dragon/Dragon.h>

DGAPI DG_BOOL dgInitWindow(dgEngine* engine, WindowCreateParams params) {
    glfwWindowHint(GLFW_RESIZABLE, params.resizable);
    if(engine->shared != nullptr) 
        engine->window = glfwCreateWindow(params.width, params.height
            engine->appName, params.monitor, engine->shared->window;
        )
    else
        engine->window = glfwCreateWindow(params.width, params.height
            engine->appName, params.monitor, NULL
        )
}