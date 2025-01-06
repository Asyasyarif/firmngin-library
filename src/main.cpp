#include <Arduino.h>
#include "Firmngin.h"

#define SSID "your_ssid"
#define PASSWORD "your_password"

Firmngin myDevice("DEVICE_KEY_123", "API_KEY_456");

// Option 1: Simple callback
void handleData(String key, String value)
{
  Serial.println("Key: " + key);
  Serial.println("Value: " + value);

  // Handle all pin events here
  if (key.startsWith("pin_"))
  {
    int pin = key.substring(4).toInt();
    int val = value.toInt();

    if (pin == 40)
    { // LED pin
      Serial.println("LED value changed to: " + String(val));
    }
  }
}

// Option 2: JSON callback
void handleDataWithJson(String key, String value, String rawData)
{
  Serial.println("Key: " + key);
  Serial.println("Value: " + value);
  Serial.println("Raw JSON: " + rawData);
}

void setup()
{
  // Add output and input pin with custom attributes
  myDevice
      .setWiFi(SSID, PASSWORD) // Set WiFi credentials
      .enableAutoReconnect()
      .setIntervalHeartbeat(10000)
      .enableOTA()
      .noDebug()
      .setOTAPassword("secret123")
      .onOTAProgress([](int progress)
                     { Serial.printf("Update Progress: %d%%\n", progress); })
      .onWiFiConnected([]()
                       { Serial.println("WiFi Connected! IP: " + WiFi.localIP().toString()); })
      .onWiFiDisconnected([]()
                          { Serial.println("WiFi Connection Lost!"); })
      .onWiFiReconnect([](int attempt)
                       { Serial.printf("WiFi Reconnecting... Attempt: %d\n", attempt); })
      .addOutputPin(10) // Without callback parameter
      .asAnalog()
      .addCustomAttribute("type", "TEMPERATURE")
      .addInputPin(11)                          // Pin 11 as input
      .addCustomAttribute("location", "Room A") // Location attribute
      .asAnalog()
      .addOutputPin(40)                  // Without callback parameter
      .asDigital()                       // Pin 40 as digital output
      .addCustomAttribute("type", "LED") // Device type attribute
      .setMqttServer("broker.hivemq.com", 1883)
      .setCallback(handleDataWithJson)
      .enableBuffer()               // Enable buffering
      .setIntervalSentBuffer(10000) // Send buffer every 10 seconds
      .maxBufferSize(50)            // Maximum 50 data
      .setCircularBuffer()          // Circular buffer mode
      .addLocationTracking()
      .begin();

  // Show device information
  myDevice.showInfo();
}

void loop()
{
  myDevice.run();

  // Option 1: Direct single update
  myDevice.updateValue(10, HIGH);
  myDevice.updateCustomData("temperature", 25);
  myDevice.updateCustomData("mode", "auto");
  myDevice.updateLocationTracking("-7.2575", "112.7498");

  // Option 2: Batch update for multiple updates
  myDevice
      .beginUpdateBatch()
      .updateValue(10, HIGH)
      .updateValue(11, LOW)
      .updateAttribute(11, "status", "active")
      .updateCustomData("temperature", 25)
      .updateCustomData("mode", "auto")
      .endUpdateBatch();

  delay(100);
}
