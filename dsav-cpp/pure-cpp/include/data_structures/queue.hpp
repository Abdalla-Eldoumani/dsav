/**
 * @file queue.hpp
 * @brief Queue data structure implementation
 *
 * A template-based queue (FIFO - First In First Out) with fixed maximum capacity.
 * Implemented using a circular buffer for efficient enqueue/dequeue operations.
 */

#pragma once

#include <array>
#include <optional>
#include <stdexcept>
#include <cstddef>

namespace dsav {

/**
 * @brief Fixed-size queue data structure using circular buffer
 *
 * Template parameters:
 * - T: Type of elements stored in the queue
 * - MaxSize: Maximum number of elements (default 16)
 *
 * The queue uses a circular buffer internally, making enqueue and dequeue O(1).
 */
template<typename T, size_t MaxSize = 16>
class Queue {
public:
    /**
     * @brief Construct an empty queue
     */
    Queue() = default;

    /**
     * @brief Enqueue an element at the rear
     *
     * @param value Value to enqueue
     * @return true if successful, false if queue is full
     */
    bool enqueue(const T& value) {
        if (isFull()) {
            return false;
        }

        m_data[m_rear] = value;
        m_rear = (m_rear + 1) % MaxSize;
        m_size++;
        return true;
    }

    /**
     * @brief Dequeue an element from the front
     *
     * @return The dequeued value if successful, std::nullopt if queue is empty
     */
    std::optional<T> dequeue() {
        if (isEmpty()) {
            return std::nullopt;
        }

        T value = m_data[m_front];
        m_front = (m_front + 1) % MaxSize;
        m_size--;
        return value;
    }

    /**
     * @brief Peek at the front element without removing it
     *
     * @return The front value if queue is not empty, std::nullopt otherwise
     */
    std::optional<T> peek() const {
        if (isEmpty()) {
            return std::nullopt;
        }
        return m_data[m_front];
    }

    /**
     * @brief Check if the queue is empty
     *
     * @return true if empty
     */
    bool isEmpty() const {
        return m_size == 0;
    }

    /**
     * @brief Check if the queue is full
     *
     * @return true if full
     */
    bool isFull() const {
        return m_size >= MaxSize;
    }

    /**
     * @brief Get current number of elements
     *
     * @return Number of elements in the queue
     */
    size_t size() const {
        return m_size;
    }

    /**
     * @brief Get maximum capacity
     *
     * @return Maximum number of elements the queue can hold
     */
    constexpr size_t capacity() const {
        return MaxSize;
    }

    /**
     * @brief Clear all elements
     *
     * Resets the queue to empty state.
     */
    void clear() {
        m_front = 0;
        m_rear = 0;
        m_size = 0;
    }

    /**
     * @brief Get direct access to internal data (for visualization)
     *
     * @return Const pointer to internal data array
     */
    const T* data() const {
        return m_data.data();
    }

    /**
     * @brief Get the front index (for visualization)
     *
     * @return Index of the front element
     */
    size_t frontIndex() const {
        return m_front;
    }

    /**
     * @brief Get the rear index (for visualization)
     *
     * @return Index where next element will be enqueued
     */
    size_t rearIndex() const {
        return m_rear;
    }

    /**
     * @brief Get element at a specific internal index (for visualization)
     *
     * Note: This is the raw array index, not queue position.
     *
     * @param index Array index to access
     * @return Element at the given index
     * @throws std::out_of_range if index >= MaxSize
     */
    const T& at(size_t index) const {
        if (index >= MaxSize) {
            throw std::out_of_range("Queue index out of range");
        }
        return m_data[index];
    }

    /**
     * @brief Get element at queue position (0 = front, size-1 = rear)
     *
     * @param position Position in queue (0-based from front)
     * @return Element at the given position
     * @throws std::out_of_range if position >= size
     */
    const T& atPosition(size_t position) const {
        if (position >= m_size) {
            throw std::out_of_range("Queue position out of range");
        }
        size_t actualIndex = (m_front + position) % MaxSize;
        return m_data[actualIndex];
    }

private:
    std::array<T, MaxSize> m_data{};  ///< Internal circular buffer
    size_t m_front = 0;               ///< Index of front element
    size_t m_rear = 0;                ///< Index where next element will be added
    size_t m_size = 0;                ///< Current number of elements
};

} // namespace dsav
