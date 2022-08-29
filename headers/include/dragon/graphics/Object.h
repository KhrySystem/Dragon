#pragma once

#include <glm/glm.hpp>
#include <dragon/graphics/dgBaseObj.h>
#include <dragon/DGAPIdef.h>

struct Object : public dgBaseObj {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 anchor;
};

DGAPI Object* dgCreate3DObject(dgEngine* engine, float[] verts);
DGAPI UIElement* dgCreateUIElement(dgEngine* engine, float verts[], bool is2D);
DGAPI void dgMakeElementVisible(dgBaseObj* element);
DGAPI void dgMakeElementInvisible(dgBaseObj* element);