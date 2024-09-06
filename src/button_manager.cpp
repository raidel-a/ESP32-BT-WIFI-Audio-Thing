#include "bluetooth_manager.h"

AnalogAudioStream BluetoothManager::out;
BluetoothA2DPSink BluetoothManager::a2dp_sink(out);

void BluetoothManager::initialize() {
    a2dp_sink.start("ESP32 Music Receiver");
    Serial.println("A2DP sink started. Waiting for connection...");

    delay(9000);
    if (a2dp_sink.is_connected()) {
        Serial.println("A2DP sink connected successfully.");
    } else {
        Serial.println("A2DP sink not connected. Check your Bluetooth device.");
    }
}

void BluetoothManager::handlePlayPause() {
    static bool isPlaying = true;
    if (isPlaying) {
        a2dp_sink.pause();
        isPlaying = false;
    } else {
        a2dp_sink.play();
        isPlaying = true;
    }
}

void BluetoothManager::handleNext() {
    a2dp_sink.next();
}

void BluetoothManager::handlePrevious() {
    a2dp_sink.previous();
}