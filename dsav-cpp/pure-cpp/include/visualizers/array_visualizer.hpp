/**
 * @file array_visualizer.hpp
 * @brief Visualizer for Dynamic Array data structure
 *
 * Provides interactive visualization of array operations with smooth animations.
 * Demonstrates random access, insertion, deletion, and search operations.
 */

#pragma once

#include "visualizer.hpp"
#include "data_structures/dynamic_array.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "color_scheme.hpp"
#include <vector>
#include <string>
#include <imgui.h>

namespace dsav {

/**
 * @brief Interactive visualizer for Dynamic Array data structure
 *
 * Features:
 * - Horizontal layout showing array elements with indices
 * - Smooth animations for insert (shift right), delete (shift left)
 * - Search animation highlighting elements being checked
 * - Access animation highlighting accessed element
 * - Interactive controls for array manipulation
 */
class ArrayVisualizer : public IVisualizer {
public:
    /**
     * @brief Construct an array visualizer
     */
    ArrayVisualizer();

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
    std::string getName() const override { return "Array"; }
    bool isAnimating() const override;
    bool isPaused() const override;

    // Array-specific operations (with animation)
    void insertValue(size_t index, int value);
    void deleteValue(size_t index);
    void searchValue(int value);
    void accessValue(size_t index);
    void updateValue(size_t index, int value);

private:
    /**
     * @brief Sync visual elements with current array state
     */
    void syncVisuals();

    /**
     * @brief Calculate position for element at given index
     *
     * @param index Array index
     * @return Position for rendering
     */
    glm::vec2 calculatePosition(size_t index) const;

    // Data
    DynamicArray<int> m_array;                 ///< Underlying array data structure
    std::vector<VisualElement> m_elements;     ///< Visual representation of array elements
    AnimationController m_animator;            ///< Animation controller

    // UI state
    std::string m_statusText;                  ///< Current status message
    int m_inputValue = 0;                      ///< Value for insert/update/search
    int m_inputIndex = 0;                      ///< Index for insert/delete/access/update
    bool m_isPaused = true;                    ///< Pause state
    float m_speed = 1.0f;                      ///< Animation speed multiplier

    // Operation mode
    enum class OperationMode {
        Insert,
        Delete,
        Search,
        Access,
        Update
    };
    OperationMode m_currentMode = OperationMode::Insert;

    // Visual constants
    static constexpr float ELEMENT_WIDTH = 80.0f;
    static constexpr float ELEMENT_HEIGHT = 60.0f;
    static constexpr float ELEMENT_SPACING = 10.0f;
    static constexpr float START_X = 100.0f;
    static constexpr float START_Y = 150.0f;
};

} // namespace dsav
