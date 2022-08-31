#include <Dragon/dragon.h>

int main(void) {
	if(!dgInit()) return 1;
	dgEngine* instance = dgCreateEngine();
	return 0;
}