#include <Dragon/dragon.h>

int main(void) {
	if(!dgInit()) return 1;
	dgEngine* instance = dgCreateEngine("OpenWindow.cpp");
	WindowCreateParams params;
	params.width = 800;
	params.height = 600;
	params.monitor = NULL;
	params.resizable = DG_FALSE;
	if(!dgInitWindow(instance, params)) { return 2; }
	return 0;
}