#include <dragon/dragon.hpp>

std::vector<DgVertex> verts = {
	{{0.5, 0.5, 0.0}, {1.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, 1.0},
	{{0.0, 0.5, 0.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, 1.0},
	{{0.0, 0.5, 0.5}, {0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, 1.0}
};

int main(void) {
	DgEngine engine;
	std::shared_ptr<DgEngine> engineRef(&engine);
	DgResult result = dgCreateEngine(engineRef);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(engineRef);
		return result;
	}
	DgWindow window;
	std::shared_ptr<DgWindow> windowRef(&window);
 	result = dgCreateWindow(engineRef, "", 800, 600, DG_TRUE, DG_FALSE, windowRef);
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