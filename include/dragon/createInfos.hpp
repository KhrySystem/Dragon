#pragma once

#include "dragon_core.hpp"
#include "engine.hpp"
#include "vertex.hpp"

struct DgEngineCreateInfo {
    std::function<void(int, const char*, void*)> pCallback;
    std::vector<const char*> vkExtensions;
    std::vector<const char*> vkDeviceExtensions;
    std::vector<const char*> vkValidationLayers;
};

struct DgModelCreateInfo {
	std::vector<DgVertex> verticies;
};

struct DgWindowCreateInfo {
    std::weak_ptr<DgEngine> pEngine;
    std::string title;
    unsigned int width;
    unsigned int height; 
    DgBool32 isResizable = DG_FALSE; 
    DgBool32 isFullscreen = DG_FALSE;
};