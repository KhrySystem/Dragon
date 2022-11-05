#pragma once

namespace Message {
    struct Message {
        DgUInt64 code;
        std::string message;
    };

    struct Engine {
        std::function<void(Message*)> pCallback; 
    };
}


