/**
 * @file bst_visualizer.hpp
 * @brief Visualizer for Binary Search Tree data structure
 *
 * Provides interactive visualization of BST operations with hierarchical tree layout.
 * Shows parent-child relationships with connecting lines and supports tree traversals.
 */

#pragma once

#include "visualizer.hpp"
#include "data_structures/binary_search_tree.hpp"
#include "animation.hpp"
#include "renderer.hpp"
#include "color_scheme.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <imgui.h>

namespace dsav {

/**
 * @brief Visual representation of a tree node
 */
struct VisualTreeNode {
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 color;
    glm::vec4 borderColor;
    std::string label;
    int value;  // Store value for identification
};

/**
 * @brief Interactive visualizer for Binary Search Tree data structure
 *
 * Features:
 * - Hierarchical tree layout with automatic positioning
 * - Lines connecting parent to child nodes
 * - Smooth animations for insert/delete/search operations
 * - Tree traversal animations (inorder, preorder, postorder, level-order)
 * - Interactive controls for BST manipulation
 */
class BSTVisualizer : public IVisualizer {
public:
    /**
     * @brief Construct a BST visualizer
     */
    BSTVisualizer();

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
    std::string getName() const override { return "Binary Search Tree"; }
    bool isAnimating() const override;
    bool isPaused() const override;

    // BST-specific operations (with animation)
    void insertValue(int value);
    void deleteValue(int value);
    void searchValue(int value);
    void traverseInorder();
    void traversePreorder();
    void traversePostorder();
    void traverseLevelOrder();
    void initializeRandom(size_t count);

private:
    /**
     * @brief Sync visual nodes with current tree state
     */
    void syncVisuals();

    /**
     * @brief Calculate positions for all nodes using tree layout algorithm
     *
     * @param node Current node
     * @param x X position
     * @param y Y position (level)
     * @param xOffset Horizontal offset for this subtree
     */
    void calculatePositions(std::shared_ptr<TreeNode<int>> node, float x, float y, float xOffset);

    /**
     * @brief Draw line connecting parent to child
     *
     * @param drawList ImGui draw list
     * @param from Parent position
     * @param to Child position
     * @param color Line color
     */
    void drawConnection(ImDrawList* drawList, const ImVec2& from, const ImVec2& to, ImU32 color);

    /**
     * @brief Collect nodes for traversal animation
     *
     * Different orderings based on traversal type
     */
    std::vector<int> collectTraversalOrder(const std::string& type);

    // Data
    BinarySearchTree<int> m_bst;                      ///< Underlying BST data structure
    std::vector<VisualTreeNode> m_visualNodes;        ///< Visual representation of nodes
    std::map<int, glm::vec2> m_nodePositions;         ///< Position map for nodes
    AnimationController m_animator;                   ///< Animation controller

    // UI state
    std::string m_statusText;                         ///< Current status message
    int m_inputValue = 0;                             ///< Value for insert/delete/search
    bool m_isPaused = true;                           ///< Pause state
    float m_speed = 1.0f;                             ///< Animation speed multiplier
    int m_initCount = 10;                             ///< Number of nodes for random initialization

    // Camera/viewport control for scrolling/panning
    float m_cameraOffsetX = 0.0f;                     ///< Horizontal camera offset for panning
    float m_cameraOffsetY = 0.0f;                     ///< Vertical camera offset for panning
    float m_zoomLevel = 1.0f;                         ///< Zoom level (1.0 = normal, >1 = zoomed in, <1 = zoomed out)
    bool m_isDragging = false;                        ///< Mouse drag state for panning
    ImVec2 m_lastMousePos;                            ///< Last mouse position for drag delta

    // Operation mode
    enum class OperationMode {
        Insert,
        Delete,
        Search,
        TraverseInorder,
        TraversePreorder,
        TraversePostorder,
        TraverseLevelOrder,
        Initialize
    };
    OperationMode m_currentMode = OperationMode::Insert;

    // Visual constants
    static constexpr float NODE_RADIUS = 25.0f;
    static constexpr float VERTICAL_SPACING = 80.0f;
    static constexpr float HORIZONTAL_SPACING = 60.0f;
    static constexpr float START_X = 400.0f;
    static constexpr float START_Y = 80.0f;
};

} // namespace dsav
