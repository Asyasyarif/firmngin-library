#include <Arduino.h>
#include "firmngin.h"

#define SSID "your_ssid"
#define PASSWORD "your_password"

// Pin definitions
#define SENSOR_PIN 34 // ADC pin for sensor
#define LED_STATUS 2  // Built-in LED

Firmngin myDevice = Firmngin("DEVICE_KEY_123", "API_KEY_456");

void onWiFiConnected()
{
    digitalWrite(LED_STATUS, HIGH);
    Serial.println("WiFi Connected!");
    Serial.println("Buffered data will be sent...");
}

void onWiFiDisconnected()
{
    digitalWrite(LED_STATUS, LOW);
    Serial.println("WiFi Disconnected!");
    Serial.println("Data will be stored in circular buffer...");
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_STATUS, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);

    myDevice
        .setWiFi(SSID, PASSWORD)
        .enableAutoReconnect()
        .onWiFiConnected(onWiFiConnected)
        .onWiFiDisconnected(onWiFiDisconnected)
        .enableBuffer()
        .setCircularBuffer()         // Enable circular buffer
        .setIntervalSentBuffer(5000) // Send buffer every 5 seconds
        .maxBufferSize(10)           // Keep last 10 readings
        .begin();

    // Setup sensor pin
    myDevice
        .addInputPin(SENSOR_PIN)
        .asAnalog()
        .addCustomAttribute("type", "SENSOR")
        .addCustomAttribute("unit", "raw");

    Serial.println("Device started with circular buffer");
    Serial.println("Buffer size: 10 readings");
    Serial.println("When buffer is full, oldest data will be overwritten");
}

void loop()
{
    myDevice.run();

    // Read sensor every second
    static unsigned long lastReadTime = 0;
    if (millis() - lastReadTime >= 1000)
    {
        int sensorValue = analogRead(SENSOR_PIN);

        // Update value (will be buffered if offline)
        myDevice.updateValue(SENSOR_PIN, sensorValue);

        if (myDevice.isWiFiConnected())
        {
            Serial.printf("Sensor value sent: %d\n", sensorValue);
        }
        else
        {
            Serial.printf("Sensor value buffered: %d\n", sensorValue);
        }

        lastReadTime = millis();
    }

    delay(100);
}