#include "Firmngin.h"

Firmngin::Firmngin(const String &deviceKey, const String &apiKey)
    : _deviceKey(deviceKey), _apiKey(apiKey),
      _network(deviceKey, _debugMode),
      _buffer(100, _debugMode),
      _pins(_debugMode) {}

Firmngin &Firmngin::begin()
{
    if (_debugMode)
    {
        Serial.println("Initializing Firmngin...");
    }
    return *this;
}

void Firmngin::run()
{
    _network.run();
    _buffer.run();
    _pins.run();

    if (_otaEnabled)
    {
        handleOTA();
    }

    if (millis() - _lastHeartbeat >= _heartbeatInterval)
    {
        sendHeartbeat();
    }
}

void Firmngin::showInfo()
{
    if (_debugMode)
    {
        Serial.println("\n=== Firmngin Info ===");
        Serial.println("Device Key: " + _deviceKey);
        Serial.println("Debug Mode: " + String(_debugMode ? "ON" : "OFF"));
        Serial.println("OTA Enabled: " + String(_otaEnabled ? "YES" : "NO"));
        Serial.println("===================\n");
    }
}

// Network configuration methods
Firmngin &Firmngin::setWiFi(const char *ssid, const char *password)
{
    _network.setupWiFi(ssid, password);
    return *this;
}

Firmngin &Firmngin::setMqttServer(const char *server, uint16_t port)
{
    _network.setupMqtt(server, port);
    return *this;
}

Firmngin &Firmngin::setMqttCredentials(const char *username, const char *password)
{
    _network.setMqttCredentials(username, password);
    return *this;
}

Firmngin &Firmngin::enableAutoReconnect()
{
    _network.enableAutoReconnect();
    return *this;
}

Firmngin &Firmngin::onWiFiConnected(std::function<void()> callback)
{
    _network.onWiFiConnected(callback);
    return *this;
}

Firmngin &Firmngin::onWiFiDisconnected(std::function<void()> callback)
{
    _network.onWiFiDisconnected(callback);
    return *this;
}

Firmngin &Firmngin::onWiFiReconnect(std::function<void(int)> callback)
{
    _network.onWiFiReconnect(callback);
    return *this;
}

// Pin management methods
Firmngin &Firmngin::addOutputPin(int pin)
{
    _pins.addOutputPin(pin);
    return *this;
}

Firmngin &Firmngin::addInputPin(int pin)
{
    _pins.addInputPin(pin);
    return *this;
}

Firmngin &Firmngin::asAnalog()
{
    _pins.asAnalog();
    return *this;
}

Firmngin &Firmngin::asDigital()
{
    _pins.asDigital();
    return *this;
}

Firmngin &Firmngin::addCustomAttribute(const String &key, const String &value)
{
    _pins.addCustomAttribute(key, value);
    return *this;
}

Firmngin &Firmngin::updateValue(int pin, int value)
{
    _pins.updateValue(pin, value);
    return *this;
}

Firmngin &Firmngin::updateAttribute(int pin, const String &key, const String &value)
{
    _pins.updateAttribute(pin, key, value);
    return *this;
}

// Buffer management methods
Firmngin &Firmngin::enableBuffer()
{
    _buffer.enable();
    return *this;
}

Firmngin &Firmngin::setCircularBuffer()
{
    _buffer.setCircularMode();
    return *this;
}

Firmngin &Firmngin::setQueueBuffer()
{
    _buffer.setQueueMode();
    return *this;
}

Firmngin &Firmngin::setIntervalSentBuffer(unsigned long ms)
{
    _buffer.setSendInterval(ms);
    return *this;
}

Firmngin &Firmngin::maxBufferSize(int size)
{
    _buffer.setMaxSize(size);
    return *this;
}

// Custom data methods
Firmngin &Firmngin::updateCustomData(const String &key, const String &value)
{
    FirmnginCore::BufferedData data;
    data.timestamp = millis();
    data.type = "custom";
    data.pin = -1;
    data.key = key;
    data.value = value;
    _buffer.addData(data);
    return *this;
}

Firmngin &Firmngin::updateCustomData(const String &key, int value)
{
    return updateCustomData(key, String(value));
}

Firmngin &Firmngin::updateCustomData(const String &key, float value)
{
    return updateCustomData(key, String(value));
}

Firmngin &Firmngin::updateCustomData(const String &key, double value)
{
    return updateCustomData(key, String(value));
}

// Batch update methods
Firmngin &Firmngin::beginUpdateBatch()
{
    _batchMode = true;
    _batchUpdates.clear();
    return *this;
}

Firmngin &Firmngin::endUpdateBatch()
{
    processBatchUpdates();
    _batchMode = false;
    return *this;
}

// Debug control
Firmngin &Firmngin::noDebug()
{
    _debugMode = false;
    return *this;
}

// OTA Methods
Firmngin &Firmngin::enableOTA()
{
    _otaEnabled = true;
    return *this;
}

Firmngin &Firmngin::setOTAPassword(const String &password)
{
    _otaPassword = password;
    return *this;
}

Firmngin &Firmngin::onOTAProgress(std::function<void(int)> callback)
{
    _otaProgressCallback = callback;
    return *this;
}

Firmngin &Firmngin::setIntervalHeartbeat(unsigned long ms)
{
    _heartbeatInterval = ms;
    return *this;
}

// Location Methods
Firmngin &Firmngin::addLocationTracking()
{
    _locationEnabled = true;
    return *this;
}

Firmngin &Firmngin::updateLocationTracking(const String &latitude, const String &longitude)
{
    if (_locationEnabled)
    {
        _lastLatitude = latitude;
        _lastLongitude = longitude;

        if (!_batchMode)
        {
            FirmnginCore::BufferedData data;
            data.timestamp = millis();
            data.type = "location";
            data.key = "location";
            data.value = latitude;
            data.value2 = longitude;
            _buffer.addData(data);
        }
    }
    return *this;
}

// Private methods
void Firmngin::processBatchUpdates()
{
    for (const auto &update : _batchUpdates)
    {
        FirmnginCore::BufferedData data;
        data.timestamp = millis();
        data.type = update.type;
        data.pin = update.pin;
        data.key = update.key;
        data.value = update.value;
        data.value2 = update.value2;
        _buffer.addData(data);
    }
}

void Firmngin::handleOTA()
{
    // OTA handling implementation
}

void Firmngin::sendHeartbeat()
{
    if (_debugMode)
    {
        Serial.println("Sending heartbeat...");
    }
    _lastHeartbeat = millis();
}

Firmngin &Firmngin::setCallback(SimpleCallback callback)
{
    _simpleCallback = callback;
    _jsonCallback = nullptr; // Reset JSON callback
    return *this;
}

Firmngin &Firmngin::setCallback(JsonCallback callback)
{
    _jsonCallback = callback;
    _simpleCallback = nullptr; // Reset simple callback
    return *this;
}

void Firmngin::handleIncomingData(const String &key, const String &value, const String &rawData)
{
    if (_simpleCallback)
    {
        _simpleCallback(key, value);
    }
    else if (_jsonCallback)
    {
        _jsonCallback(key, value, rawData);
    }
}

bool Firmngin::isWiFiConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}