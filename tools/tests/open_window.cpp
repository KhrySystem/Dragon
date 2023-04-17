#include <dragon/dragon.hpp>

std::vector<DgVertex> verts = {
	{{0.5, 0.5, 0.0}, {1.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, 1.0},
	{{0.0, 0.5, 0.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, 1.0},
	{{0.0, 0.5, 0.5}, {0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, 1.0}
};

int main(void) {
	DgEngine engine;
	std::shared_ptr<DgEngine> engineRef(&engine);
	DgEngineCreateInfo engineCreateInfo{};

	DgResult result = dgCreateEngine(engineRef, engineCreateInfo);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(engineRef);
		return result;
	}
	DgWindow window;
	std::shared_ptr<DgWindow> windowRef(&window);
	DgWindowCreateInfo windowCreateInfo{};
	windowCreateInfo.pEngine = engineRef;
	windowCreateInfo.title = "Open Window";
	windowCreateInfo.width = 600;
	windowCreateInfo.height = 400;
 	result = dgCreateWindow(windowRef, windowCreateInfo);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(engineRef);
		return result;
	}

	dgAddRenderLayer(windowRef);
	DgModel model;
	std::shared_ptr<DgModel> modelRef(&model);
	result = dgCreateModel(windowRef, 0, verts, modelRef);

	while (dgGetWindowCount(engineRef) > 0) {
		dgUpdate(engineRef);
	}

	std::cout << engineRef.use_count() << std::endl;

	dgTerminateEngine(engineRef);
	engineRef.reset();
	windowRef.reset();
	modelRef.reset();
}