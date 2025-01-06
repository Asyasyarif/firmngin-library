#include "FirmnginPins.h"

namespace FirmnginCore
{

    PinManager::PinManager(bool debug)
        : _debugMode(debug) {}

    PinManager &PinManager::addOutputPin(int pin)
    {
        Pin newPin;
        newPin.pin = pin;
        newPin.isOutput = true;
        newPin.mode = PinMode::PIN_DIGITAL;
        pinMode(pin, OUTPUT);
        _pins.push_back(newPin);
        return *this;
    }

    PinManager &PinManager::addInputPin(int pin)
    {
        Pin newPin;
        newPin.pin = pin;
        newPin.isOutput = false;
        newPin.mode = PinMode::PIN_DIGITAL;
        pinMode(pin, INPUT);
        _pins.push_back(newPin);
        return *this;
    }

    PinManager &PinManager::asAnalog()
    {
        if (!_pins.empty())
        {
            _pins.back().mode = PinMode::PIN_ANALOG;
        }
        return *this;
    }

    PinManager &PinManager::asDigital()
    {
        if (!_pins.empty())
        {
            _pins.back().mode = PinMode::PIN_DIGITAL;
        }
        return *this;
    }

    PinManager &PinManager::addCustomAttribute(const String &key, const String &value)
    {
        if (!_pins.empty())
        {
            _pins.back().customAttributes[key] = value;
        }
        return *this;
    }

    bool PinManager::updateValue(int pin, int value)
    {
        Pin *targetPin = getPin(pin);
        if (!targetPin)
            return false;

        if (targetPin->isOutput)
        {
            if (targetPin->mode == PinMode::PIN_DIGITAL)
            {
                digitalWrite(targetPin->pin, value);
            }
            else
            {
                analogWrite(targetPin->pin, value);
            }
            return true;
        }
        return false;
    }

    bool PinManager::updateAttribute(int pin, const String &key, const String &value)
    {
        Pin *targetPin = getPin(pin);
        if (!targetPin)
            return false;

        targetPin->customAttributes[key] = value;
        return true;
    }

    const std::vector<Pin> &PinManager::getPins() const
    {
        return _pins;
    }

    Pin *PinManager::getPin(int pinNumber)
    {
        for (auto &pin : _pins)
        {
            if (pin.pin == pinNumber)
            {
                return &pin;
            }
        }
        return nullptr;
    }

    void PinManager::run()
    {
        for (const auto &pin : _pins)
        {
            if (!pin.isOutput)
            {
                checkPinState(pin);
            }
        }
    }

    void PinManager::checkPinState(const Pin &pin)
    {
        int currentValue;
        if (pin.mode == PinMode::PIN_DIGITAL)
        {
            currentValue = digitalRead(pin.pin);
        }
        else
        {
            currentValue = analogRead(pin.pin);
        }

        if (_debugMode)
        {
            Serial.printf("Pin %d value: %d\n", pin.pin, currentValue);
        }
    }

} // namespace FirmnginCore