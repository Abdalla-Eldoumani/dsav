/**
 * @file asm_stack_visualizer.hpp
 * @brief Stack visualizer using ARMv8 assembly backend
 *
 * This visualizer uses assembly functions for stack operations while
 * C++ handles visualization, animation, and UI.
 */

#pragma once

#include "visualizer.hpp"
#include "asm_interface.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "color_scheme.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <imgui.h>

namespace dsav {

/**
 * @brief Stack visualizer with assembly backend
 *
 * Calls ARMv8 assembly functions for core stack operations,
 * then syncs visual state from assembly memory.
 */
class AsmStackVisualizer : public IVisualizer {
public:
    /**
     * @brief Construct visualizer and initialize assembly stack
     */
    AsmStackVisualizer();

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
    std::string getName() const override { return "Stack (ASM)"; }
    bool isAnimating() const override;
    bool isPaused() const override;

    // Stack-specific operations (call assembly, then animate)
    void pushValue(int value);
    void popValue();
    void peekValue();

private:
    /**
     * @brief Sync visual elements with current assembly stack state
     *
     * Reads stack data from assembly using accessor functions,
     * then creates VisualElements for rendering.
     */
    void syncFromAssembly();

    /**
     * @brief Calculate position for element at given index
     *
     * @param index Stack index (0 = bottom)
     * @return Position for rendering
     */
    glm::vec2 calculatePosition(size_t index) const;

    // Visual representation
    std::vector<VisualElement> m_elements;     ///< Visual representation of stack elements
    AnimationController m_animator;            ///< Animation controller

    // UI state
    std::string m_statusText;                  ///< Current status message
    int m_inputValue = 0;                      ///< Value to push (from UI input)
    int m_lastPoppedValue = 0;                 ///< Last value popped (for display)
    int m_lastPeekedValue = 0;                 ///< Last value peeked (for display)
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
