#include <dragon/dragon.hpp>

int main(void) {
	DgEngine engine;
	dgCreateEngine(&engine);
	dgCreateWindow(&engine, "Win", 300, 300, false);
	dgUpdate(&engine);
	dgTerminateEngine(&engine);
}