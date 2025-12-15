/**
 * @file sorting.cpp
 * @brief Implementation of sorting algorithm steppers
 */

#include "algorithms/sorting.hpp"
#include <algorithm>

namespace dsav::algorithms {

// ===== BubbleSortStepper =====

BubbleSortStepper::BubbleSortStepper(std::vector<int>& arr)
    : m_arr(arr), m_n(arr.size()) {
    m_sortedIndices.reserve(m_n);
}

bool BubbleSortStepper::step() {
    if (m_sorted) {
        return false;
    }

    // Compare current adjacent elements
    m_state = SortState::Comparing;
    m_currentI = static_cast<int>(m_i);
    m_currentJ = static_cast<int>(m_j);

    if (m_arr[m_j] > m_arr[m_j + 1]) {
        // Need to swap
        m_state = SortState::Swapping;
        std::swap(m_arr[m_j], m_arr[m_j + 1]);
        m_swapped = true;
    }

    // Advance to next comparison
    m_j++;

    // Check if we completed a pass
    if (m_j >= m_n - 1 - m_i) {
        // Mark the last element as sorted (it's in final position)
        m_sortedIndices.push_back(static_cast<int>(m_n - 1 - m_i));

        if (!m_swapped) {
            // No swaps in this pass - array is sorted
            // Mark all remaining elements as sorted
            for (int idx = 0; idx < static_cast<int>(m_n - 1 - m_i); ++idx) {
                bool alreadySorted = false;
                for (int sorted : m_sortedIndices) {
                    if (sorted == idx) {
                        alreadySorted = true;
                        break;
                    }
                }
                if (!alreadySorted) {
                    m_sortedIndices.push_back(idx);
                }
            }
            m_sorted = true;
            m_state = SortState::Sorted;
            return false;
        }

        m_swapped = false;
        m_i++;
        m_j = 0;

        if (m_i >= m_n - 1) {
            // Completed all passes
            // Mark all elements as sorted
            for (size_t idx = 0; idx < m_n; ++idx) {
                bool alreadySorted = false;
                for (int sorted : m_sortedIndices) {
                    if (sorted == static_cast<int>(idx)) {
                        alreadySorted = true;
                        break;
                    }
                }
                if (!alreadySorted) {
                    m_sortedIndices.push_back(static_cast<int>(idx));
                }
            }
            m_sorted = true;
            m_state = SortState::Sorted;
            return false;
        }
    }

    return true;
}

void BubbleSortStepper::reset() {
    m_i = 0;
    m_j = 0;
    m_swapped = false;
    m_sorted = false;
    m_state = SortState::Idle;
    m_currentI = -1;
    m_currentJ = -1;
    m_sortedIndices.clear();
}

// ===== SelectionSortStepper =====

SelectionSortStepper::SelectionSortStepper(std::vector<int>& arr)
    : m_arr(arr), m_n(arr.size()) {
    m_sortedIndices.reserve(m_n);
}

bool SelectionSortStepper::step() {
    if (m_sorted) {
        return false;
    }

    if (m_findingMin) {
        // Finding minimum in unsorted portion
        m_state = SortState::Comparing;

        if (m_j == m_i) {
            // Start of new pass - initialize min index
            m_minIdx = m_i;
        }

        // Check if current element is smaller than current minimum
        if (m_arr[m_j] < m_arr[m_minIdx]) {
            m_minIdx = m_j;
        }

        m_j++;

        // Check if we've scanned the entire unsorted portion
        if (m_j >= m_n) {
            // Found minimum, now swap it
            m_findingMin = false;
            m_j = m_i;  // Reset for next pass
        }
    } else {
        // Swap minimum with current position
        if (m_minIdx != m_i) {
            m_state = SortState::Swapping;
            std::swap(m_arr[m_i], m_arr[m_minIdx]);
        }

        // Mark current position as sorted
        m_sortedIndices.push_back(static_cast<int>(m_i));

        // Move to next position
        m_i++;
        m_findingMin = true;

        if (m_i >= m_n - 1) {
            // All elements sorted (last one is automatically in place)
            m_sortedIndices.push_back(static_cast<int>(m_n - 1));
            m_sorted = true;
            m_state = SortState::Sorted;
            return false;
        }

        m_j = m_i;
        m_minIdx = m_i;
    }

    return true;
}

void SelectionSortStepper::reset() {
    m_i = 0;
    m_j = 0;
    m_minIdx = 0;
    m_findingMin = true;
    m_sorted = false;
    m_state = SortState::Idle;
    m_sortedIndices.clear();
}

// ===== InsertionSortStepper =====

InsertionSortStepper::InsertionSortStepper(std::vector<int>& arr)
    : m_arr(arr), m_n(arr.size()) {
    // First element is considered sorted
    if (m_n > 0) {
        m_sortedIndices.push_back(0);
    }
}

bool InsertionSortStepper::step() {
    if (m_sorted) {
        return false;
    }

    if (m_i >= static_cast<int>(m_n)) {
        m_sorted = true;
        m_state = SortState::Sorted;
        return false;
    }

    // First step of inserting arr[i] into sorted portion
    if (m_j == m_i - 1) {
        m_key = m_arr[m_i];
        m_j = m_i - 1;
    }

    // Compare and shift
    if (m_j >= 0 && m_arr[m_j] > m_key) {
        m_state = SortState::Swapping;
        m_arr[m_j + 1] = m_arr[m_j];
        m_j--;
    } else {
        // Found the correct position - insert key
        m_arr[m_j + 1] = m_key;

        // Mark newly inserted position as sorted
        m_sortedIndices.push_back(m_i);

        // Move to next element
        m_i++;

        if (m_i >= static_cast<int>(m_n)) {
            m_sorted = true;
            m_state = SortState::Sorted;
            return false;
        }

        m_j = m_i - 1;
        m_state = SortState::Comparing;
    }

    return true;
}

void InsertionSortStepper::reset() {
    m_i = 1;
    m_j = 0;
    m_key = 0;
    m_sorted = false;
    m_state = SortState::Idle;
    m_sortedIndices.clear();
    if (m_n > 0) {
        m_sortedIndices.push_back(0);
    }
}

// ===== MergeSortStepper =====

MergeSortStepper::MergeSortStepper(std::vector<int>& arr)
    : m_arr(arr), m_n(arr.size()) {
    m_sortedIndices.reserve(m_n);
}

bool MergeSortStepper::step() {
    if (m_sorted) {
        return false;
    }

    // Bottom-up iterative merge sort
    // Process subarrays of size m_currentSize
    if (m_currentLeft == -1) {
        // Start new pass with current size
        m_currentLeft = 0;
    }

    if (m_currentLeft >= static_cast<int>(m_n) - 1) {
        // Finished this pass, double the size
        m_currentSize *= 2;
        m_currentLeft = -1;

        if (m_currentSize >= static_cast<int>(m_n)) {
            // Sorting complete
            for (size_t i = 0; i < m_n; ++i) {
                m_sortedIndices.push_back(static_cast<int>(i));
            }
            m_sorted = true;
            m_state = SortState::Sorted;
            return false;
        }
        return true;
    }

    // Calculate mid and right for current merge
    m_currentMid = std::min(m_currentLeft + m_currentSize - 1, static_cast<int>(m_n) - 1);
    m_currentRight = std::min(m_currentLeft + 2 * m_currentSize - 1, static_cast<int>(m_n) - 1);

    // Perform merge
    m_state = SortState::Comparing;
    merge(m_currentLeft, m_currentMid, m_currentRight);

    // Mark merged range as sorted for this pass
    for (int i = m_currentLeft; i <= m_currentRight; ++i) {
        bool alreadySorted = false;
        for (int sorted : m_sortedIndices) {
            if (sorted == i) {
                alreadySorted = true;
                break;
            }
        }
        if (!alreadySorted && m_currentSize >= static_cast<int>(m_n) / 2) {
            m_sortedIndices.push_back(i);
        }
    }

    // Move to next subarray
    m_currentLeft += 2 * m_currentSize;

    return true;
}

void MergeSortStepper::merge(int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left;
    int j = mid + 1;
    int k = 0;

    m_state = SortState::Comparing;

    // Merge two sorted subarrays
    while (i <= mid && j <= right) {
        if (m_arr[i] <= m_arr[j]) {
            temp[k++] = m_arr[i++];
        } else {
            temp[k++] = m_arr[j++];
            m_state = SortState::Swapping;
        }
    }

    // Copy remaining elements
    while (i <= mid) {
        temp[k++] = m_arr[i++];
    }
    while (j <= right) {
        temp[k++] = m_arr[j++];
    }

    // Copy back to original array
    for (int idx = 0; idx < k; ++idx) {
        m_arr[left + idx] = temp[idx];
    }
}

void MergeSortStepper::reset() {
    m_currentSize = 1;
    m_currentLeft = -1;
    m_currentRight = -1;
    m_currentMid = -1;
    m_sorted = false;
    m_state = SortState::Idle;
    m_sortedIndices.clear();
}

// ===== QuickSortStepper =====

QuickSortStepper::QuickSortStepper(std::vector<int>& arr)
    : m_arr(arr), m_n(arr.size()) {
    m_sortedIndices.reserve(m_n);
    // Initialize with full array range
    if (m_n > 0) {
        m_stack.push_back({0, static_cast<int>(m_n) - 1});
    }
}

bool QuickSortStepper::step() {
    if (m_sorted) {
        return false;
    }

    if (m_stack.empty()) {
        // All partitions processed
        for (size_t i = 0; i < m_n; ++i) {
            m_sortedIndices.push_back(static_cast<int>(i));
        }
        m_sorted = true;
        m_state = SortState::Sorted;
        return false;
    }

    // Pop next range to partition
    PartitionRange range = m_stack.back();
    m_stack.pop_back();

    if (range.low < range.high) {
        // Partition the array
        m_state = SortState::Comparing;
        m_partitionLow = range.low;
        m_partitionHigh = range.high;

        int pivotIdx = partition(range.low, range.high);
        m_pivotIdx = pivotIdx;

        // Mark pivot as in final sorted position
        m_sortedIndices.push_back(pivotIdx);

        // Push left partition
        if (pivotIdx - 1 > range.low) {
            m_stack.push_back({range.low, pivotIdx - 1});
        } else {
            // Mark single element as sorted
            if (range.low == pivotIdx - 1) {
                m_sortedIndices.push_back(range.low);
            }
        }

        // Push right partition
        if (pivotIdx + 1 < range.high) {
            m_stack.push_back({pivotIdx + 1, range.high});
        } else {
            // Mark single element as sorted
            if (range.high == pivotIdx + 1) {
                m_sortedIndices.push_back(range.high);
            }
        }
    } else if (range.low == range.high) {
        // Single element is already sorted
        m_sortedIndices.push_back(range.low);
    }

    return true;
}

int QuickSortStepper::partition(int low, int high) {
    // Choose last element as pivot
    int pivot = m_arr[high];
    m_pivotIdx = high;
    int i = low - 1;

    m_state = SortState::Comparing;

    for (int j = low; j < high; ++j) {
        m_leftIdx = j;
        m_rightIdx = high;

        if (m_arr[j] < pivot) {
            i++;
            if (i != j) {
                m_state = SortState::Swapping;
                std::swap(m_arr[i], m_arr[j]);
            }
        }
    }

    // Place pivot in correct position
    if (i + 1 != high) {
        m_state = SortState::Swapping;
        std::swap(m_arr[i + 1], m_arr[high]);
    }

    return i + 1;
}

void QuickSortStepper::reset() {
    m_stack.clear();
    if (m_n > 0) {
        m_stack.push_back({0, static_cast<int>(m_n) - 1});
    }
    m_pivotIdx = -1;
    m_leftIdx = -1;
    m_rightIdx = -1;
    m_partitionLow = -1;
    m_partitionHigh = -1;
    m_isPartitioning = false;
    m_sorted = false;
    m_state = SortState::Idle;
    m_sortedIndices.clear();
}

} // namespace dsav::algorithms
