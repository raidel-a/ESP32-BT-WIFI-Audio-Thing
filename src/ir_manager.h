#pragma once
#include <IRremote.hpp>

class IRManager {
public:
    static void initialize();
    static void handleReceive();
    static void handleTransmit();
private:
    static uint32_t lastIRCode;
    static uint8_t lastIRBits;
    static decode_type_t lastIRProtocol;
};