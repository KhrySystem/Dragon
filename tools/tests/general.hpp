#pragma once

#include <dragon/dragon.hpp>

using namespace Dragon;

void mCallback(Message::Message* pMessage) {
    printf("Message Recieved:\n\tEngine:\t\t%s\n\tCode:\t\t%llx\n\tMessage:\t%s", pMessage->engineName.c_str(), pMessage->code, pMessage->message.c_str());
}