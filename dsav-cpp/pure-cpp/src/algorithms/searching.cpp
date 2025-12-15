/**
 * @file searching.cpp
 * @brief Implementation of search algorithm steppers
 */

#include "algorithms/searching.hpp"

namespace dsav::algorithms {

// ===== LinearSearchStepper =====

LinearSearchStepper::LinearSearchStepper(const std::vector<int>& arr, int target)
    : m_arr(arr), m_target(target), m_n(arr.size()) {
}

bool LinearSearchStepper::step() {
    if (m_complete) {
        return false;
    }

    if (m_currentIdx >= static_cast<int>(m_n)) {
        // Reached end without finding target
        m_state = SearchState::NotFound;
        m_result = -1;
        m_complete = true;
        return false;
    }

    // Check current element
    m_state = SearchState::Checking;

    if (m_arr[m_currentIdx] == m_target) {
        // Found target
        m_state = SearchState::Found;
        m_result = m_currentIdx;
        m_complete = true;
        return false;
    }

    // Move to next element
    m_currentIdx++;

    return true;
}

void LinearSearchStepper::reset() {
    m_currentIdx = 0;
    m_result = -1;
    m_complete = false;
    m_state = SearchState::Idle;
}

// ===== BinarySearchStepper =====

BinarySearchStepper::BinarySearchStepper(const std::vector<int>& arr, int target)
    : m_arr(arr), m_target(target), m_n(arr.size()) {
    m_left = 0;
    m_right = static_cast<int>(m_n) - 1;
}

bool BinarySearchStepper::step() {
    if (m_complete) {
        return false;
    }

    if (m_left > m_right) {
        // Search space exhausted - target not found
        m_state = SearchState::NotFound;
        m_result = -1;
        m_complete = true;
        return false;
    }

    // Calculate mid point
    m_mid = m_left + (m_right - m_left) / 2;
    m_state = SearchState::Checking;

    if (m_arr[m_mid] == m_target) {
        // Found target
        m_state = SearchState::Found;
        m_result = m_mid;
        m_complete = true;
        return false;
    } else if (m_arr[m_mid] < m_target) {
        // Target is in right half
        m_left = m_mid + 1;
    } else {
        // Target is in left half
        m_right = m_mid - 1;
    }

    return true;
}

void BinarySearchStepper::reset() {
    m_left = 0;
    m_right = static_cast<int>(m_n) - 1;
    m_mid = -1;
    m_result = -1;
    m_complete = false;
    m_state = SearchState::Idle;
}

} // namespace dsav::algorithms
