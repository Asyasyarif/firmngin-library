#ifndef FIRMNGIN_NETWORK_H
#define FIRMNGIN_NETWORK_H

#include <Arduino.h>
#include <functional>

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "AsyncMqttClient.h"
#include "FirmnginTypes.h"

namespace FirmnginCore
{

    /**
     * @brief Handles all network-related operations including WiFi and MQTT
     */
    class NetworkManager
    {
    public:
        /**
         * @brief Initialize network manager
         * @param deviceKey Unique device identifier
         * @param debug Enable debug output
         */
        NetworkManager(const String &deviceKey, bool debug = true);

        /**
         * @brief Configure WiFi connection
         * @param ssid WiFi SSID
         * @param password WiFi password
         */
        void setupWiFi(const char *ssid, const char *password);

        /**
         * @brief Configure MQTT connection
         * @param server MQTT broker address
         * @param port MQTT broker port
         */
        void setupMqtt(const char *server, uint16_t port);

        /**
         * @brief Set MQTT credentials
         * @param username MQTT username
         * @param password MQTT password
         */
        void setMqttCredentials(const char *username, const char *password);

        /**
         * @brief Set callback for WiFi events
         */
        void onWiFiConnected(std::function<void()> callback);
        void onWiFiDisconnected(std::function<void()> callback);
        void onWiFiReconnect(std::function<void(int)> callback);

        /**
         * @brief Enable auto reconnect for WiFi
         */
        void enableAutoReconnect();

        /**
         * @brief Publish message to MQTT topic
         */
        void publishMqtt(const char *topic, const char *payload);

        /**
         * @brief Check if WiFi is connected
         */
        bool isWiFiConnected() const;

        /**
         * @brief Run network tasks
         */
        void run();

    private:
        String _deviceKey;
        bool _debugMode;

        // WiFi configuration
        String _wifiSSID;
        String _wifiPassword;
        bool _wifiEnabled = false;
        bool _autoReconnectEnabled = false;
        int _reconnectAttempts = 0;
        const int MAX_RECONNECT_ATTEMPTS = 5;

        // MQTT configuration
        AsyncMqttClient _mqttClient;
        bool _mqttEnabled = false;
        String _mqttServer;
        uint16_t _mqttPort;

        // Callbacks
        std::function<void()> _wifiConnectedCallback;
        std::function<void()> _wifiDisconnectedCallback;
        std::function<void(int)> _wifiReconnectCallback;

        void setupCallbacks();
    };

} // namespace FirmnginCore

#endif // FIRMNGIN_NETWORK_H