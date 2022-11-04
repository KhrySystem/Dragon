#pragma once

struct CreateInfo {
    int8_t verbosity = 0;
    uint8_t refreshRate;
    std::string name;
    DgBool32 xrEnabled;
};