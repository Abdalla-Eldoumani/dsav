/**
 * @file linked_list.hpp
 * @brief Singly linked list data structure implementation
 *
 * A template-based singly linked list with visualization-friendly interface.
 * Each node contains data and a pointer to the next node.
 */

#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <functional>

namespace dsav {

/**
 * @brief Node structure for singly linked list
 *
 * @tparam T Type of data stored in the node
 */
template<typename T>
struct ListNode {
    T data;
    std::shared_ptr<ListNode<T>> next;

    explicit ListNode(const T& value) : data(value), next(nullptr) {}
};

/**
 * @brief Singly linked list data structure
 *
 * Template parameter:
 * - T: Type of elements stored in the list
 *
 * Provides operations for inserting, deleting, and searching nodes.
 */
template<typename T>
class LinkedList {
public:
    /**
     * @brief Construct an empty linked list
     */
    LinkedList() = default;

    /**
     * @brief Insert a value at the front of the list
     *
     * @param value Value to insert
     */
    void insertFront(const T& value) {
        auto newNode = std::make_shared<ListNode<T>>(value);
        newNode->next = m_head;
        m_head = newNode;
        m_size++;
    }

    /**
     * @brief Insert a value at the back of the list
     *
     * @param value Value to insert
     */
    void insertBack(const T& value) {
        auto newNode = std::make_shared<ListNode<T>>(value);

        if (!m_head) {
            m_head = newNode;
        } else {
            auto current = m_head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        m_size++;
    }

    /**
     * @brief Insert a value at a specific position
     *
     * @param index Position to insert at (0-based)
     * @param value Value to insert
     * @return true if successful, false if index out of range
     */
    bool insertAt(size_t index, const T& value) {
        if (index > m_size) {
            return false;
        }

        if (index == 0) {
            insertFront(value);
            return true;
        }

        auto newNode = std::make_shared<ListNode<T>>(value);
        auto current = m_head;

        for (size_t i = 0; i < index - 1; ++i) {
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;
        m_size++;
        return true;
    }

    /**
     * @brief Delete the first node
     *
     * @return The deleted value if successful, std::nullopt if list is empty
     */
    std::optional<T> deleteFront() {
        if (!m_head) {
            return std::nullopt;
        }

        T value = m_head->data;
        m_head = m_head->next;
        m_size--;
        return value;
    }

    /**
     * @brief Delete the last node
     *
     * @return The deleted value if successful, std::nullopt if list is empty
     */
    std::optional<T> deleteBack() {
        if (!m_head) {
            return std::nullopt;
        }

        if (!m_head->next) {
            T value = m_head->data;
            m_head = nullptr;
            m_size--;
            return value;
        }

        auto current = m_head;
        while (current->next && current->next->next) {
            current = current->next;
        }

        T value = current->next->data;
        current->next = nullptr;
        m_size--;
        return value;
    }

    /**
     * @brief Delete node at a specific position
     *
     * @param index Position to delete (0-based)
     * @return The deleted value if successful, std::nullopt if index out of range
     */
    std::optional<T> deleteAt(size_t index) {
        if (index >= m_size || !m_head) {
            return std::nullopt;
        }

        if (index == 0) {
            return deleteFront();
        }

        auto current = m_head;
        for (size_t i = 0; i < index - 1; ++i) {
            current = current->next;
        }

        if (!current->next) {
            return std::nullopt;
        }

        T value = current->next->data;
        current->next = current->next->next;
        m_size--;
        return value;
    }

    /**
     * @brief Search for a value in the list
     *
     * @param value Value to search for
     * @return Index of first occurrence, or std::nullopt if not found
     */
    std::optional<size_t> find(const T& value) const {
        auto current = m_head;
        size_t index = 0;

        while (current) {
            if (current->data == value) {
                return index;
            }
            current = current->next;
            index++;
        }

        return std::nullopt;
    }

    /**
     * @brief Check if list is empty
     *
     * @return true if empty
     */
    bool isEmpty() const {
        return m_head == nullptr;
    }

    /**
     * @brief Get current number of nodes
     *
     * @return Number of nodes
     */
    size_t size() const {
        return m_size;
    }

    /**
     * @brief Clear all nodes
     */
    void clear() {
        m_head = nullptr;
        m_size = 0;
    }

    /**
     * @brief Get head node (for visualization)
     *
     * @return Shared pointer to head node
     */
    std::shared_ptr<ListNode<T>> head() const {
        return m_head;
    }

    /**
     * @brief Traverse list and apply function to each node
     *
     * @param func Function to apply to each node's data
     */
    void traverse(std::function<void(const T&)> func) const {
        auto current = m_head;
        while (current) {
            func(current->data);
            current = current->next;
        }
    }

private:
    std::shared_ptr<ListNode<T>> m_head = nullptr;  ///< Head of the list
    size_t m_size = 0;                              ///< Number of nodes
};

} // namespace dsav
