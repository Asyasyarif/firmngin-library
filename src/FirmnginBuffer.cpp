#include "FirmnginBuffer.h"

namespace FirmnginCore
{

    BufferManager::BufferManager(size_t maxSize, bool debug)
        : _maxSize(maxSize), _debugMode(debug) {}

    void BufferManager::enable()
    {
        _enabled = true;
    }

    void BufferManager::setCircularMode()
    {
        _circularMode = true;
        _queueMode = false;
    }

    void BufferManager::setQueueMode()
    {
        _queueMode = true;
        _circularMode = false;
    }

    void BufferManager::setSendInterval(unsigned long ms)
    {
        _sendInterval = ms;
    }

    void BufferManager::setMaxSize(size_t size)
    {
        _maxSize = size;
    }

    bool BufferManager::addData(const BufferedData &data)
    {
        if (!_enabled)
            return false;

        if (_buffer.size() >= _maxSize)
        {
            if (_circularMode)
            {
                _buffer.erase(_buffer.begin());
            }
            else if (_queueMode)
            {
                return false;
            }
        }

        _buffer.push_back(data);
        return true;
    }

    const std::vector<BufferedData> &BufferManager::getData() const
    {
        return _buffer;
    }

    void BufferManager::clear()
    {
        _buffer.clear();
    }

    bool BufferManager::shouldSend() const
    {
        return (millis() - _lastSendTime) >= _sendInterval;
    }

    void BufferManager::run()
    {
        if (!_enabled)
            return;

        if (shouldSend())
        {
            _lastSendTime = millis();

            if (_debugMode)
            {
                Serial.printf("Buffer size: %d\n", _buffer.size());
            }
        }
    }

    void BufferManager::checkMemory() const
    {
        if (_debugMode)
        {
            Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
        }
    }

} // namespace FirmnginCore