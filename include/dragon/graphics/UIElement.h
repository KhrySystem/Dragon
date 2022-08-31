#pragma once

#include <dragon/dgEngine.h>
#include <glm/glm.hpp>
#ifndef dgEngine
    struct dgEngine;
#endif
struct UIElement : public dgBaseObj {
    glm::vec2 position;
    glm::vec2 rotation;
    glm::vec2 anchor;
};

UIElement* dgCreateUIElement(dgEngine* engine, float verts[], bool is2D);
