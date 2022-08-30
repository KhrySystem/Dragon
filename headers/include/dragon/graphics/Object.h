#pragma once

#include <glm/glm.hpp>
#include <dragon/dgEngine.h>

struct Object : public dgBaseObj {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 anchor;
};

Object* dgCreate3DObject(dgEngine* engine, float verts[]);
DGAPI void dgMakeElementVisible(dgBaseObj* element);
DGAPI void dgMakeElementInvisible(dgBaseObj* element);