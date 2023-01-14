#include <dragon/dragon.hpp>

DGAPI DgBool32 dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable) {
	glfwWindowHint(GLFW_RESIZABLE, isResizable);
	GLFWwindow* glfw = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	assert(glfw != NULL);
	DgWindow window;
	window.window = glfw;
	VkResult result = glfwCreateWindowSurface(pEngine->vulkan, window.window, nullptr, &window.surface);
	if(pEngine->primaryGPU->presentationQueue == nullptr)
		dgGeneratePresentationQueue(pEngine->primaryGPU, &window);
	assert(result == VK_SUCCESS);
	return DG_TRUE;
}

DGAPI int dgGetWindowCount(DgEngine* pEngine) {
	return pEngine->windows.size();
}