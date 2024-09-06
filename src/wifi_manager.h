#pragma once
#include <WiFi.h>
#include "credentials.h"

class WiFiManager {
public:
    static void initialize();
    static void checkConnection();
private:
    static unsigned long lastCheckTime;
};