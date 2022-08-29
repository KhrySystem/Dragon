#include <Dragon/dragon.hpp>

int main(void) {
	if(!dgInit())
		return 1;
	dgEngine* engine = dgCreateEngine("OpenWindow.cpp");

	dgWindowCreateParams windowParams{};
	windowParams.width = 800;
	windowParams.height = 600;
	windowParams.isBorderless = false;
	windowParams.isFullscreen = false;
	windowParams.title = "OpenWindow.cpp";
	windowParams.monitor = NULL;


	dgInitWindow(engine, windowParams);

	while(!dgShouldWindowClose(engine)) {
		dgUpdateWindow(engine);
	}

	dgDestroyEngine(engine);
}