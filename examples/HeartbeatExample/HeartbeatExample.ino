#include <Arduino.h>
#include "firmngin.h"

#define SSID "your_ssid"
#define PASSWORD "your_password"

// Pin definitions
#define LED_HEARTBEAT 2 // Built-in LED untuk heartbeat indicator
#define TEMP_SENSOR 34  // Temperature sensor pin
#define FAN_PIN 25      // Fan control pin

Firmngin myDevice = Firmngin("DEVICE_KEY_123", "API_KEY_456");

// Variables for monitoring
float temperature = 0;
int fanSpeed = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_HEARTBEAT, OUTPUT);
    pinMode(TEMP_SENSOR, INPUT);
    pinMode(FAN_PIN, OUTPUT);

    myDevice
        .setWiFi(SSID, PASSWORD)
        .enableAutoReconnect()
        .setIntervalHeartbeat(60000) // Set heartbeat interval to 60 seconds
        .enableBuffer()
        .setIntervalSentBuffer(10000)
        .maxBufferSize(50)
        .begin();

    // Setup monitoring pins
    myDevice
        .addInputPin(TEMP_SENSOR)
        .asAnalog()
        .addCustomAttribute("type", "TEMPERATURE")
        .addCustomAttribute("unit", "celsius")

        .addOutputPin(FAN_PIN)
        .asAnalog()
        .addCustomAttribute("type", "FAN")
        .addCustomAttribute("max_speed", "255");

    // Tampilkan device info
    myDevice.showInfo();
}

void loop()
{
    myDevice.run(); // Will handle heartbeat automatically

    // Read sensor
    temperature = analogRead(TEMP_SENSOR) * 3.3 / 4095.0 * 100; // Convert to Celsius

    // Control logic
    if (temperature > 30)
    {
        fanSpeed = map(temperature, 30, 50, 128, 255); // Increase fan speed with temperature
        analogWrite(FAN_PIN, fanSpeed);
    }
    else
    {
        fanSpeed = 0;
        analogWrite(FAN_PIN, fanSpeed);
    }

    // Update nilai ke server
    myDevice
        .beginUpdateBatch()
        .updateValue(TEMP_SENSOR, (int)(temperature * 100))
        .updateValue(FAN_PIN, fanSpeed)
        .updateAttribute(TEMP_SENSOR, "last_read", String(millis()))
        .endUpdateBatch();

    // Visual feedback for heartbeat
    digitalWrite(LED_HEARTBEAT, (millis() / 1000) % 2); // Blink every second

    delay(100);
}