#include "ir_manager.h"
#include "config.h"

uint32_t IRManager::lastIRCode = 0;
uint8_t IRManager::lastIRBits = 0;
decode_type_t IRManager::lastIRProtocol = UNKNOWN;

void IRManager::initialize() {
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);
    pinMode(SEND_BUTTON_PIN, INPUT_PULLUP);
}

void IRManager::handleReceive() {
    if (IrReceiver.decode()) {
        lastIRCode = IrReceiver.decodedIRData.decodedRawData;
        lastIRBits = IrReceiver.decodedIRData.numberOfBits;
        lastIRProtocol = IrReceiver.decodedIRData.protocol;

        Serial.println();
        Serial.print("IR Code Received: 0x");
        Serial.println(lastIRCode, HEX);
        Serial.print("Protocol: ");
        Serial.println(getProtocolString(lastIRProtocol));

        IrReceiver.resume();
    }
}

void IRManager::handleTransmit() {
    if (digitalRead(SEND_BUTTON_PIN) == LOW) {
        if (lastIRCode != 0) {
            Serial.println("Transmitting last received IR code");
            IrSender.sendNEC(lastIRCode, lastIRBits);
        } else {
            Serial.println("No IR code to transmit");
        }
        delay(BUTTON_DEBOUNCE_DELAY);
    }
}