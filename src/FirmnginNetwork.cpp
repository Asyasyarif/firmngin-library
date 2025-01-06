#include "FirmnginNetwork.h"

namespace FirmnginCore
{

    NetworkManager::NetworkManager(const String &deviceKey, bool debug)
        : _deviceKey(deviceKey), _debugMode(debug) {}

    void NetworkManager::setupWiFi(const char *ssid, const char *password)
    {
        _wifiSSID = ssid;
        _wifiPassword = password;
        _wifiEnabled = true;

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        if (_debugMode)
        {
            Serial.println("Connecting to WiFi...");
        }
    }

    void NetworkManager::setupMqtt(const char *server, uint16_t port)
    {
        _mqttServer = server;
        _mqttPort = port;
        _mqttEnabled = true;

        _mqttClient.setServer(server, port);
        setupCallbacks();
    }

    void NetworkManager::setMqttCredentials(const char *username, const char *password)
    {
        _mqttClient.setCredentials(username, password);
    }

    void NetworkManager::onWiFiConnected(std::function<void()> callback)
    {
        _wifiConnectedCallback = callback;
    }

    void NetworkManager::onWiFiDisconnected(std::function<void()> callback)
    {
        _wifiDisconnectedCallback = callback;
    }

    void NetworkManager::onWiFiReconnect(std::function<void(int)> callback)
    {
        _wifiReconnectCallback = callback;
    }

    void NetworkManager::enableAutoReconnect()
    {
        _autoReconnectEnabled = true;

#if defined(ESP8266)
        WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP &event)
                                {
            if (_debugMode) {
                Serial.println("WiFi Connected!");
                Serial.print("IP: ");
                Serial.println(WiFi.localIP());
            }
            if (_wifiConnectedCallback) {
                _wifiConnectedCallback();
            } });

        WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event)
                                       {
            _reconnectAttempts++;
            if (_wifiReconnectCallback) {
                _wifiReconnectCallback(_reconnectAttempts);
            }
            WiFi.begin(_wifiSSID.c_str(), _wifiPassword.c_str()); });
#endif
    }

    void NetworkManager::publishMqtt(const char *topic, const char *payload)
    {
        if (!_mqttEnabled || !_mqttClient.connected())
            return;
        _mqttClient.publish(topic, 0, true, payload);
    }

    bool NetworkManager::isWiFiConnected() const
    {
        return WiFi.status() == WL_CONNECTED;
    }

    void NetworkManager::run()
    {
        // Handle WiFi reconnection
        if (_wifiEnabled && !isWiFiConnected())
        {
            if (_debugMode)
            {
                Serial.println("WiFi disconnected!");
            }

            if (_wifiDisconnectedCallback)
            {
                _wifiDisconnectedCallback();
            }

            if (_autoReconnectEnabled && _reconnectAttempts < MAX_RECONNECT_ATTEMPTS)
            {
                WiFi.begin(_wifiSSID.c_str(), _wifiPassword.c_str());
            }
        }

        // Handle MQTT reconnection
        if (_mqttEnabled && !_mqttClient.connected())
        {
            _mqttClient.connect();
        }
    }

    void NetworkManager::setupCallbacks()
    {
        _mqttClient.onConnect([this](bool sessionPresent)
                              {
            if (_debugMode) {
                Serial.println("Connected to MQTT!");
            } });

        _mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason)
                                 {
            if (_debugMode) {
                Serial.println("Disconnected from MQTT!");
            } });
    }

} // namespace FirmnginCore