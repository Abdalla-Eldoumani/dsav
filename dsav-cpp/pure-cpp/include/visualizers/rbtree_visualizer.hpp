/**
 * @file rbtree_visualizer.hpp
 * @brief Visualizer for Red-Black Tree data structure
 *
 * Provides interactive visualization of RB tree operations with animated rotations,
 * recoloring, and case explanations for educational purposes.
 */

#pragma once

#include "visualizer.hpp"
#include "data_structures/red_black_tree.hpp"
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
 * @brief Visual representation of an RB tree node
 */
struct VisualRBTreeNode {
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 color;           // Fill color (animated)
    glm::vec4 borderColor;     // Border shows RED/BLACK
    std::string label;         // Value display
    int value;                 // Store value for identification
    RBColor rbColor;           // RED or BLACK
    bool isNIL;                // True for NIL sentinel nodes
};

/**
 * @brief Insertion fixup case information
 */
struct FixupCaseInfo {
    std::string caseName;      // e.g., "Case 1: Uncle RED", "Case 3: Line"
    std::string explanation;   // Detailed explanation
    std::string nodeRoles;     // "z=42, p=30, g=50, u=70"
};

/**
 * @brief Interactive visualizer for Red-Black Tree data structure
 *
 * Features:
 * - Hierarchical tree layout with automatic positioning
 * - Red/Black border colors for node distinction
 * - Optional NIL leaf node visualization
 * - Animated rotations (smooth node movements)
 * - Animated recoloring
 * - Case explanation panel showing fixup details
 * - Interactive controls for RB tree manipulation
 */
class RBTreeVisualizer : public IVisualizer {
public:
    /**
     * @brief Construct an RB tree visualizer
     */
    RBTreeVisualizer();

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
    std::string getName() const override { return "Red-Black Tree"; }
    bool isAnimating() const override;
    bool isPaused() const override;

    // RB tree-specific operations (with animation)
    void insertValue(int value);
    void deleteValue(int value);
    void searchValue(int value);
    void traverseInorder();
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
    void calculatePositions(std::shared_ptr<RBTreeNode<int>> node, float x, float y, float xOffset);

    /**
     * @brief Add NIL nodes to visualization (for educational purposes)
     *
     * @param parent Parent node
     * @param isLeft True if NIL is left child
     */
    void addNILNode(std::shared_ptr<RBTreeNode<int>> parent, bool isLeft);

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
     * @brief Get border color based on RB color
     */
    glm::vec4 getBorderColor(RBColor color) const;

    /**
     * @brief Collect nodes for inorder traversal animation
     */
    std::vector<int> collectInorderTraversal();

    /**
     * @brief Create animation sequence for insertion
     *
     * Highlights the insertion path and demonstrates fixup cases
     */
    void animateInsertion(int value);

    /**
     * @brief Create animation for rotation
     *
     * @param pivotValue Value of node being rotated
     * @param isLeftRotation True for left rotation, false for right
     */
    void animateRotation(int pivotValue, bool isLeftRotation);

    /**
     * @brief Create animation for recoloring
     *
     * @param value Node value
     * @param fromColor Original color
     * @param toColor Target color
     */
    void animateRecolor(int value, RBColor fromColor, RBColor toColor);

    /**
     * @brief Update case explanation text
     */
    void updateCaseExplanation(const FixupCaseInfo& info);

    // Data
    RedBlackTree<int> m_rbTree;                       ///< Underlying RB tree data structure
    std::vector<VisualRBTreeNode> m_visualNodes;      ///< Visual representation of nodes
    std::map<int, glm::vec2> m_nodePositions;         ///< Position map for nodes
    AnimationController m_animator;                   ///< Animation controller

    // UI state
    std::string m_statusText;                         ///< Current status message
    int m_inputValue = 0;                             ///< Value for insert/search
    bool m_isPaused = true;                           ///< Pause state
    float m_speed = 1.0f;                             ///< Animation speed multiplier
    int m_initCount = 10;                             ///< Number of nodes for random initialization
    bool m_showNIL = true;                            ///< Show NIL leaf nodes
    bool m_showCaseExplanation = true;                ///< Show case explanation panel

    // Case explanation
    FixupCaseInfo m_currentCase;                      ///< Current fixup case being shown

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
        Initialize
    };
    OperationMode m_currentMode = OperationMode::Insert;

    // Visual constants
    static constexpr float NODE_RADIUS = 25.0f;
    static constexpr float VERTICAL_SPACING = 80.0f;
    static constexpr float HORIZONTAL_SPACING = 60.0f;
    static constexpr float START_X = 400.0f;
    static constexpr float START_Y = 80.0f;
    static constexpr float NIL_NODE_RADIUS = 12.0f;  // Smaller NIL nodes
};

} // namespace dsav
