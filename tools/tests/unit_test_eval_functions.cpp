#include <dragon/dragon.hpp>

int main(void) {
	DgEngine engine;
	if (!dgCreateEngine(&engine)) {
		std::cerr << "dgCreateEngine failed" << std::endl;
		return 1;
	}
	std::cout << "dgCreateEngine successful" << std::endl;
		
	if (!dgCreateWindow(&engine, "Win", 300, 300, false)) {
		std::cerr << "dgCreateWindow failed" << std::endl;
		return 2;
	}
	std::cout << "dgCreateWindow successful" << std::endl;

	dgUpdate(&engine);
	std::cout << "dgUpdate successful" << std::endl;
	dgTerminateEngine(&engine);
	std::cout << "dgTerminateEngine successful" << std::endl;
	return 0;
}