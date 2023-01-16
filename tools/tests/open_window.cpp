#include <dragon/dragon.hpp>

int main(void) {
	DgEngine e;
	DgResult result = dgCreateEngine(&e);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(&e);
		return 1;
	}
	result = dgCreateWindow(&e, "Not Resizable", 800, 600, DG_FALSE);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(&e);
		return 2;
	}

	while (dgGetWindowCount(&e) > 0) {
		dgUpdate(&e);
	}

	dgTerminateEngine(&e);
	return 0;
}