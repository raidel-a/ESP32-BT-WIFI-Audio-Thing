#include "wifi_manager.h"
#include "ir_manager.h"
#include "bluetooth_manager.h"
#include "button_manager.h"

void setup() {
    Serial.begin(115200);

    WiFiManager::initialize();
    IRManager::initialize();
    BluetoothManager::initialize();
    ButtonManager::initialize();
}

void loop() {
    WiFiManager::checkConnection();
    IRManager::handleReceive();
    IRManager::handleTransmit();
}