#include <dragon/dragon.hpp>

int main(void) {
	DgEngine engine;
	if (!dgCreateEngine(&engine)) {
		std::cerr << "dgCreateEngine failed" << std::endl;
		return 1;
	}
		
	if (!dgCreateWindow(&engine, "Win", 300, 300, false)) {
		std::cerr << "dgCreateWindow failed" << std::endl;
		return 2;
	}

	dgUpdate(&engine);
	dgTerminateEngine(&engine);
	return 0;
}