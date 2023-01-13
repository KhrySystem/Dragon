#include <dragon/dragon.hpp>

int main(void) {
	DgEngine e;
	if (!dgCreateEngine(&e)) {
		return 1;
	}
	if (!dgCreateWindow(&e, "Not Resizable", 800, 600, DG_FALSE)) {
		return 2;
	}

	while (dgGetWindowCount(&e) > 0) {
		dgUpdate(&e);
	}

	dgTerminateEngine(&e);
	return 0;
}