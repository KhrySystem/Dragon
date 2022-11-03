#pragma once

#include <dragon/dragon.hpp>

using namespace Dragon;

void mCallback(Message::Message* pMessage) {
    printf("Error:\n\tCode:%I64i\n\tMessage:%s",  pMessage->code, pMessage->message.c_str());
}