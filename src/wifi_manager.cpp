#include "wifi_manager.h"
#include "config.h"

unsigned long WiFiManager::lastCheckTime = 0;

void WiFiManager::initialize() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void WiFiManager::checkConnection() {
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime >= WIFI_CHECK_INTERVAL) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi connection lost. Reconnecting...");
            WiFi.reconnect();
        }
        lastCheckTime = currentTime;
    }
}