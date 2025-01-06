#ifndef FIRMNGIN_TYPES_H
#define FIRMNGIN_TYPES_H

#include <Arduino.h>
#include <vector>
#include <map>

namespace FirmnginCore
{

    enum class PinMode : uint8_t
    {
        PIN_DIGITAL = 0,
        PIN_ANALOG = 1
    };

    struct Pin
    {
        int pin;                                   // Pin number
        bool isOutput;                             // Whether pin is output
        PinMode mode;                              // Pin mode (digital/analog)
        std::map<String, String> customAttributes; // Custom attributes for the pin
    };

    struct BatchUpdate
    {
        String type;   // Type of update: "pin", "attribute", "custom"
        int pin;       // Pin number (-1 for non-pin updates)
        String key;    // Key for attribute or custom updates
        String value;  // Value to update
        String value2; // Secondary value
    };

    struct BufferedData
    {
        unsigned long timestamp; // Timestamp of data
        String type;             // Type of data
        int pin;                 // Pin number
        String key;              // Key for attribute/custom data
        String value;            // Primary value
        String value2;           // Secondary value
    };

    class NetworkManager;
    class BufferManager;
    class PinManager;

} // namespace FirmnginCore

#endif // FIRMNGIN_TYPES_H