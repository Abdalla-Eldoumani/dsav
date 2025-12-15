/**
 * @file searching.hpp
 * @brief Search algorithm implementations with step-by-step execution
 *
 * Provides stepper classes for search algorithms that allow
 * step-by-step visualization of the search process.
 */

#pragma once

#include <vector>
#include <optional>

namespace dsav::algorithms {

/**
 * @brief States for search visualization
 */
enum class SearchState {
    Checking,       ///< Checking current element
    Found,          ///< Target element found
    NotFound,       ///< Target not found in array
    Idle            ///< No active operation
};

/**
 * @brief Linear Search step-by-step executor
 *
 * Implements linear search with ability to execute one step at a time
 * for visualization purposes.
 */
class LinearSearchStepper {
public:
    /**
     * @brief Construct stepper with array and target
     * @param arr Reference to array to search
     * @param target Value to search for
     */
    LinearSearchStepper(const std::vector<int>& arr, int target);

    /**
     * @brief Execute one step of the algorithm
     * @return true if more steps remain, false if search is complete
     */
    bool step();

    /**
     * @brief Reset to initial state
     */
    void reset();

    /**
     * @brief Get current state
     */
    SearchState getState() const { return m_state; }

    /**
     * @brief Get current index being checked
     */
    int getCurrentIndex() const { return m_currentIdx; }

    /**
     * @brief Check if search is complete
     */
    bool isComplete() const { return m_complete; }

    /**
     * @brief Get result (-1 if not found, index if found)
     */
    int getResult() const { return m_result; }

private:
    const std::vector<int>& m_arr;
    int m_target;
    size_t m_n;
    int m_currentIdx = 0;
    int m_result = -1;
    bool m_complete = false;
    SearchState m_state = SearchState::Idle;
};

/**
 * @brief Binary Search step-by-step executor
 *
 * Implements binary search with ability to execute one step at a time
 * for visualization purposes. Assumes array is sorted.
 */
class BinarySearchStepper {
public:
    /**
     * @brief Construct stepper with sorted array and target
     * @param arr Reference to sorted array to search
     * @param target Value to search for
     */
    BinarySearchStepper(const std::vector<int>& arr, int target);

    /**
     * @brief Execute one step of the algorithm
     * @return true if more steps remain, false if search is complete
     */
    bool step();

    /**
     * @brief Reset to initial state
     */
    void reset();

    /**
     * @brief Get current state
     */
    SearchState getState() const { return m_state; }

    /**
     * @brief Get current middle index being checked
     */
    int getMidIndex() const { return m_mid; }

    /**
     * @brief Get current left bound
     */
    int getLeftBound() const { return m_left; }

    /**
     * @brief Get current right bound
     */
    int getRightBound() const { return m_right; }

    /**
     * @brief Check if search is complete
     */
    bool isComplete() const { return m_complete; }

    /**
     * @brief Get result (-1 if not found, index if found)
     */
    int getResult() const { return m_result; }

private:
    const std::vector<int>& m_arr;
    int m_target;
    size_t m_n;
    int m_left = 0;
    int m_right = 0;
    int m_mid = -1;
    int m_result = -1;
    bool m_complete = false;
    SearchState m_state = SearchState::Idle;
};

} // namespace dsav::algorithms
