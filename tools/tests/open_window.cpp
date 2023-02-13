#include <dragon/dragon.hpp>

std::vector<DgVertex> verts = {
	{{0.5, 0.5, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.0, 0.5, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{0.0, 0.5, 0.5}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}}
};

int main(void) {
	DgEngine engine;
	DgResult result = dgCreateEngine(&engine);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(&engine);
		return result;
	}
	DgWindow window;
	result = dgCreateWindow(&engine, "", 800, 600, DG_TRUE, DG_FALSE, &window);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(&engine);
		return result;
	}

	result = dgCreateUIElement(&window, verts);

	while (dgGetWindowCount(&engine) > 0) {
		dgUpdate(&engine);
	}

	dgTerminateEngine(&engine);
	return 0;
}