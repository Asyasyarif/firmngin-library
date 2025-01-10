#ifndef FIRMNGIN_H
#define FIRMNGIN_H

#include <Arduino.h>
#include "FirmnginTypes.h"
#include "FirmnginNetwork.h"
#include "FirmnginBuffer.h"
#include "FirmnginPins.h"

// Tipe callback
typedef std::function<void(String, String)> SimpleCallback;
typedef std::function<void(String, String, String)> JsonCallback;

class Firmngin
{
public:
    Firmngin(const String &deviceKey, const String &apiKey);

    // Callback methods
    Firmngin &setCallback(SimpleCallback callback);
    Firmngin &setCallback(JsonCallback callback);

    Firmngin &begin();
    void run();
    void showInfo();

    // Network configuration methods
    Firmngin &setWiFi(const char *ssid, const char *password);
    Firmngin &setMqttServer(const char *server = DEFAULT_MQTT_SERVER, uint16_t port = DEFAULT_MQTT_PORT);
    Firmngin &setMqttCredentials(const char *username, const char *password);
    Firmngin &enableAutoReconnect();
    Firmngin &onWiFiConnected(std::function<void()> callback);
    Firmngin &onWiFiDisconnected(std::function<void()> callback);
    Firmngin &onWiFiReconnect(std::function<void(int)> callback);
    bool isWiFiConnected() const;

    // Pin management methods
    Firmngin &addOutputPin(int pin);
    Firmngin &addInputPin(int pin);
    Firmngin &asAnalog();
    Firmngin &asDigital();
    Firmngin &addCustomAttribute(const String &key, const String &value);
    Firmngin &updateValue(int pin, int value);
    Firmngin &updateAttribute(int pin, const String &key, const String &value);

    // Buffer management methods
    Firmngin &enableBuffer();
    Firmngin &setCircularBuffer();
    Firmngin &setQueueBuffer();
    Firmngin &setIntervalSentBuffer(unsigned long ms);
    Firmngin &maxBufferSize(int size);

    // Custom data methods
    Firmngin &updateCustomData(const String &key, const String &value);
    Firmngin &updateCustomData(const String &key, int value);
    Firmngin &updateCustomData(const String &key, float value);
    Firmngin &updateCustomData(const String &key, double value);

    // Batch update methods
    Firmngin &beginUpdateBatch();
    Firmngin &endUpdateBatch();

    // Debug control
    Firmngin &noDebug();

    // OTA Methods
    Firmngin &enableOTA();
    Firmngin &setOTAPassword(const String &password);
    Firmngin &onOTAProgress(std::function<void(int)> callback);
    Firmngin &setIntervalHeartbeat(unsigned long ms);

    // Location Methods
    Firmngin &addLocationTracking();
    Firmngin &updateLocationTracking(const String &latitude, const String &longitude);

private:
    String _deviceKey;
    String _apiKey;
    bool _debugMode = true;

    SimpleCallback _simpleCallback = nullptr;
    JsonCallback _jsonCallback = nullptr;

    FirmnginCore::NetworkManager _network;
    FirmnginCore::BufferManager _buffer;
    FirmnginCore::PinManager _pins;

    bool _batchMode = false;
    std::vector<FirmnginCore::BatchUpdate> _batchUpdates;

    // OTA variables
    bool _otaEnabled = false;
    String _otaPassword;
    std::function<void(int)> _otaProgressCallback;
    unsigned long _heartbeatInterval = 10000;
    unsigned long _lastHeartbeat = 0;

    // Location tracking
    bool _locationEnabled = false;
    String _lastLatitude;
    String _lastLongitude;

    static constexpr const char *DEFAULT_MQTT_SERVER = "broker.hivemq.com";
    static constexpr uint16_t DEFAULT_MQTT_PORT = 1883;

    void processBatchUpdates();
    void handleOTA();
    void sendHeartbeat();
    void handleIncomingData(const String &key, const String &value, const String &rawData);
};

#endif // FIRMNGIN_H