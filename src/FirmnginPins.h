#ifndef FIRMNGIN_PINS_H
#define FIRMNGIN_PINS_H

#include <Arduino.h>
#include <vector>
#include <map>
#include "FirmnginTypes.h"

namespace FirmnginCore
{

    /**
     * @brief Manages GPIO pins and their attributes
     */
    class PinManager
    {
    public:
        /**
         * @brief Initialize pin manager
         * @param debug Enable debug output
         */
        PinManager(bool debug = true);

        /**
         * @brief Add output pin
         * @param pin Pin number
         * @return Reference to this object
         */
        PinManager &addOutputPin(int pin);

        /**
         * @brief Add input pin
         * @param pin Pin number
         * @return Reference to this object
         */
        PinManager &addInputPin(int pin);

        /**
         * @brief Set pin mode to analog
         * @return Reference to this object
         */
        PinManager &asAnalog();

        /**
         * @brief Set pin mode to digital
         * @return Reference to this object
         */
        PinManager &asDigital();

        /**
         * @brief Add custom attribute to last added pin
         * @param key Attribute key
         * @param value Attribute value
         * @return Reference to this object
         */
        PinManager &addCustomAttribute(const String &key, const String &value);

        /**
         * @brief Update pin value
         * @param pin Pin number
         * @param value New value
         * @return true if update was successful
         */
        bool updateValue(int pin, int value);

        /**
         * @brief Update pin attribute
         * @param pin Pin number
         * @param key Attribute key
         * @param value New value
         * @return true if update was successful
         */
        bool updateAttribute(int pin, const String &key, const String &value);

        /**
         * @brief Get all pins
         * @return Vector of pins
         */
        const std::vector<Pin> &getPins() const;

        /**
         * @brief Run pin tasks
         */
        void run();

    private:
        std::vector<Pin> _pins;
        bool _debugMode;

        Pin *getPin(int pinNumber);
        void checkPinState(const Pin &pin);
    };

} // namespace FirmnginCore

#endif // FIRMNGIN_PINS_H