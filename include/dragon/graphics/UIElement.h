#pragma once

#include "dgBaseObj.h"
#include <glm/glm.hpp>

struct UIElement : public dgBaseObj {
    glm::vec2 position;
    glm::vec2 rotation;
    glm::vec2 anchor;
};
