# ESP32 Music Receiver

This project implements a music receiver using an ESP32 microcontroller. It combines WiFi, Bluetooth A2DP, IR remote control, and physical button functionality to create a versatile audio device.

## Features

- WiFi connectivity for network-based features
- Bluetooth A2DP sink for wireless audio streaming
- IR remote control support for device control
- Physical buttons for play/pause, next, and previous track control
- FreeRTOS task for responsive button handling

## Hardware Requirements

- ESP32 development board
- IR receiver (e.g., TSOP38238)
- IR transmitter (optional)
- Push buttons (3x)
- Audio output device (e.g., I2S DAC or amplifier)

## Software Dependencies

- Arduino core for ESP32
- ESP32-A2DP library
- Arduino-audio-tools library
- IRremote library

## Setup

1. Clone this repository
2. Open the project in PlatformIO
3. Copy `credentials_example.h` to `credentials.h` and update with your WiFi credentials
4. Update `config.h` with your specific pin assignments and other settings
5. Build and upload the project to your ESP32

## Usage

- Power on the device and wait for it to connect to WiFi
- Pair your Bluetooth device with "ESP32 Music Receiver"
- Use the IR remote or physical buttons to control playback
- The device will automatically reconnect to WiFi if the connection is lost

## File Structure

- `main.cpp`: Main program logic
- `config.h`: Configuration settings
- `credentials.h`: WiFi credentials (not tracked in git)
- `wifi_manager.h/cpp`: WiFi connection management
- `bluetooth_manager.h/cpp`: Bluetooth A2DP functionality
- `ir_manager.h/cpp`: IR remote control handling
- `button_manager.h/cpp`: Physical button handling

## Contributing

Contributions to this project are welcome. Please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License - see the LICENSE file for details.