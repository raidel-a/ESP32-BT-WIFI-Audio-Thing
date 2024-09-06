#include "BluetoothA2DPSink.h"
#include "AudioTools.h"
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define DISABLE_SEND_PWM_BY_TIMER
#include <IRremote.hpp>

const int IR_RECEIVE_PIN = 15;
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const int PLAY_PAUSE_PIN = 12;
const int NEXT_PIN = 13;
const int PREV_PIN = 14;

AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);

void buttonHandlingTask(void *parameter)
{
  const TickType_t xDelay = pdMS_TO_TICKS(50); // 50ms delay between checks

  while (1)
  {
    // Check Play/Pause button
    if (digitalRead(PLAY_PAUSE_PIN) == LOW)
    {
      static bool isPlaying = true;
      if (isPlaying)
      {
        a2dp_sink.pause();
        isPlaying = false;
      }
      else
      {
        a2dp_sink.play();
        isPlaying = true;
      }
      while (digitalRead(PLAY_PAUSE_PIN) == LOW)
      {
        vTaskDelay(xDelay);
      } // Wait for button release
    }

    // Check Next button
    if (digitalRead(NEXT_PIN) == LOW)
    {
      a2dp_sink.next();
      while (digitalRead(NEXT_PIN) == LOW)
      {
        vTaskDelay(xDelay);
      } // Wait for button release
    }

    // Check Previous button
    if (digitalRead(PREV_PIN) == LOW)
    {
      a2dp_sink.previous();
      while (digitalRead(PREV_PIN) == LOW)
      {
        vTaskDelay(xDelay);
      } // Wait for button release
    }

    vTaskDelay(xDelay);
  }
}

void setup()
{
  Serial.begin(115200);
  
  // Start the IR receiver
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup button pins
  pinMode(PLAY_PAUSE_PIN, INPUT_PULLUP);
  pinMode(NEXT_PIN, INPUT_PULLUP);
  pinMode(PREV_PIN, INPUT_PULLUP);

  // Start the Bluetooth A2DP sink
  a2dp_sink.start("ESP32 Music Receiver");
  Serial.println("A2DP sink started. Waiting for connection...");

  // Wait a bit and then check if we're connected
  delay(9000);
  if (a2dp_sink.is_connected())
  {
    Serial.println("A2DP sink connected successfully.");
  }
  else
  {
    Serial.println("A2DP sink not connected. Check your Bluetooth device.");
  }

  // Create button handling task
  xTaskCreate(
      buttonHandlingTask, // Function that implements the task
      "ButtonTask",       // Text name for the task
      2048,               // Stack size in words
      NULL,               // Task input parameter
      1,                  // Priority of the task
      NULL                // Task handle
  );
}

void loop()
{

// if a code is received, print it to the serial port
if (IrReceiver.decode()) {
  Serial.println();
  Serial.print("IR Code Received: 0x");
  Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
  Serial.print("Protocol: ");
  Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));
  Serial.print("Command: ");
  Serial.println(IrReceiver.decodedIRData.command);
  IrReceiver.resume();
}

  // Main loop can be empty as we're using interrupts for button handling
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nReconnected to WiFi");
    Serial.print("New IP address: ");
    Serial.println(WiFi.localIP());
  }
  delay(10000); // Check every 10 seconds
}
