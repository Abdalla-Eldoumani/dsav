/**
 * @file searching_visualizer.hpp
 * @brief Visualizer for search algorithms
 *
 * Provides interactive visualization of search algorithms with step-by-step
 * execution and animated element checking.
 */

#pragma once

#include "visualizer.hpp"
#include "algorithms/searching.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "color_scheme.hpp"
#include <vector>
#include <string>
#include <memory>
#include <imgui.h>

namespace dsav {

/**
 * @brief Visual representation of an array element during searching
 */
struct VisualSearchElement {
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 color;
    glm::vec4 borderColor;
    std::string label;
    int value;
    bool isChecked = false;
    bool isFound = false;
};

/**
 * @brief Interactive visualizer for search algorithms
 *
 * Features:
 * - Linear Search and Binary Search
 * - Step-by-step execution with animations
 * - Color-coded states (checking, found, not checked)
 * - Array initialization and target input
 * - Speed control and playback
 */
class SearchingVisualizer : public IVisualizer {
public:
    /**
     * @brief Construct a searching visualizer
     */
    SearchingVisualizer();

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
    std::string getName() const override { return "Search Algorithms"; }
    bool isAnimating() const override;
    bool isPaused() const override;

    // Search-specific operations
    void startSearch();
    void randomizeArray();
    void sortArray();  // For binary search
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
    std::vector<int> m_array;                          ///< Array being searched
    std::vector<VisualSearchElement> m_elements;       ///< Visual representation
    AnimationController m_animator;                    ///< Animation controller

    // Algorithm state
    enum class Algorithm {
        LinearSearch,
        BinarySearch
    };
    Algorithm m_currentAlgorithm = Algorithm::LinearSearch;

    std::unique_ptr<algorithms::LinearSearchStepper> m_linearSearcher;
    std::unique_ptr<algorithms::BinarySearchStepper> m_binarySearcher;

    // UI state
    std::string m_statusText;                          ///< Current status message
    bool m_isPaused = true;                            ///< Pause state
    bool m_isSearching = false;                        ///< Currently executing search
    float m_speed = 1.0f;                              ///< Animation speed multiplier
    int m_arraySize = 10;                              ///< Size of array to search
    int m_target = 50;                                 ///< Target value to find
    int m_stepDelay = 500;                             ///< Delay between steps (ms)
    float m_timeSinceLastStep = 0.0f;                  ///< Time accumulator for auto-step

    // Visual constants
    static constexpr float ELEMENT_WIDTH = 60.0f;
    static constexpr float ELEMENT_HEIGHT = 60.0f;
    static constexpr float ELEMENT_SPACING = 10.0f;
    static constexpr float START_X = 50.0f;
    static constexpr float START_Y = 300.0f;
    static constexpr int MAX_ARRAY_SIZE = 15;
    static constexpr int MAX_VALUE = 100;
};

} // namespace dsav
