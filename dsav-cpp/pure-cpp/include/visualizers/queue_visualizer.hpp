/**
 * @file queue_visualizer.hpp
 * @brief Visualizer for Queue data structure
 *
 * Provides interactive visualization of queue operations with smooth animations.
 * Displays queue as a horizontal arrangement with front and rear indicators.
 */

#pragma once

#include "visualizer.hpp"
#include "data_structures/queue.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "color_scheme.hpp"
#include <vector>
#include <string>
#include <imgui.h>

namespace dsav {

/**
 * @brief Interactive visualizer for Queue data structure
 *
 * Features:
 * - Horizontal layout showing elements from front to rear
 * - Circular buffer visualization with array indices
 * - Smooth animations for enqueue/dequeue operations
 * - Front and rear pointer indicators
 * - Interactive controls for queue manipulation
 */
class QueueVisualizer : public IVisualizer {
public:
    /**
     * @brief Construct a queue visualizer
     *
     * @param maxSize Maximum queue capacity (default 16)
     */
    explicit QueueVisualizer(size_t maxSize = 16);

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
    std::string getName() const override { return "Queue"; }
    bool isAnimating() const override;
    bool isPaused() const override;

    // Queue-specific operations (with animation)
    void enqueueValue(int value);
    void dequeueValue();
    void peekValue();
    void initializeRandom(size_t count);

private:
    /**
     * @brief Sync visual elements with current queue state
     *
     * Creates VisualElements for each item in the queue.
     */
    void syncVisuals();

    /**
     * @brief Calculate position for element at given array index
     *
     * Elements are arranged horizontally in a circular pattern.
     *
     * @param arrayIndex Array index in the circular buffer
     * @return Position for rendering
     */
    glm::vec2 calculatePosition(size_t arrayIndex) const;

    // Data
    Queue<int, 16> m_queue;                    ///< Underlying queue data structure
    std::vector<VisualElement> m_elements;     ///< Visual representation of queue elements
    AnimationController m_animator;            ///< Animation controller

    // UI state
    std::string m_statusText;                  ///< Current status message
    int m_inputValue = 0;                      ///< Value to enqueue (from UI input)
    bool m_isPaused = true;                    ///< Pause state
    float m_speed = 1.0f;                      ///< Animation speed multiplier
    int m_initCount = 16;                      ///< Number of elements for random initialization

    // Camera/viewport control for scrolling/panning
    float m_cameraOffsetX = 0.0f;              ///< Horizontal camera offset for panning
    float m_zoomLevel = 1.0f;                  ///< Zoom level (1.0 = normal, >1 = zoomed in, <1 = zoomed out)
    bool m_isDragging = false;                 ///< Mouse drag state for panning
    ImVec2 m_lastMousePos;                     ///< Last mouse position for drag delta

    // Visual constants
    static constexpr float ELEMENT_WIDTH = 80.0f;
    static constexpr float ELEMENT_HEIGHT = 60.0f;
    static constexpr float ELEMENT_SPACING = 15.0f;
    static constexpr float START_X = 150.0f;
    static constexpr float START_Y = 100.0f;
};

} // namespace dsav
