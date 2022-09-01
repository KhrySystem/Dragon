#include <dragon/Dragon.h>

DGAPI DG_BOOL dgInitWindow(dgEngine* engine, WindowCreateParams params) {
    glfwWindowHint(GLFW_RESIZABLE, params.resizable);
    if(engine->shared != DG_NULL) 
        engine->window = glfwCreateWindow(params.width, params.height,
            engine->appName, params.monitor, engine->shared->window
        );
    else
        engine->window = glfwCreateWindow(params.width, params.height,
            engine->appName, params.monitor, DG_NULL
        );

    if(engine->window == DG_NULL) return DG_FALSE;
    return DG_TRUE;
}

DGAPI DG_BOOL dgShouldWindowClose(dgEngine* engine) {
    glfwPollEvents();
    return glfwWindowShouldClose(engine->window);
}