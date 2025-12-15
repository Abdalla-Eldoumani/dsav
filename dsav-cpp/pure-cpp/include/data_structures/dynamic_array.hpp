/**
 * @file dynamic_array.hpp
 * @brief Dynamic array data structure implementation
 *
 * A wrapper around std::vector that provides visualization-friendly interface
 * for demonstrating array operations: insert, delete, search, access.
 */

#pragma once

#include <vector>
#include <optional>
#include <stdexcept>
#include <algorithm>

namespace dsav {

/**
 * @brief Dynamic array data structure
 *
 * Template parameter:
 * - T: Type of elements stored in the array
 *
 * Wraps std::vector to provide a clean interface for visualization.
 */
template<typename T>
class DynamicArray {
public:
    /**
     * @brief Construct an empty array
     */
    DynamicArray() = default;

    /**
     * @brief Construct an array with initial capacity
     *
     * @param capacity Initial capacity to reserve
     */
    explicit DynamicArray(size_t capacity) {
        m_data.reserve(capacity);
    }

    /**
     * @brief Insert an element at the end
     *
     * @param value Value to insert
     */
    void pushBack(const T& value) {
        m_data.push_back(value);
    }

    /**
     * @brief Insert an element at a specific index
     *
     * Shifts elements to the right to make space.
     *
     * @param index Index where to insert (0 to size)
     * @param value Value to insert
     * @return true if successful, false if index out of range
     */
    bool insert(size_t index, const T& value) {
        if (index > m_data.size()) {
            return false;
        }
        m_data.insert(m_data.begin() + index, value);
        return true;
    }

    /**
     * @brief Delete element at a specific index
     *
     * Shifts elements to the left to fill the gap.
     *
     * @param index Index of element to delete
     * @return The deleted value if successful, std::nullopt if index out of range
     */
    std::optional<T> deleteAt(size_t index) {
        if (index >= m_data.size()) {
            return std::nullopt;
        }
        T value = m_data[index];
        m_data.erase(m_data.begin() + index);
        return value;
    }

    /**
     * @brief Access element at index
     *
     * @param index Index to access
     * @return Reference to element at index
     * @throws std::out_of_range if index >= size
     */
    T& at(size_t index) {
        return m_data.at(index);
    }

    /**
     * @brief Access element at index (const version)
     *
     * @param index Index to access
     * @return Const reference to element at index
     * @throws std::out_of_range if index >= size
     */
    const T& at(size_t index) const {
        return m_data.at(index);
    }

    /**
     * @brief Access element at index (unchecked)
     *
     * @param index Index to access
     * @return Reference to element at index
     */
    T& operator[](size_t index) {
        return m_data[index];
    }

    /**
     * @brief Access element at index (unchecked, const version)
     *
     * @param index Index to access
     * @return Const reference to element at index
     */
    const T& operator[](size_t index) const {
        return m_data[index];
    }

    /**
     * @brief Search for a value in the array (linear search)
     *
     * @param value Value to search for
     * @return Index of first occurrence, or std::nullopt if not found
     */
    std::optional<size_t> find(const T& value) const {
        auto it = std::find(m_data.begin(), m_data.end(), value);
        if (it != m_data.end()) {
            return static_cast<size_t>(std::distance(m_data.begin(), it));
        }
        return std::nullopt;
    }

    /**
     * @brief Update element at index
     *
     * @param index Index to update
     * @param value New value
     * @return true if successful, false if index out of range
     */
    bool update(size_t index, const T& value) {
        if (index >= m_data.size()) {
            return false;
        }
        m_data[index] = value;
        return true;
    }

    /**
     * @brief Check if array is empty
     *
     * @return true if empty
     */
    bool isEmpty() const {
        return m_data.empty();
    }

    /**
     * @brief Get current number of elements
     *
     * @return Number of elements
     */
    size_t size() const {
        return m_data.size();
    }

    /**
     * @brief Get current capacity
     *
     * @return Capacity (reserved space)
     */
    size_t capacity() const {
        return m_data.capacity();
    }

    /**
     * @brief Clear all elements
     */
    void clear() {
        m_data.clear();
    }

    /**
     * @brief Get direct access to internal data (for visualization)
     *
     * @return Const pointer to internal data
     */
    const T* data() const {
        return m_data.data();
    }

    /**
     * @brief Reserve capacity
     *
     * @param newCapacity New capacity to reserve
     */
    void reserve(size_t newCapacity) {
        m_data.reserve(newCapacity);
    }

    /**
     * @brief Get begin iterator (for range-based for)
     */
    typename std::vector<T>::iterator begin() {
        return m_data.begin();
    }

    /**
     * @brief Get end iterator (for range-based for)
     */
    typename std::vector<T>::iterator end() {
        return m_data.end();
    }

    /**
     * @brief Get begin const iterator (for range-based for)
     */
    typename std::vector<T>::const_iterator begin() const {
        return m_data.begin();
    }

    /**
     * @brief Get end const iterator (for range-based for)
     */
    typename std::vector<T>::const_iterator end() const {
        return m_data.end();
    }

private:
    std::vector<T> m_data;  ///< Internal dynamic array
};

} // namespace dsav
