#include <dragon/dragon.hpp>

DGAPI int dgGetWindowCount(boost::shared_ptr<DgEngine> pEngine) {
	if (pEngine == nullptr)
		return -1;
	return pEngine->windows.size();
}

DGAPI DgResult dgUpdate(boost::shared_ptr<DgEngine> pEngine) {
	if (pEngine == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}
	glfwPollEvents();
	for (int i = 0; i < pEngine->windows.size(); i++) {
		boost::shared_ptr<DgWindow>  pWindow = pEngine->windows[i];

		DgResult r = _dgRenderWindow(pWindow);
		if (r != DG_SUCCESS) {
			_dgDestroyWindow(pEngine->vulkan, pWindow);
			pEngine->windows.erase(pEngine->windows.begin() + i);
			return r;
		}

		if (glfwWindowShouldClose(pWindow->window)) {
			_dgDestroyWindow(pEngine->vulkan, pWindow);
			pEngine->windows.erase(pEngine->windows.begin() + i);
		}
	}

	return DG_SUCCESS;
}