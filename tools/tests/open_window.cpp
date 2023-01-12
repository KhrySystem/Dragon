#include <dragon/dragon.hpp>

int main(void) {
	DgEngine e;
	dgAddLayerToEngine(&e, "VK_LAYER_KHRONOS_validation");
	dgAddLayerToEngine(&e, "VK_LAYER_LUNARG_monitor");
	if (!dgCreateEngine(&e)) {
		return 1;
	}
	dgCreateWindow(&e, "Not Resizable", 800, 600, DG_FALSE);

	while (dgGetWindowCount(&e) > 0) {
		dgUpdate(&e);
	}

	dgTerminateEngine(&e);
	return 0;
}