#pragma once

#include <glm/glm.hpp>
#include <dragon/graphics/dgBaseObj.h>

struct UIElement : public dgBaseObj
{
    glm::vec2 position;
    glm::vec2 rotation;
    glm::vec2 anchor;
};
