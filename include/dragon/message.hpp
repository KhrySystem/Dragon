#pragma once

#include <string>

#include "dragon_core.h"

/**
 * @struct DgMessage
 * @brief Structure that holds a debug message from the engine
 */
typedef struct {
    /**
     * @brief Code of the message
     */
    int code;
    /**
     * @brief Message string
     */
    std::string message;
    /**
     * @brief Additional data for the message
     */
    void* pCallbackData;
} DgMessage;