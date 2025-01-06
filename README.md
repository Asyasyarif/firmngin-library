# Firmngin Arduino Library

Firmngin is an Arduino library for IoT device management with MQTT support. This library provides an easy-to-use interface for managing WiFi connections, MQTT communication, pin management, and data buffering.

## Features

- WiFi connection with auto reconnect
- MQTT communication
- Digital and analog pin management
- Data buffering with circular or queue mode
- Location tracking
- Offline mode (planned)
- Custom data updates
- Batch updates
- Heartbeat monitoring
- OTA support (planned)

## Installation

1. Download this library as a ZIP file
2. In Arduino IDE: Sketch > Include Library > Add .ZIP Library
3. Select the downloaded ZIP file

## Dependencies

- [AsyncMqttClient](https://github.com/marvinroger/async-mqtt-client)

## Basic Usage

```cpp
#include <Firmngin.h>

// Initialize device
Firmngin device("DEVICE_KEY", "API_KEY");

void setup() {
    device
        .setWiFi("SSID", "PASSWORD")
        .enableAutoReconnect()
        .setMqttServer("broker.hivemq.com", 1883)
        .addOutputPin(LED_BUILTIN)
        .asDigital()
        .begin();
}

void loop() {
    device.run();
    
    // Update pin value
    device.updateValue(LED_BUILTIN, HIGH);
    
    // Update custom data
    device.updateCustomData("temperature", 27.5);
    
    // Batch update
    device
        .beginUpdateBatch()
        .updateValue(LED_BUILTIN, HIGH)
        .updateCustomData("humidity", 80)
        .endUpdateBatch();
}
```

## Documentation

See the [examples](examples) folder for more usage examples.

### Available Examples

- [BasicExample](examples/BasicExample/BasicExample.ino) - Basic library usage
- [AdvancedFeatures](examples/AdvancedFeatures/AdvancedFeatures.ino) - Advanced features like buffering and batch updates

## License

MIT License 