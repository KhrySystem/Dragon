#include <dragon/dragon.hpp>

typedef struct dgInstance {
	GLFWwindow** windows;
} dgInstance;

dgInstance DGAPI dgCreateInstance_internal() {
	dgInstance i = dgInstance();
	return i;
}