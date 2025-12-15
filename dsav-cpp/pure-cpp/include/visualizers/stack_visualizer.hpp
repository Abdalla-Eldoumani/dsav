/**
 * @file stack_visualizer.hpp
 * @brief Visualizer for Stack data structure
 *
 * Provides interactive visualization of stack operations with smooth animations.
 */

#pragma once

#include "visualizer.hpp"
#include "data_structures/stack.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "color_scheme.hpp"
#include <vector>
#include <string>
#include <imgui.h>

namespace dsav {

/**
 * @brief Interactive visualizer for Stack data structure
 *
 * Features:
 * - Visual representation of stack elements as colored boxes
 * - Smooth animations for push/pop operations
 * - Interactive controls for stack manipulation
 * - Real-time status updates
 */
class StackVisualizer : public IVisualizer {
public:
    /**
     * @brief Construct a stack visualizer
     *
     * @param maxSize Maximum stack capacity (default 10)
     */
    explicit StackVisualizer(size_t maxSize = 10);

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
    std::string getName() const override { return "Stack"; }
    bool isAnimating() const override;
    bool isPaused() const override;

    // Stack-specific operations (with animation)
    void pushValue(int value);
    void popValue();
    void peekValue();

private:
    /**
     * @brief Sync visual elements with current stack state
     *
     * Creates VisualElements for each item in the stack.
     */
    void syncVisuals();

    /**
     * @brief Calculate position for element at given index
     *
     * Stack grows upward, so higher indices are rendered higher on screen.
     *
     * @param index Stack index (0 = bottom)
     * @return Position for rendering
     */
    glm::vec2 calculatePosition(size_t index) const;

    // Data
    Stack<int, 16> m_stack;                    ///< Underlying stack data structure
    std::vector<VisualElement> m_elements;     ///< Visual representation of stack elements
    AnimationController m_animator;            ///< Animation controller

    // UI state
    std::string m_statusText;                  ///< Current status message
    int m_inputValue = 0;                      ///< Value to push (from UI input)
    bool m_isPaused = true;                    ///< Pause state
    float m_speed = 1.0f;                      ///< Animation speed multiplier

    // Visual constants
    static constexpr float ELEMENT_WIDTH = 120.0f;
    static constexpr float ELEMENT_HEIGHT = 60.0f;
    static constexpr float ELEMENT_SPACING = 10.0f;
    static constexpr float START_X = 100.0f;
    static constexpr float START_Y = 500.0f;  // Bottom of visualization area
};

} // namespace dsav
