#include <dragon/dragon.hpp>

int main(void) {
	DgEngine e;
	std::cout << "dgCreateEngine" << std::endl;
	DgResult result = dgCreateEngine(&e);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(&e);
		return result;
	}
	std::cout << "dgCreateWindow" << std::endl;
	result = dgCreateWindow(&e, "Not Resizable", 800, 600, DG_FALSE);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(&e);
		return result;
	}

	std::cout << "dgGetWindowCount" << std::endl;
	while (dgGetWindowCount(&e) > 0) {
		dgUpdate(&e);
	}

	dgTerminateEngine(&e);
	return 0;
}