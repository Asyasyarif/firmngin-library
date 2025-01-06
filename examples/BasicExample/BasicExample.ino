#include <Firmngin.h>

// WiFi credentials
#define SSID "your_ssid"
#define PASSWORD "your_password"

// Create device instance
Firmngin device("DEVICE_KEY_123", "API_KEY_456");

void setup()
{
    // Initialize device with WiFi and MQTT
    device
        .setWiFi(SSID, PASSWORD)
        .enableAutoReconnect()
        .onWiFiConnected([]()
                         { Serial.println("WiFi Connected!"); })
        .onWiFiDisconnected([]()
                            { Serial.println("WiFi Connection Lost!"); })

        // Add output pin (LED)
        .addOutputPin(LED_BUILTIN)
        .asDigital()
        .addCustomAttribute("type", "LED")

        // Add input pin (Button)
        .addInputPin(0)
        .asDigital()
        .addCustomAttribute("type", "BUTTON")

        // Start device
        .begin();

    // Show device info
    device.showInfo();
}

void loop()
{
    // Run device tasks
    device.run();

    // Toggle LED every second
    static unsigned long lastToggle = 0;
    if (millis() - lastToggle >= 1000)
    {
        static bool ledState = false;
        ledState = !ledState;
        device.updateValue(LED_BUILTIN, ledState);
        lastToggle = millis();
    }
}