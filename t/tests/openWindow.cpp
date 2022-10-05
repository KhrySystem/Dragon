#include <Dragon/dragon.h>

int main(void) {
	if(!dgInit()) return 1;
	dgEngine* instance = dgCreateEngine("OpenWindow");
	WindowCreateParams params;
	params.width = 800;
	params.height = 600;
	params.monitor = NULL;
	params.resizable = DG_FALSE;
	if(!dgInitWindow(instance, params)) { return 2; }

	while(!dgShouldWindowClose(instance)) {
		dgUpdateEngine(instance);
	}
	dgDestroyEngine(instance);
	return 0;
}