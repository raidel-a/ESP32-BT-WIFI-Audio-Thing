#include "button_manager.h"
#include "config.h"
#include "bluetooth_manager.h"

void ButtonManager::initialize() {
    pinMode(PLAY_PAUSE_PIN, INPUT_PULLUP);
    pinMode(NEXT_PIN, INPUT_PULLUP);
    pinMode(PREV_PIN, INPUT_PULLUP);

    xTaskCreate(
        buttonHandlingTask,
        "ButtonTask",
        2048,
        NULL,
        1,
        NULL
    );
}

void ButtonManager::buttonHandlingTask(void* parameter) {
    const TickType_t xDelay = pdMS_TO_TICKS(50);

    while (1) {
        if (digitalRead(PLAY_PAUSE_PIN) == LOW) {
            BluetoothManager::handlePlayPause();
            while (digitalRead(PLAY_PAUSE_PIN) == LOW) {
                vTaskDelay(xDelay);
            }
        }

        if (digitalRead(NEXT_PIN) == LOW) {
            BluetoothManager::handleNext();
            while (digitalRead(NEXT_PIN) == LOW) {
                vTaskDelay(xDelay);
            }
        }

        if (digitalRead(PREV_PIN) == LOW) {
            BluetoothManager::handlePrevious();
            while (digitalRead(PREV_PIN) == LOW) {
                vTaskDelay(xDelay);
            }
        }

        vTaskDelay(xDelay);
    }
}