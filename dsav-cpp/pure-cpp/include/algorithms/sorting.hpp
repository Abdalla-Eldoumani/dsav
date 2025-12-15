/**
 * @file sorting.hpp
 * @brief Sorting algorithm implementations with step-by-step execution
 *
 * Provides stepper classes for sorting algorithms that allow
 * step-by-step visualization of the sorting process.
 */

#pragma once

#include <vector>
#include <functional>

namespace dsav::algorithms {

/**
 * @brief States for sorting visualization
 */
enum class SortState {
    Comparing,      ///< Comparing two elements
    Swapping,       ///< Swapping two elements
    Sorted,         ///< Element is in final sorted position
    Idle            ///< No active operation
};

/**
 * @brief Bubble Sort step-by-step executor
 *
 * Implements bubble sort with ability to execute one step at a time
 * for visualization purposes.
 */
class BubbleSortStepper {
public:
    /**
     * @brief Construct stepper with array to sort
     * @param arr Reference to array (will be modified in-place)
     */
    explicit BubbleSortStepper(std::vector<int>& arr);

    /**
     * @brief Execute one step of the algorithm
     * @return true if more steps remain, false if sorting is complete
     */
    bool step();

    /**
     * @brief Reset to initial state
     */
    void reset();

    /**
     * @brief Get current state
     */
    SortState getState() const { return m_state; }

    /**
     * @brief Get indices being compared/swapped
     */
    int getIndexI() const { return m_currentI; }
    int getIndexJ() const { return m_currentJ; }

    /**
     * @brief Check if sorting is complete
     */
    bool isComplete() const { return m_sorted; }

    /**
     * @brief Get indices that are known to be in final sorted position
     */
    const std::vector<int>& getSortedIndices() const { return m_sortedIndices; }

private:
    std::vector<int>& m_arr;
    size_t m_n;
    size_t m_i = 0;
    size_t m_j = 0;
    bool m_swapped = false;
    bool m_sorted = false;
    SortState m_state = SortState::Idle;
    int m_currentI = -1;
    int m_currentJ = -1;
    std::vector<int> m_sortedIndices;  // Indices known to be sorted
};

/**
 * @brief Selection Sort step-by-step executor
 *
 * Implements selection sort with ability to execute one step at a time
 * for visualization purposes.
 */
class SelectionSortStepper {
public:
    explicit SelectionSortStepper(std::vector<int>& arr);
    bool step();
    void reset();

    SortState getState() const { return m_state; }
    int getCurrentIndex() const { return static_cast<int>(m_i); }
    int getMinIndex() const { return static_cast<int>(m_minIdx); }
    int getCompareIndex() const { return static_cast<int>(m_j); }
    bool isComplete() const { return m_sorted; }
    const std::vector<int>& getSortedIndices() const { return m_sortedIndices; }

private:
    std::vector<int>& m_arr;
    size_t m_n;
    size_t m_i = 0;
    size_t m_j = 0;
    size_t m_minIdx = 0;
    bool m_findingMin = true;
    bool m_sorted = false;
    SortState m_state = SortState::Idle;
    std::vector<int> m_sortedIndices;
};

/**
 * @brief Insertion Sort step-by-step executor
 *
 * Implements insertion sort with ability to execute one step at a time
 * for visualization purposes.
 */
class InsertionSortStepper {
public:
    explicit InsertionSortStepper(std::vector<int>& arr);
    bool step();
    void reset();

    SortState getState() const { return m_state; }
    int getCurrentIndex() const { return m_i; }
    int getCompareIndex() const { return m_j; }
    bool isComplete() const { return m_sorted; }
    const std::vector<int>& getSortedIndices() const { return m_sortedIndices; }

private:
    std::vector<int>& m_arr;
    size_t m_n;
    int m_i = 1;
    int m_j = 0;
    int m_key = 0;
    bool m_sorted = false;
    SortState m_state = SortState::Idle;
    std::vector<int> m_sortedIndices;
};

/**
 * @brief Merge Sort step-by-step executor
 *
 * Implements merge sort with ability to execute one step at a time
 * for visualization purposes. Uses iterative approach with explicit stack.
 */
class MergeSortStepper {
public:
    explicit MergeSortStepper(std::vector<int>& arr);
    bool step();
    void reset();

    SortState getState() const { return m_state; }
    int getLeftIndex() const { return m_currentLeft; }
    int getRightIndex() const { return m_currentRight; }
    int getMidIndex() const { return m_currentMid; }
    bool isComplete() const { return m_sorted; }
    const std::vector<int>& getSortedIndices() const { return m_sortedIndices; }

private:
    struct MergeRange {
        int left;
        int right;
        bool isMerging;
    };

    void merge(int left, int mid, int right);

    std::vector<int>& m_arr;
    size_t m_n;
    std::vector<MergeRange> m_stack;
    int m_currentSize = 1;
    int m_currentLeft = -1;
    int m_currentRight = -1;
    int m_currentMid = -1;
    bool m_sorted = false;
    SortState m_state = SortState::Idle;
    std::vector<int> m_sortedIndices;
};

/**
 * @brief Quick Sort step-by-step executor
 *
 * Implements quick sort with ability to execute one step at a time
 * for visualization purposes. Uses iterative approach with explicit stack.
 */
class QuickSortStepper {
public:
    explicit QuickSortStepper(std::vector<int>& arr);
    bool step();
    void reset();

    SortState getState() const { return m_state; }
    int getPivotIndex() const { return m_pivotIdx; }
    int getLeftIndex() const { return m_leftIdx; }
    int getRightIndex() const { return m_rightIdx; }
    bool isComplete() const { return m_sorted; }
    const std::vector<int>& getSortedIndices() const { return m_sortedIndices; }

private:
    struct PartitionRange {
        int low;
        int high;
    };

    int partition(int low, int high);

    std::vector<int>& m_arr;
    size_t m_n;
    std::vector<PartitionRange> m_stack;
    int m_pivotIdx = -1;
    int m_leftIdx = -1;
    int m_rightIdx = -1;
    int m_partitionLow = -1;
    int m_partitionHigh = -1;
    bool m_isPartitioning = false;
    bool m_sorted = false;
    SortState m_state = SortState::Idle;
    std::vector<int> m_sortedIndices;
};

} // namespace dsav::algorithms
