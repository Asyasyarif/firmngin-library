#ifndef FIRMNGIN_BUFFER_H
#define FIRMNGIN_BUFFER_H

#include <Arduino.h>
#include <vector>
#include "FirmnginTypes.h"

namespace FirmnginCore
{

    /**
     * @brief Manages data buffering and storage operations
     */
    class BufferManager
    {
    public:
        /**
         * @brief Initialize buffer manager
         * @param maxSize Maximum buffer size
         * @param debug Enable debug output
         */
        BufferManager(size_t maxSize = 100, bool debug = true);

        /**
         * @brief Enable buffer functionality
         */
        void enable();

        /**
         * @brief Set buffer mode to circular
         * When buffer is full, oldest data will be overwritten
         */
        void setCircularMode();

        /**
         * @brief Set buffer mode to queue
         * When buffer is full, new data will be dropped
         */
        void setQueueMode();

        /**
         * @brief Set interval for sending buffered data
         * @param ms Interval in milliseconds
         */
        void setSendInterval(unsigned long ms);

        /**
         * @brief Set maximum buffer size
         * @param size Maximum number of items
         */
        void setMaxSize(size_t size);

        /**
         * @brief Add data to buffer
         * @param data Data to buffer
         * @return true if data was added successfully
         */
        bool addData(const BufferedData &data);

        /**
         * @brief Get all buffered data
         * @return Vector of buffered data
         */
        const std::vector<BufferedData> &getData() const;

        /**
         * @brief Clear buffer
         */
        void clear();

        /**
         * @brief Check if buffer needs to be sent
         * @return true if buffer should be sent
         */
        bool shouldSend() const;

        /**
         * @brief Run buffer tasks
         */
        void run();

    private:
        bool _enabled = false;
        bool _circularMode = false;
        bool _queueMode = false;
        size_t _maxSize;
        bool _debugMode;

        std::vector<BufferedData> _buffer;
        unsigned long _sendInterval = 5000;
        unsigned long _lastSendTime = 0;
        size_t _bufferIndex = 0;

        void checkMemory() const;
    };

} // namespace FirmnginCore

#endif // FIRMNGIN_BUFFER_H