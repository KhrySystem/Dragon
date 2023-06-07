#pragma once

#include "dragon_core.hpp"
#include "model.hpp"
#include "window.hpp"

struct DgModelCreateInfo_T {
    std::vector<DgVertex> vertices;
    std::vector<uint32_t> indices;
    std::shared_ptr<DgWindow> pWindow;
    size_t renderLayer;
};

typedef struct DgModelCreateInfo_T DgModelCreateInfo;

DGAPI DgResult dgCreateModel(DgModelCreateInfo createInfo, std::shared_ptr<DgModel> pModel);
DGAPI void dgDestroyModel(std::shared_ptr<DgModel> pModel, std::shared_ptr<DgWindow> pWindow);