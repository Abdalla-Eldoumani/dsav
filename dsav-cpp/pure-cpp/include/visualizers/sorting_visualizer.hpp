/**
 * @file sorting_visualizer.hpp
 * @brief Visualizer for sorting algorithms
 *
 * Provides interactive visualization of sorting algorithms with step-by-step
 * execution and animated comparisons/swaps.
 */

#pragma once

#include "visualizer.hpp"
#include "algorithms/sorting.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "color_scheme.hpp"
#include <vector>
#include <string>
#include <memory>
#include <imgui.h>

namespace dsav {

/**
 * @brief Visual representation of an array element during sorting
 */
struct VisualSortElement {
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 color;
    glm::vec4 borderColor;
    std::string label;
    int value;
    bool isSorted = false;
};

/**
 * @brief Interactive visualizer for sorting algorithms
 *
 * Features:
 * - Multiple sorting algorithms (Bubble, Selection, Insertion)
 * - Step-by-step execution with animations
 * - Color-coded states (comparing, swapping, sorted)
 * - Array randomization and custom input
 * - Speed control and playback
 */
class SortingVisualizer : public IVisualizer {
public:
    /**
     * @brief Construct a sorting visualizer
     */
    SortingVisualizer();

    // IVisualizer interface implementation
    void update(float deltaTime) override;
    void renderVisualization() override;
    void renderControls() override;
    void play() override;
    void pause() override;
    void step() override;
    void reset() override;
    void setSpeed(float speed) override;
    std::string getStatusText() const override;
    std::string getName() const override { return "Sorting Algorithms"; }
    bool isAnimating() const override;
    bool isPaused() const override;

    // Sorting-specific operations
    void startSort();
    void randomizeArray();
    void setArray(const std::vector<int>& arr);

private:
    /**
     * @brief Sync visual elements with current array state
     */
    void syncVisuals();

    /**
     * @brief Calculate position for element at index
     */
    glm::vec2 calculatePosition(size_t index) const;

    /**
     * @brief Execute one step of the current algorithm
     */
    void executeStep();

    /**
     * @brief Update element colors based on current state
     */
    void updateColors();

    // Data
    std::vector<int> m_array;                          ///< Array being sorted
    std::vector<VisualSortElement> m_elements;         ///< Visual representation
    AnimationController m_animator;                    ///< Animation controller

    // Algorithm state
    enum class Algorithm {
        BubbleSort,
        SelectionSort,
        InsertionSort,
        MergeSort,
        QuickSort
    };
    Algorithm m_currentAlgorithm = Algorithm::BubbleSort;

    std::unique_ptr<algorithms::BubbleSortStepper> m_bubbleSorter;
    std::unique_ptr<algorithms::SelectionSortStepper> m_selectionSorter;
    std::unique_ptr<algorithms::InsertionSortStepper> m_insertionSorter;
    std::unique_ptr<algorithms::MergeSortStepper> m_mergeSorter;
    std::unique_ptr<algorithms::QuickSortStepper> m_quickSorter;

    // UI state
    std::string m_statusText;                          ///< Current status message
    bool m_isPaused = true;                            ///< Pause state
    bool m_isSorting = false;                          ///< Currently executing sort
    float m_speed = 1.0f;                              ///< Animation speed multiplier
    int m_arraySize = 10;                              ///< Size of array to sort
    int m_stepDelay = 500;                             ///< Delay between steps (ms)
    float m_timeSinceLastStep = 0.0f;                  ///< Time accumulator for auto-step

    // Visual constants
    static constexpr float ELEMENT_WIDTH = 50.0f;
    static constexpr float ELEMENT_HEIGHT_SCALE = 5.0f;  // Height multiplier for value
    static constexpr float ELEMENT_SPACING = 10.0f;
    static constexpr float START_X = 100.0f;
    static constexpr float BASE_Y = 500.0f;              // Baseline for bars
    static constexpr int MAX_ARRAY_SIZE = 20;
    static constexpr int MAX_VALUE = 100;
};

} // namespace dsav
