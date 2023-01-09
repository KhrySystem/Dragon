#include <dragon/dragon.hpp>

DGAPI DgBool32 dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable) {
	glfwWindowHint(GLFW_RESIZABLE, isResizable);
	GLFWwindow* glfw = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	assert(glfw != NULL);
	DgWindow window;
	window.window = glfw;

	if (glfwCreateWindowSurface(pEngine->vulkan, window.window, nullptr, &window.surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	pEngine->windows.push_back(window);
	return DG_TRUE;
}

DGAPI int dgGetWindowCount(DgEngine* pEngine) {
	return pEngine->windows.size();
}