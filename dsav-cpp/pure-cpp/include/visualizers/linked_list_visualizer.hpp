/**
 * @file linked_list_visualizer.hpp
 * @brief Visualizer for Linked List data structure
 *
 * Provides interactive visualization of linked list operations with smooth animations.
 * Shows nodes as boxes with arrows representing pointer connections.
 */

#pragma once

#include "visualizer.hpp"
#include "data_structures/linked_list.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "color_scheme.hpp"
#include <vector>
#include <string>
#include <memory>
#include <imgui.h>

namespace dsav {

/**
 * @brief Visual representation of a linked list node
 */
struct VisualNode {
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 color;
    glm::vec4 borderColor;
    std::string label;
    bool hasNext;  // Whether this node has a next pointer
    bool isNull;   // Whether this is the NULL terminator node
};

/**
 * @brief Interactive visualizer for Linked List data structure
 *
 * Features:
 * - Horizontal layout showing nodes with pointer arrows
 * - Smooth animations for insert/delete operations
 * - Search animation highlighting nodes being checked
 * - HEAD pointer indicator
 * - NULL indicator at the end
 * - Interactive controls for list manipulation
 */
class LinkedListVisualizer : public IVisualizer {
public:
    /**
     * @brief Construct a linked list visualizer
     */
    LinkedListVisualizer();

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
    std::string getName() const override { return "Linked List"; }
    bool isAnimating() const override;
    bool isPaused() const override;

    // Linked list-specific operations (with animation)
    void insertFrontValue(int value);
    void insertBackValue(int value);
    void insertAtValue(size_t index, int value);
    void deleteFrontValue();
    void deleteBackValue();
    void deleteAtValue(size_t index);
    void searchValue(int value);
    void initializeRandom(size_t count);

private:
    /**
     * @brief Sync visual nodes with current list state
     */
    void syncVisuals();

    /**
     * @brief Calculate position for node at given index
     *
     * @param index Node index (0 = head)
     * @return Position for rendering
     */
    glm::vec2 calculatePosition(size_t index) const;

    /**
     * @brief Draw arrow from one node to another
     *
     * @param drawList ImGui draw list
     * @param from Starting position
     * @param to Ending position
     * @param color Arrow color
     */
    void drawArrow(ImDrawList* drawList, const ImVec2& from, const ImVec2& to, ImU32 color);

    // Data
    LinkedList<int> m_list;                    ///< Underlying linked list data structure
    std::vector<VisualNode> m_visualNodes;     ///< Visual representation of nodes
    AnimationController m_animator;            ///< Animation controller

    // UI state
    std::string m_statusText;                  ///< Current status message
    int m_inputValue = 0;                      ///< Value for insert/search
    int m_inputIndex = 0;                      ///< Index for insert/delete
    bool m_isPaused = true;                    ///< Pause state
    float m_speed = 1.0f;                      ///< Animation speed multiplier
    int m_initCount = 10;                      ///< Number of nodes for random initialization

    // Camera/viewport control for scrolling/panning
    float m_cameraOffsetX = 0.0f;              ///< Horizontal camera offset for panning
    float m_zoomLevel = 1.0f;                  ///< Zoom level (1.0 = normal, >1 = zoomed in, <1 = zoomed out)
    bool m_isDragging = false;                 ///< Mouse drag state for panning
    ImVec2 m_lastMousePos;                     ///< Last mouse position for drag delta

    // Operation mode
    enum class OperationMode {
        InsertFront,
        InsertBack,
        InsertAt,
        DeleteFront,
        DeleteBack,
        DeleteAt,
        Search
    };
    OperationMode m_currentMode = OperationMode::InsertFront;

    // Visual constants
    static constexpr float NODE_WIDTH = 80.0f;
    static constexpr float NODE_HEIGHT = 60.0f;
    static constexpr float NODE_SPACING = 100.0f;  // Horizontal spacing between nodes
    static constexpr float START_X = 150.0f;
    static constexpr float START_Y = 200.0f;
    static constexpr float ARROW_LENGTH = 40.0f;
};

} // namespace dsav
