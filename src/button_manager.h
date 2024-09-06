#pragma once
#include "BluetoothA2DPSink.h"
#include "AudioTools.h"

class BluetoothManager {
public:
    static void initialize();
    static void handlePlayPause();
    static void handleNext();
    static void handlePrevious();
private:
    static AnalogAudioStream out;
    static BluetoothA2DPSink a2dp_sink;
};