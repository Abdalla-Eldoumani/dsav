/**
 * @file visualizer.hpp
 * @brief Base interface for all data structure visualizers
 *
 * Defines the common interface that all visualizers must implement.
 * Each visualizer manages rendering, animation, and interaction for a specific data structure.
 */

#pragma once

#include <string>

namespace dsav {

/**
 * @brief Base interface for data structure visualizers
 *
 * All visualizers (Stack, Queue, Array, etc.) inherit from this interface.
 * The visualizer is responsible for:
 * - Rendering the data structure visually
 * - Rendering UI controls for interaction
 * - Managing animations
 * - Updating state based on time
 */
class IVisualizer {
public:
    virtual ~IVisualizer() = default;

    /**
     * @brief Update the visualizer state
     *
     * Called every frame to update animations, timers, etc.
     *
     * @param deltaTime Time elapsed since last frame (in seconds)
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Render the data structure visualization
     *
     * This should draw the visual representation of the data structure
     * (e.g., stack elements, array cells, tree nodes) in the main visualization area.
     * Use ImGui's DrawList or OpenGL for rendering.
     */
    virtual void renderVisualization() = 0;

    /**
     * @brief Render ImGui control panel
     *
     * This should render interactive controls (buttons, inputs, sliders)
     * that allow the user to manipulate the data structure.
     */
    virtual void renderControls() = 0;

    /**
     * @brief Play the animation queue
     *
     * Resumes animation playback if paused.
     */
    virtual void play() = 0;

    /**
     * @brief Pause the animation queue
     *
     * Pauses animation playback without clearing the queue.
     */
    virtual void pause() = 0;

    /**
     * @brief Step forward one animation/operation
     *
     * Executes the next operation in the queue and pauses.
     */
    virtual void step() = 0;

    /**
     * @brief Reset the data structure to initial state
     *
     * Clears all data and resets animations.
     */
    virtual void reset() = 0;

    /**
     * @brief Set animation speed multiplier
     *
     * @param speed Speed multiplier (0.1 = slow, 1.0 = normal, 5.0 = fast)
     */
    virtual void setSpeed(float speed) = 0;

    /**
     * @brief Get current status text for display
     *
     * Returns a human-readable string describing the current state
     * (e.g., "Stack has 5 elements", "Pushing value 42")
     *
     * @return Status string
     */
    virtual std::string getStatusText() const = 0;

    /**
     * @brief Get the name of this visualizer
     *
     * @return Visualizer name (e.g., "Stack", "Binary Search Tree")
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Check if animations are currently running
     *
     * @return true if animations are in progress
     */
    virtual bool isAnimating() const = 0;

    /**
     * @brief Check if currently paused
     *
     * @return true if paused
     */
    virtual bool isPaused() const = 0;
};

} // namespace dsav
