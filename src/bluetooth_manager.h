#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class ButtonManager {
public:
    static void initialize();
    static void buttonHandlingTask(void* parameter);
};