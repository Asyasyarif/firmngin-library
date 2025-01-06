#include <Firmngin.h>

// WiFi credentials
#define SSID "your_ssid"
#define PASSWORD "your_password"

// Create device instance
Firmngin device("DEVICE_KEY_123", "API_KEY_456");

void setup()
{
    // Initialize device with advanced features
    device
        .setWiFi(SSID, PASSWORD)
        .enableAutoReconnect()
        .onWiFiConnected([]()
                         { Serial.println("WiFi Connected!"); })
        .onWiFiDisconnected([]()
                            { Serial.println("WiFi Connection Lost!"); })

        // Enable OTA updates
        .enableOTA()
        .setOTAPassword("secret123")
        .onOTAProgress([](int progress)
                       { Serial.printf("OTA Progress: %d%%\n", progress); })

        // Enable data buffering
        .enableBuffer()
        .setCircularBuffer()
        .setIntervalSentBuffer(10000)
        .maxBufferSize(50)

        // Enable location tracking
        .addLocationTracking()

        // Add sensors
        .addInputPin(36)
        .asAnalog()
        .addCustomAttribute("type", "TEMPERATURE")

        .addInputPin(39)
        .asAnalog()
        .addCustomAttribute("type", "HUMIDITY")

        // Start device
        .begin();

    // Show device info
    device.showInfo();
}

void loop()
{
    // Run device tasks
    device.run();

    // Update sensor readings every 5 seconds
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate >= 5000)
    {
        // Start batch update
        device.beginUpdateBatch()
            .updateValue(36, analogRead(36)) // Temperature
            .updateValue(39, analogRead(39)) // Humidity
            .updateCustomData("status", "active")
            .updateLocationTracking("-7.2575", "112.7498")
            .endUpdateBatch();

        lastUpdate = millis();
    }
}