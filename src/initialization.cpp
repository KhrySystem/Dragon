#include <dragon/Dragon.hpp>

DGBOOL DGAPI dgInit_internal() {
	if(!glfwInit()) return DGFALSE;
	return DGTRUE;
}