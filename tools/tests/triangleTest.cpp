#include <dragon/Dragon.hpp>

float triVerts2D[] = {
    -0.5f, -0.5f, 
     0.5f, -0.5f, 
     0.0f,  0.5f
};

int main(void) {
    if(!dgInit())
		return 1;
	dgEngine* engine = dgCreateEngine("TriangleTest.cpp");

	dgWindowCreateParams windowParams{};
	windowParams.width = 800;
	windowParams.height = 600;
	windowParams.isBorderless = false;
	windowParams.isFullscreen = false;
	windowParams.title = "TriangleTest.cpp";
	windowParams.monitor = NULL;

    Object* object = dgCreateUIElement(engine, triVerts2D, true, DRAGON_STATIC_OBJECT);

	dgInitWindow(engine, windowParams);

	while(!dgShouldWindowClose(engine)) {
		dgUpdateEngine(engine);
	}

	dgDestroyEngine(engine);
}