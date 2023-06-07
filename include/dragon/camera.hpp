#pragma once

#include "dragon_core.hpp"

struct DgCamera_T {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

typedef struct DgCamera_T DgCamera;