#include <dragon/dragon.hpp>

DGAPI DgBool32 dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable) {
	glfwWindowHint(GLFW_RESIZABLE, isResizable);
	GLFWwindow* glfw = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	assert(glfw != NULL);
	DgWindow window;
	window.window = glfw;
	VkResult result = glfwCreateWindowSurface(pEngine->vulkan, window.window, nullptr, &window.surface);

	assert(result == VK_SUCCESS);

	if (pEngine->primaryGPU->presentationQueue == nullptr) {
		_dgGeneratePresentationQueue(pEngine->primaryGPU, &window);
	}


	return DG_TRUE;
}

DGAPI int dgGetWindowCount(DgEngine* pEngine) {
	return pEngine->windows.size();
}

DGAPI void _dgChooseSwapSurfaceFormat(DgWindow* pWindow, const std::vector<VkSurfaceFormatKHR>& pFormats) {
	// Ensure window is not null
	assert(pWindow != nullptr);

	// Loop through all formats to find one that is good
	for (const VkSurfaceFormatKHR format : pFormats) {
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			pWindow->surfaceFormat = format;
			return;
		}
	}
	// return the default format
	pWindow->surfaceFormat = pFormats.at(0);
}

DGAPI void _dgChooseSwapPresentMode(DgWindow* pWindow, const std::vector<VkPresentModeKHR>& pPresentModes) {
	// Ensure window is not null
	assert(pWindow != nullptr);


	// loop through all modes to find one that is good
	for (const VkPresentModeKHR availablePresentMode : pPresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			pWindow->presentMode = availablePresentMode;
			return;
		}
	}
	// return the default mode
	pWindow->presentMode = VK_PRESENT_MODE_FIFO_KHR;
}

DGAPI void _dgChooseSwapExtent2D(DgWindow* pWindow, const VkSurfaceCapabilitiesKHR* pCapabilities) {

}