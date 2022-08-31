#pragma once

#include "dgBaseObj.h"
#include <glm/glm.hpp>

struct Object : public dgBaseObj {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 anchor;
};
