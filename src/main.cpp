// Include necessary libraries
#include <Arduino.h>
#include "BluetoothA2DPSink.h"
#include "AudioTools.h"
#include <ESPmDNS.h>
#include "credentials.h"
#include <IRremote.hpp>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>

WebServer server(80);

// Define constants
#define IR_RECEIVE_PIN 15
#define PLAY_PAUSE_PIN 12
#define NEXT_PIN 13
#define PREV_PIN 14
#define IR_LED_PIN 4
#define IR_BUTTON_PIN 33

#define WIFI_CHECK_INTERVAL 50000

// Global variables
AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);
// IRrecv irrecv(IR_RECEIVE_PIN);
// IRsend irsend(IR_LED_PIN);
decode_results results;
unsigned long lastWifiCheck = 0;

// Function prototypes
void setupWiFi();
void setupMDNS();
void setupIR();
void setupButtons();
void setupA2DP();
void buttonHandlingTask(void *parameter);
void checkWiFiConnection();
void debugPrint(const String &message, bool newLine = true);

void setup()
{
  Serial.begin(115200);
  setupMDNS();
  setupWiFi();
  setupMDNS();
  setupIR();
  setupButtons();
  setupA2DP();

  ElegantOTA.begin(&server);

  xTaskCreate(buttonHandlingTask, "ButtonTask", 2048, NULL, 1, NULL);
}

void loop()
{
  // checkWiFiConnection();
  server.handleClient();
  ElegantOTA.loop();

  if (IrReceiver.decode())
  {
    if (IrReceiver.decodedIRData.decodedRawData == 0x0 || IrReceiver.decodedIRData.protocol == UNKNOWN)
    {
      debugPrint(".", false);
    }
    else
    {
      debugPrint("");
      debugPrint("IR Code Received: 0x" + String(IrReceiver.decodedIRData.decodedRawData, HEX));
      debugPrint("Protocol: " + String(getProtocolString(IrReceiver.decodedIRData.protocol)));
      debugPrint("Command: " + String(IrReceiver.decodedIRData.command));
      debugPrint("");
    }
    IrReceiver.resume();
  }

  delay(2000);
}

void debugPrint(const String &message, bool newLine)
{
  newLine ? Serial.println(message) : Serial.print(message);
}

void setupWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  debugPrint("Connecting to WiFi...");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    debugPrint(".", false);
  }
  debugPrint("");
  debugPrint("Connected to " + String(WIFI_SSID), false);
  debugPrint("IP address: " + WiFi.localIP().toString(), false);
  debugPrint("");

  server.on("/", []()
            { server.send(200, "text/plain", "Hi! This is ElegantOTA Demo."); });

  ElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void setupMDNS()
{
  if (!MDNS.begin("ESP32-A2DP-Receiver"))
  {
    debugPrint("Error setting up MDNS responder!");
  }
  else
  {
    debugPrint("mDNS responder started");
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ota", "tcp", 80);
  }
}

void setupIR()
{
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    IrSender.begin(IR_LED_PIN, ENABLE_LED_FEEDBACK);
}

void setupButtons()
{
  pinMode(PLAY_PAUSE_PIN, INPUT_PULLUP);
  pinMode(NEXT_PIN, INPUT_PULLUP);
  pinMode(PREV_PIN, INPUT_PULLUP);
  pinMode(IR_BUTTON_PIN, INPUT_PULLUP);
  pinMode(IR_LED_PIN, OUTPUT);
  digitalWrite(IR_LED_PIN, LOW);  // Ensure the transistor is off initially
}

void setupA2DP()
{
  a2dp_sink.start("ESP32 Music Receiver");
  debugPrint("A2DP sink started. Waiting for connection...");
}

void buttonHandlingTask(void *parameter)
{
  const TickType_t xDelay = pdMS_TO_TICKS(50);
  while (1)
  {
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
      }
    }
    if (digitalRead(NEXT_PIN) == LOW)
    {
      a2dp_sink.next();
      while (digitalRead(NEXT_PIN) == LOW)
      {
        vTaskDelay(xDelay);
      }
    }
    if (digitalRead(PREV_PIN) == LOW)
    {
      a2dp_sink.previous();
      while (digitalRead(PREV_PIN) == LOW)
      {
        vTaskDelay(xDelay);
      }
    }

    if (digitalRead(IR_BUTTON_PIN) == LOW)
    {
      static bool isOn = true;
      if (isOn)
      {
        IrSender.sendNEC(0x10ef, 0xfd02, 0); // LED Strip off
        // IrSender.sendNEC(0x10ef, 0x639c, 0); // LED Bulb off
        isOn = false;
        debugPrint("OFF");
      }
      else
      {
        IrSender.sendNEC(0x10ef, 0xfc03, 0); // LED Strip on
        // IrSender.sendNEC(0x10ef, 0x22dd, 0); // LED Bulb on
        isOn = true;
        debugPrint("ON");
      }
      while (digitalRead(IR_BUTTON_PIN) == LOW)
      {
        vTaskDelay(xDelay);
      }
    }
  }
}

void checkWiFiConnection()
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastWifiCheck >= WIFI_CHECK_INTERVAL)
  {
    lastWifiCheck = currentMillis;
    if (WiFi.status() != WL_CONNECTED)
    {
      debugPrint("WiFi connection lost. Reconnecting...");
      WiFi.reconnect();
      debugPrint("");
    }
    else
    {
      debugPrint("WiFi still connected");
      debugPrint("IP address: " + WiFi.localIP().toString());
      debugPrint("");
    }
  }
}