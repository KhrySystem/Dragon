#include <dragon/Dragon.hpp>

DGBOOL DGAPI dgInit() {
	if(!glfwInit()) return DGFALSE;
	return DGTRUE;
}

void DGAPI dgDestroyEngine(dgEngine* engine) {
	vkDestroyInstance(engine->vulkan, nullptr);
	glfwDestroyWindow(engine->window);
	glfwTerminate();
}