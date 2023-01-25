#include <dragon/dragon.hpp>

int main(void) {
	DgEngine e;
	DgResult result = dgCreateEngine(&e);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(&e);
		return result;
	}
	result = dgCreateWindow(&e, "", 800, 600, DG_TRUE, DG_TRUE);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(&e);
		return result;
	}

	while (dgGetWindowCount(&e) > 0) {
		dgUpdate(&e);
	}

	dgTerminateEngine(&e);
	return 0;
}