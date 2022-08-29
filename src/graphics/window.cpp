#include <dragon/Dragon.hpp>

DGAPI DGBOOL dgInitWindow(dgEngine* engine, dgWindowCreateParams params) {
	dgWindow newWindow = glfwCreateWindow(params.width, params.height, params.title, params.monitor, NULL);
	if(newWindow != NULL) {
		engine->window = newWindow;
		glfwMakeContextCurrent(engine->window);
		return DGTRUE;
	}
	return DGFALSE;

}

DGAPI DGBOOL dgShouldWindowClose(dgEngine* engine) {
	return glfwWindowShouldClose(engine->window);
}

DGAPI void dgUpdateEngine(dgEngine* engine) {
	
}