/**
 * @file stack.hpp
 * @brief Stack data structure implementation
 *
 * A template-based stack (LIFO - Last In First Out) with fixed maximum capacity.
 * Provides standard stack operations: push, pop, peek, isEmpty, isFull.
 */

#pragma once

#include <array>
#include <optional>
#include <stdexcept>
#include <cstddef>

namespace dsav {

/**
 * @brief Fixed-size stack data structure
 *
 * Template parameters:
 * - T: Type of elements stored in the stack
 * - MaxSize: Maximum number of elements (default 16)
 *
 * The stack uses a fixed-size array internally, making it suitable for
 * visualization where we want predictable bounds.
 */
template<typename T, size_t MaxSize = 16>
class Stack {
public:
    /**
     * @brief Construct an empty stack
     */
    Stack() = default;

    /**
     * @brief Push an element onto the stack
     *
     * @param value Value to push
     * @return true if successful, false if stack is full
     */
    bool push(const T& value) {
        if (isFull()) {
            return false;
        }
        m_data[++m_top] = value;
        return true;
    }

    /**
     * @brief Pop an element from the stack
     *
     * @return The popped value if successful, std::nullopt if stack is empty
     */
    std::optional<T> pop() {
        if (isEmpty()) {
            return std::nullopt;
        }
        return m_data[m_top--];
    }

    /**
     * @brief Peek at the top element without removing it
     *
     * @return The top value if stack is not empty, std::nullopt otherwise
     */
    std::optional<T> peek() const {
        if (isEmpty()) {
            return std::nullopt;
        }
        return m_data[m_top];
    }

    /**
     * @brief Check if the stack is empty
     *
     * @return true if empty
     */
    bool isEmpty() const {
        return m_top < 0;
    }

    /**
     * @brief Check if the stack is full
     *
     * @return true if full
     */
    bool isFull() const {
        return m_top >= static_cast<int>(MaxSize) - 1;
    }

    /**
     * @brief Get current number of elements
     *
     * @return Number of elements in the stack
     */
    size_t size() const {
        return static_cast<size_t>(m_top + 1);
    }

    /**
     * @brief Get maximum capacity
     *
     * @return Maximum number of elements the stack can hold
     */
    constexpr size_t capacity() const {
        return MaxSize;
    }

    /**
     * @brief Clear all elements
     *
     * Resets the stack to empty state.
     */
    void clear() {
        m_top = -1;
    }

    /**
     * @brief Get direct access to internal data (for visualization)
     *
     * Returns a pointer to the internal array. This is used by the visualizer
     * to render the current stack state.
     *
     * @return Const pointer to internal data array
     */
    const T* data() const {
        return m_data.data();
    }

    /**
     * @brief Get the current top index (for visualization)
     *
     * @return Index of the top element (-1 if empty)
     */
    int topIndex() const {
        return m_top;
    }

    /**
     * @brief Get element at a specific index (for visualization)
     *
     * @param index Index to access (must be <= m_top)
     * @return Element at the given index
     * @throws std::out_of_range if index is invalid
     */
    const T& at(size_t index) const {
        if (index > static_cast<size_t>(m_top)) {
            throw std::out_of_range("Stack index out of range");
        }
        return m_data[index];
    }

private:
    std::array<T, MaxSize> m_data{};  ///< Internal storage array
    int m_top = -1;                   ///< Index of top element (-1 when empty)
};

} // namespace dsav
