/**
 * @file rbtree_visualizer.cpp
 * @brief Implementation of RB tree visualizer
 */

#include "visualizers/rbtree_visualizer.hpp"
#include "ui_components.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <random>
#include <algorithm>

namespace dsav {

RBTreeVisualizer::RBTreeVisualizer()
    : m_statusText("Red-Black Tree is empty") {
    // Initialize with a balanced tree for demonstration
    m_rbTree.insert(50);
    m_rbTree.insert(30);
    m_rbTree.insert(70);
    m_rbTree.insert(20);
    m_rbTree.insert(40);
    m_rbTree.insert(60);
    m_rbTree.insert(80);
    syncVisuals();

    m_currentCase.caseName = "Ready";
    m_currentCase.explanation = "Insert values to see RB tree balancing in action";
    m_currentCase.nodeRoles = "";
}

void RBTreeVisualizer::update(float deltaTime) {
    // Update animations
    m_animator.update(deltaTime);

    // Update status if not animating
    if (!isAnimating()) {
        if (m_rbTree.isEmpty()) {
            m_statusText = "Red-Black Tree is empty";
        } else {
            std::ostringstream oss;
            oss << "Tree has " << m_rbTree.size() << " node(s), Height: " << m_rbTree.height()
                << ", Black Height: " << m_rbTree.blackHeight();
            m_statusText = oss.str();
        }
    }
}

void RBTreeVisualizer::renderVisualization() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    // Draw background
    drawList->AddRectFilled(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::mantle))
    );

    // Calculate smaller hitbox (with padding on all sides)
    const float padding = 20.0f;
    ImVec2 hitboxMin = ImVec2(canvasPos.x + padding, canvasPos.y + padding);
    ImVec2 hitboxMax = ImVec2(canvasPos.x + canvasSize.x - padding, canvasPos.y + canvasSize.y - padding - 40.0f);

    // Camera controls - Mouse interaction within smaller hitbox
    ImVec2 mousePos = ImGui::GetMousePos();
    bool isInHitbox = (mousePos.x >= hitboxMin.x && mousePos.x <= hitboxMax.x &&
                       mousePos.y >= hitboxMin.y && mousePos.y <= hitboxMax.y);

    if (isInHitbox) {
        // Panning - Drag with mouse
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 1.0f)) {
            if (!m_isDragging) {
                m_isDragging = true;
                m_lastMousePos = mousePos;
            }
            ImVec2 delta = ImVec2(mousePos.x - m_lastMousePos.x, mousePos.y - m_lastMousePos.y);
            m_cameraOffsetX += delta.x;
            m_cameraOffsetY += delta.y;
            m_lastMousePos = mousePos;

            // Prevent window from being dragged when we're panning the canvas
            ImGui::SetWindowFocus();
        } else {
            m_isDragging = false;
        }

        // Zooming - Ctrl + Scroll
        ImGuiIO& io = ImGui::GetIO();
        if (io.KeyCtrl && io.MouseWheel != 0.0f) {
            float zoomDelta = io.MouseWheel * 0.1f;
            float oldZoom = m_zoomLevel;
            m_zoomLevel += zoomDelta;
            m_zoomLevel = std::clamp(m_zoomLevel, 0.3f, 3.0f);

            // Zoom focus on mouse cursor position
            float ratio = (m_zoomLevel - oldZoom) / oldZoom;
            float relativeX = mousePos.x - canvasPos.x - m_cameraOffsetX;
            float relativeY = mousePos.y - canvasPos.y - m_cameraOffsetY;
            m_cameraOffsetX -= relativeX * ratio;
            m_cameraOffsetY -= relativeY * ratio;
        }
        // Scrolling - Mouse wheel (vertical)
        else if (io.MouseWheel != 0.0f) {
            m_cameraOffsetY += io.MouseWheel * 30.0f;
        }
        // Horizontal scrolling - Shift + Mouse wheel
        else if (io.KeyShift && io.MouseWheelH != 0.0f) {
            m_cameraOffsetX += io.MouseWheelH * 30.0f;
        }
    }

    // Apply camera offset
    float horizontalOffset = m_cameraOffsetX;
    float verticalOffset = m_cameraOffsetY;

    // Draw connections first (so they appear behind nodes)
    auto root = m_rbTree.root();
    if (root) {
        std::function<void(std::shared_ptr<RBTreeNode<int>>)> drawConnections;
        drawConnections = [&](std::shared_ptr<RBTreeNode<int>> node) {
            if (!node) return;

            auto parentIt = m_nodePositions.find(node->data);
            if (parentIt == m_nodePositions.end()) return;

            // Apply zoom and camera offset to parent position
            float scaledParentX = parentIt->second.x * m_zoomLevel;
            float scaledParentY = parentIt->second.y * m_zoomLevel;
            ImVec2 parentPos = ImVec2(
                canvasPos.x + scaledParentX + horizontalOffset,
                canvasPos.y + scaledParentY + verticalOffset
            );

            // Draw connection to left child
            if (node->left) {
                auto leftIt = m_nodePositions.find(node->left->data);
                if (leftIt != m_nodePositions.end()) {
                    float scaledLeftX = leftIt->second.x * m_zoomLevel;
                    float scaledLeftY = leftIt->second.y * m_zoomLevel;
                    ImVec2 leftPos = ImVec2(
                        canvasPos.x + scaledLeftX + horizontalOffset,
                        canvasPos.y + scaledLeftY + verticalOffset
                    );
                    drawConnection(drawList, parentPos, leftPos,
                                 ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay0)));
                }
            }

            // Draw connection to right child
            if (node->right) {
                auto rightIt = m_nodePositions.find(node->right->data);
                if (rightIt != m_nodePositions.end()) {
                    float scaledRightX = rightIt->second.x * m_zoomLevel;
                    float scaledRightY = rightIt->second.y * m_zoomLevel;
                    ImVec2 rightPos = ImVec2(
                        canvasPos.x + scaledRightX + horizontalOffset,
                        canvasPos.y + scaledRightY + verticalOffset
                    );
                    drawConnection(drawList, parentPos, rightPos,
                                 ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay0)));
                }
            }

            drawConnections(node->left);
            drawConnections(node->right);
        };

        drawConnections(root);
    }

    // Draw nodes
    for (const auto& vnode : m_visualNodes) {
        // Apply zoom and camera offset to node position
        float scaledX = vnode.position.x * m_zoomLevel;
        float scaledY = vnode.position.y * m_zoomLevel;
        ImVec2 center = ImVec2(
            canvasPos.x + scaledX + horizontalOffset,
            canvasPos.y + scaledY + verticalOffset
        );

        float radius = vnode.isNIL ? NIL_NODE_RADIUS * m_zoomLevel : NODE_RADIUS * m_zoomLevel;

        // Draw circle
        drawList->AddCircleFilled(
            center,
            radius,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(vnode.color))
        );

        // Draw border (RED or BLACK, thicker to make it visible)
        float borderThickness = vnode.isNIL ? 1.5f : 3.0f;
        drawList->AddCircle(
            center,
            radius,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(vnode.borderColor)),
            0,
            borderThickness
        );

        // Draw label (centered)
        if (!vnode.label.empty()) {
            ImVec2 textSize = ImGui::CalcTextSize(vnode.label.c_str());
            ImVec2 textPos = ImVec2(
                center.x - textSize.x / 2.0f,
                center.y - textSize.y / 2.0f
            );
            ImU32 textColor = vnode.isNIL
                ? ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::subtext0))
                : ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::text));
            drawList->AddText(
                textPos,
                textColor,
                vnode.label.c_str()
            );
        }
    }

    // Draw info text if empty
    if (m_rbTree.isEmpty()) {
        ImVec2 textPos = ImVec2(
            canvasPos.x + canvasSize.x / 2.0f - 150.0f,
            canvasPos.y + canvasSize.y / 2.0f
        );
        drawList->AddText(
            textPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
            "RB Tree is empty. Use Insert to add nodes."
        );
    }

    // Draw hint text at bottom showing controls and zoom level
    ImVec2 hintPos = ImVec2(canvasPos.x + 10.0f, canvasPos.y + canvasSize.y - 30.0f);
    std::ostringstream hintStream;
    hintStream << "Drag: Pan | Scroll: Move | Ctrl+Scroll: Zoom | Zoom: "
               << std::fixed << std::setprecision(1) << (m_zoomLevel * 100.0f) << "%";
    drawList->AddText(
        hintPos,
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
        hintStream.str().c_str()
    );
}

void RBTreeVisualizer::renderControls() {
    ImGui::Begin("RB Tree Controls");

    // Status
    ui::StatusText(m_statusText.c_str(), "info");
    ImGui::Separator();

    // Operation mode selection
    ImGui::Text("Operation Mode:");
    const char* modes[] = {
        "Insert",
        "Delete",
        "Search",
        "Traverse: Inorder",
        "Initialize Random"
    };
    int currentModeIdx = static_cast<int>(m_currentMode);
    if (ImGui::Combo("##Mode", &currentModeIdx, modes, IM_ARRAYSIZE(modes))) {
        m_currentMode = static_cast<OperationMode>(currentModeIdx);
    }

    ImGui::Separator();

    // Input fields
    ImGui::Text("Parameters:");
    ImGui::PushItemWidth(150.0f);

    // Value input (for Insert, Search)
    if (m_currentMode == OperationMode::Insert ||
        m_currentMode == OperationMode::Delete ||
        m_currentMode == OperationMode::Search) {
        ImGui::InputInt("Value", &m_inputValue);
    }
    // Count input (for Initialize)
    else if (m_currentMode == OperationMode::Initialize) {
        ImGui::InputInt("Count (1-20)", &m_initCount);
        m_initCount = std::clamp(m_initCount, 1, 20);
    }

    ImGui::PopItemWidth();

    ImGui::Spacing();

    // Execute button
    ImGui::BeginDisabled(isAnimating());

    bool canExecute = true;
    std::string buttonLabel = "Execute";
    std::string tooltipText = "";

    switch (m_currentMode) {
        case OperationMode::Insert:
            buttonLabel = "Insert";
            tooltipText = "Insert value into RB tree";
            break;
        case OperationMode::Delete:
            buttonLabel = "Delete";
            tooltipText = "Delete value from RB tree";
            canExecute = !m_rbTree.isEmpty();
            break;
        case OperationMode::Search:
            buttonLabel = "Search";
            tooltipText = "Search for value in RB tree";
            canExecute = !m_rbTree.isEmpty();
            break;
        case OperationMode::TraverseInorder:
            buttonLabel = "Traverse Inorder";
            tooltipText = "Inorder traversal (Left-Root-Right)";
            canExecute = !m_rbTree.isEmpty();
            break;
        case OperationMode::Initialize:
            buttonLabel = "Initialize Random";
            tooltipText = "Create RB tree with random values";
            break;
    }

    if (!canExecute) {
        ImGui::BeginDisabled();
    }

    if (ui::ButtonPrimary(buttonLabel.c_str(), ImVec2(220, 0))) {
        switch (m_currentMode) {
            case OperationMode::Insert:
                insertValue(m_inputValue);
                break;
            case OperationMode::Delete:
                deleteValue(m_inputValue);
                break;
            case OperationMode::Search:
                searchValue(m_inputValue);
                break;
            case OperationMode::TraverseInorder:
                traverseInorder();
                break;
            case OperationMode::Initialize:
                initializeRandom(m_initCount);
                break;
        }
    }

    if (!canExecute) {
        ImGui::EndDisabled();
    }

    ImGui::EndDisabled();
    ui::Tooltip(tooltipText.c_str());

    ImGui::Separator();

    // Playback controls
    ui::PlaybackControls(
        m_isPaused,
        [this]() { play(); },
        [this]() { pause(); },
        [this]() { step(); },
        [this]() { reset(); }
    );

    ImGui::Spacing();
    ui::SpeedSlider(m_speed, 0.1f, 5.0f);

    ImGui::Separator();

    // Display options
    ImGui::Text("Display Options:");
    ImGui::Checkbox("Show NIL Leaves", &m_showNIL);
    ui::Tooltip("Show black NIL leaf nodes for educational purposes");

    ImGui::Checkbox("Show Case Explanation", &m_showCaseExplanation);
    ui::Tooltip("Show fixup case details during insertion");

    ImGui::Separator();

    // Tree info
    ImGui::Text("Tree Info:");
    ImGui::Text("Nodes: %zu", m_rbTree.size());
    if (!m_rbTree.isEmpty()) {
        ImGui::Text("Height: %d", m_rbTree.height());
        ImGui::Text("Black Height: %d", m_rbTree.blackHeight());

        // Verify RB properties
        bool isValid = m_rbTree.verifyProperties();
        ImGui::TextColored(
            isValid ? colors::toImGui(colors::semantic::sorted) : colors::toImGui(colors::semantic::error),
            "Properties: %s", isValid ? "VALID" : "INVALID"
        );
    }

    ImGui::End();

    // Case explanation panel (separate window)
    if (m_showCaseExplanation && !m_currentCase.caseName.empty()) {
        ImGui::Begin("Fixup Case Explanation");

        ImGui::TextColored(colors::toImGui(colors::semantic::active), "%s", m_currentCase.caseName.c_str());
        ImGui::Separator();

        ImGui::TextWrapped("%s", m_currentCase.explanation.c_str());

        if (!m_currentCase.nodeRoles.empty()) {
            ImGui::Spacing();
            ImGui::Text("Nodes: %s", m_currentCase.nodeRoles.c_str());
        }

        ImGui::End();
    }
}

void RBTreeVisualizer::insertValue(int value) {
    // Update status
    std::ostringstream oss;
    oss << "Inserting " << value << "...";
    m_statusText = oss.str();

    // Step 1: Capture positions BEFORE insertion
    std::map<int, glm::vec2> oldPositions = m_nodePositions;
    std::map<int, RBColor> oldColors;

    // Store colors of existing nodes
    auto root = m_rbTree.root();
    if (root) {
        std::function<void(std::shared_ptr<RBTreeNode<int>>)> captureColors;
        captureColors = [&](std::shared_ptr<RBTreeNode<int>> node) {
            if (!node) return;
            oldColors[node->data] = node->color;
            captureColors(node->left);
            captureColors(node->right);
        };
        captureColors(root);
    }

    // Step 2: Perform insertion
    m_rbTree.insert(value);

    // Step 3: Recalculate positions AFTER insertion
    syncVisuals();
    std::map<int, glm::vec2> newPositions = m_nodePositions;

    // Step 4: Detect moved nodes and create smooth animations
    std::vector<Animation> parallelMoves;

    for (auto& vnode : m_visualNodes) {
        if (vnode.isNIL) continue;  // Skip NIL nodes

        auto oldPosIt = oldPositions.find(vnode.value);
        if (oldPosIt != oldPositions.end()) {
            // Node existed before - check if it moved
            if (glm::distance(oldPosIt->second, vnode.position) > 1.0f) {
                // Node moved! Create smooth animation
                vnode.position = oldPosIt->second;  // Start from old position

                Animation moveAnim = createMoveAnimation(
                    vnode.position,
                    newPositions[vnode.value],
                    0.5f  // 0.5 seconds for rotation animation
                );
                moveAnim.easingFn = easing::easeOutBack;  // Smooth overshoot
                parallelMoves.push_back(moveAnim);
            }

            // Check if color changed
            auto oldColorIt = oldColors.find(vnode.value);
            if (oldColorIt != oldColors.end() && oldColorIt->second != vnode.rbColor) {
                // Color changed! Animate border color
                glm::vec4 oldBorderColor = (oldColorIt->second == RBColor::RED)
                    ? colors::semantic::error : colors::mocha::text;

                Animation colorAnim = createColorAnimation(
                    vnode.borderColor,
                    getBorderColor(vnode.rbColor),
                    0.4f
                );
                parallelMoves.push_back(colorAnim);
            }
        }
    }

    // Step 5: Enqueue parallel animations for rotations
    if (!parallelMoves.empty()) {
        m_animator.enqueueParallel(parallelMoves);

        // Update explanation during rotation
        m_currentCase.caseName = "Balancing Tree";
        m_currentCase.explanation = "Performing rotations and recoloring to maintain RB properties...";
        m_currentCase.nodeRoles = "Animated: " + std::to_string(parallelMoves.size()) + " operations";
    }

    // Step 6: Highlight the newly inserted node
    for (auto& vnode : m_visualNodes) {
        if (vnode.value == value && !vnode.isNIL) {
            Animation flashGreen = createColorAnimation(
                vnode.color,
                colors::semantic::sorted,
                0.3f
            );
            m_animator.enqueue(flashGreen);

            Animation flashBack = createColorAnimation(
                vnode.color,
                colors::semantic::elementBase,
                0.3f
            );
            flashBack.onComplete = [this, value]() {
                std::ostringstream oss;
                oss << "Inserted " << value << " - Tree balanced";
                m_statusText = oss.str();

                // Update explanation
                m_currentCase.caseName = "Insertion Complete";
                m_currentCase.explanation = "RB tree properties maintained. All rotations and recoloring complete.";
                m_currentCase.nodeRoles = "";
            };
            m_animator.enqueue(flashBack);
            break;
        }
    }
}


void RBTreeVisualizer::deleteValue(int value) {
    // Check if tree is empty
    if (m_rbTree.isEmpty()) {
        m_statusText = "Error: Tree is empty!";
        return;
    }

    // Check if value exists
    auto nodeToDelete = m_rbTree.search(value);
    if (!nodeToDelete) {
        std::ostringstream oss;
        oss << "Error: Value " << value << " not found in tree";
        m_statusText = oss.str();
        return;
    }

    // Update status
    std::ostringstream oss;
    oss << "Deleting " << value << "...";
    m_statusText = oss.str();

    // Step 1: Flash red on node being deleted
    for (auto& vnode : m_visualNodes) {
        if (vnode.value == value && !vnode.isNIL) {
            Animation flashRed = createColorAnimation(
                vnode.color,
                colors::semantic::error,
                0.3f
            );
            m_animator.enqueue(flashRed);
            break;
        }
    }

    // Step 2: Capture positions and colors BEFORE deletion
    std::map<int, glm::vec2> oldPositions = m_nodePositions;
    std::map<int, RBColor> oldColors;

    auto root = m_rbTree.root();
    if (root) {
        std::function<void(std::shared_ptr<RBTreeNode<int>>)> captureColors;
        captureColors = [&](std::shared_ptr<RBTreeNode<int>> node) {
            if (!node) return;
            oldColors[node->data] = node->color;
            captureColors(node->left);
            captureColors(node->right);
        };
        captureColors(root);
    }

    // Step 3: Perform deletion
    bool success = m_rbTree.remove(value);

    if (!success) {
        m_statusText = "Error: Deletion failed";
        return;
    }

    // Step 4: Recalculate positions AFTER deletion
    syncVisuals();
    std::map<int, glm::vec2> newPositions = m_nodePositions;

    // Step 5: Detect moved nodes and create smooth animations
    std::vector<Animation> parallelMoves;

    for (auto& vnode : m_visualNodes) {
        if (vnode.isNIL) continue;

        auto oldPosIt = oldPositions.find(vnode.value);
        if (oldPosIt != oldPositions.end()) {
            // Node existed before - check if it moved
            if (glm::distance(oldPosIt->second, vnode.position) > 1.0f) {
                // Node moved! Create smooth animation
                vnode.position = oldPosIt->second;  // Start from old position

                Animation moveAnim = createMoveAnimation(
                    vnode.position,
                    newPositions[vnode.value],
                    0.5f
                );
                moveAnim.easingFn = easing::easeOutBack;
                parallelMoves.push_back(moveAnim);
            }

            // Check color changes and animate borders
            auto oldColorIt = oldColors.find(vnode.value);
            if (oldColorIt != oldColors.end() && oldColorIt->second != vnode.rbColor) {
                Animation colorAnim = createColorAnimation(
                    vnode.borderColor,
                    getBorderColor(vnode.rbColor),
                    0.4f
                );
                parallelMoves.push_back(colorAnim);
            }
        }
    }

    // Step 6: Enqueue parallel animations
    if (!parallelMoves.empty()) {
        m_animator.enqueueParallel(parallelMoves);
        m_currentCase.caseName = "Rebalancing After Deletion";
        m_currentCase.explanation = "Performing rotations and recoloring to maintain RB tree properties...";
    }

    // Step 7: Final status update
    Animation complete;
    complete.duration = 0.1f;
    complete.updateFn = [](float) {};
    complete.onComplete = [this, value]() {
        std::ostringstream oss;
        oss << "Deleted " << value << " - Tree balanced";
        m_statusText = oss.str();

        m_currentCase.caseName = "Deletion Complete";
        m_currentCase.explanation = "RB tree properties maintained. All fixup operations complete.";
        m_currentCase.nodeRoles = "";
    };
    m_animator.enqueue(complete);
}
void RBTreeVisualizer::searchValue(int value) {
    if (m_rbTree.isEmpty()) {
        m_statusText = "Error: Tree is empty!";
        return;
    }

    // Update status
    std::ostringstream oss;
    oss << "Searching for " << value << "...";
    m_statusText = oss.str();

    // Traverse from root following BST property
    auto current = m_rbTree.root();
    bool found = false;

    while (current) {
        // Find visual node
        for (auto& vnode : m_visualNodes) {
            if (vnode.value == current->data && !vnode.isNIL) {
                if (current->data == value) {
                    // Found!
                    Animation highlightFound = createColorAnimation(
                        vnode.color,
                        colors::semantic::sorted,
                        0.3f
                    );
                    highlightFound.onComplete = [this, value]() {
                        std::ostringstream oss;
                        oss << "Found " << value << " in tree";
                        m_statusText = oss.str();
                    };
                    m_animator.enqueue(highlightFound);

                    Animation restore = createColorAnimation(
                        vnode.color,
                        colors::semantic::elementBase,
                        0.3f
                    );
                    m_animator.enqueue(restore);
                    found = true;
                } else {
                    // Checking this node
                    Animation checking = createColorAnimation(
                        vnode.color,
                        colors::semantic::comparing,
                        0.2f
                    );
                    m_animator.enqueue(checking);

                    Animation restore = createColorAnimation(
                        vnode.color,
                        colors::semantic::elementBase,
                        0.2f
                    );
                    m_animator.enqueue(restore);
                }
                break;
            }
        }

        if (found) break;

        // Move to next node
        if (value < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (!found) {
        std::ostringstream oss;
        oss << "Value " << value << " not found in tree";
        m_statusText = oss.str();
    }
}

void RBTreeVisualizer::traverseInorder() {
    auto order = collectInorderTraversal();

    m_statusText = "Inorder traversal: Left-Root-Right";

    for (size_t i = 0; i < order.size(); ++i) {
        int value = order[i];
        for (auto& vnode : m_visualNodes) {
            if (vnode.value == value && !vnode.isNIL) {
                Animation highlight = createColorAnimation(
                    vnode.color,
                    colors::semantic::highlight,
                    0.3f
                );
                m_animator.enqueue(highlight);

                Animation restore = createColorAnimation(
                    vnode.color,
                    colors::semantic::elementBase,
                    0.3f
                );

                if (i == order.size() - 1) {
                    restore.onComplete = [this]() {
                        m_statusText = "Inorder traversal complete";
                    };
                }

                m_animator.enqueue(restore);
                break;
            }
        }
    }
}

void RBTreeVisualizer::initializeRandom(size_t count) {
    // Clear existing tree and animations
    m_rbTree.clear();
    m_visualNodes.clear();
    m_nodePositions.clear();
    m_animator.clear();

    // Reset camera
    m_cameraOffsetX = 0.0f;
    m_cameraOffsetY = 0.0f;
    m_zoomLevel = 1.0f;

    // Generate unique random values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 99);
    std::vector<int> values;

    while (values.size() < count) {
        int val = dis(gen);
        if (std::find(values.begin(), values.end(), val) == values.end()) {
            values.push_back(val);
        }
    }

    // Shuffle the values
    std::shuffle(values.begin(), values.end(), gen);

    // Insert values into RB tree
    for (int value : values) {
        m_rbTree.insert(value);
    }

    // Sync visuals
    syncVisuals();

    // Update status
    std::ostringstream oss;
    oss << "Initialized RB tree with " << count << " nodes, Height: " << m_rbTree.height()
        << ", Black Height: " << m_rbTree.blackHeight();
    m_statusText = oss.str();

    // Reset case explanation
    m_currentCase.caseName = "Ready";
    m_currentCase.explanation = "Tree initialized with random values";
    m_currentCase.nodeRoles = "";
}

void RBTreeVisualizer::play() {
    m_isPaused = false;
    m_animator.setPaused(false);
}

void RBTreeVisualizer::pause() {
    m_isPaused = true;
    m_animator.setPaused(true);
}

void RBTreeVisualizer::step() {
    // Step forward by 0.1 seconds if there are animations
    if (m_animator.hasAnimations()) {
        m_animator.stepForward(0.1f);
        m_statusText = "Stepped forward 0.1s";
    } else {
        m_statusText = "No animations to step through";
    }
}

void RBTreeVisualizer::reset() {
    m_rbTree.clear();
    m_visualNodes.clear();
    m_nodePositions.clear();
    m_animator.clear();

    // Add initial balanced tree
    m_rbTree.insert(50);
    m_rbTree.insert(30);
    m_rbTree.insert(70);
    m_rbTree.insert(20);
    m_rbTree.insert(40);
    m_rbTree.insert(60);
    m_rbTree.insert(80);
    syncVisuals();

    m_statusText = "Tree reset";
    m_isPaused = true;

    // Reset case explanation
    m_currentCase.caseName = "Ready";
    m_currentCase.explanation = "Insert values to see RB tree balancing in action";
    m_currentCase.nodeRoles = "";
}

void RBTreeVisualizer::setSpeed(float speed) {
    m_speed = speed;
    m_animator.setSpeedMultiplier(speed);
}

std::string RBTreeVisualizer::getStatusText() const {
    return m_statusText;
}

bool RBTreeVisualizer::isAnimating() const {
    return m_animator.hasAnimations();
}

bool RBTreeVisualizer::isPaused() const {
    return m_isPaused;
}

void RBTreeVisualizer::syncVisuals() {
    m_visualNodes.clear();
    m_nodePositions.clear();

    // Calculate positions using recursive layout algorithm
    // Increased initial offset from 200 to 280 for better spacing
    calculatePositions(m_rbTree.root(), START_X, START_Y, 280.0f);

    // Create visual nodes from positions
    for (const auto& [value, pos] : m_nodePositions) {
        auto node = m_rbTree.find(value);
        if (!node) continue;

        VisualRBTreeNode vnode;
        vnode.position = pos;
        vnode.size = glm::vec2(NODE_RADIUS * 2, NODE_RADIUS * 2);
        vnode.color = colors::semantic::elementBase;
        vnode.borderColor = getBorderColor(node->color);
        vnode.label = std::to_string(value);
        vnode.value = value;
        vnode.rbColor = node->color;
        vnode.isNIL = false;

        m_visualNodes.push_back(vnode);
    }

    // Add NIL nodes if enabled
    if (m_showNIL && m_rbTree.root()) {
        std::function<void(std::shared_ptr<RBTreeNode<int>>)> addNILs;
        addNILs = [&](std::shared_ptr<RBTreeNode<int>> node) {
            if (!node) return;

            // Add NIL children if they don't exist
            if (!node->left) {
                addNILNode(node, true);
            }
            if (!node->right) {
                addNILNode(node, false);
            }

            addNILs(node->left);
            addNILs(node->right);
        };

        addNILs(m_rbTree.root());
    }
}

void RBTreeVisualizer::calculatePositions(std::shared_ptr<RBTreeNode<int>> node, float x, float y, float xOffset) {
    if (!node) return;

    // Store position for this node
    m_nodePositions[node->data] = glm::vec2(x, y);

    // Calculate positions for children
    float nextY = y + VERTICAL_SPACING;
    float nextOffset = xOffset * 0.75f;  // Reduce offset for deeper levels (slower decay)

    // Ensure minimum spacing to prevent overlap (nodes are 50px diameter)
    const float MIN_OFFSET = 70.0f;
    if (nextOffset < MIN_OFFSET) {
        nextOffset = MIN_OFFSET;
    }

    if (node->left) {
        calculatePositions(node->left, x - xOffset, nextY, nextOffset);
    }

    if (node->right) {
        calculatePositions(node->right, x + xOffset, nextY, nextOffset);
    }
}

void RBTreeVisualizer::addNILNode(std::shared_ptr<RBTreeNode<int>> parent, bool isLeft) {
    auto parentIt = m_nodePositions.find(parent->data);
    if (parentIt == m_nodePositions.end()) return;

    // Calculate NIL position
    float parentX = parentIt->second.x;
    float parentY = parentIt->second.y;
    float xOffset = 100.0f * 0.6f * 0.6f;  // Smaller offset for NIL nodes
    float nilX = isLeft ? (parentX - xOffset) : (parentX + xOffset);
    float nilY = parentY + VERTICAL_SPACING;

    VisualRBTreeNode nilNode;
    nilNode.position = glm::vec2(nilX, nilY);
    nilNode.size = glm::vec2(NIL_NODE_RADIUS * 2, NIL_NODE_RADIUS * 2);
    nilNode.color = colors::mocha::surface0;  // Dark background
    nilNode.borderColor = getBorderColor(RBColor::BLACK);  // BLACK border
    nilNode.label = "NIL";
    nilNode.value = -1;  // Sentinel value
    nilNode.rbColor = RBColor::BLACK;
    nilNode.isNIL = true;

    m_visualNodes.push_back(nilNode);

    // Store NIL position with unique negative key
    static int nilCounter = -1000;
    m_nodePositions[nilCounter--] = nilNode.position;
}

void RBTreeVisualizer::drawConnection(ImDrawList* drawList, const ImVec2& from, const ImVec2& to, ImU32 color) {
    drawList->AddLine(from, to, color, 2.0f);
}

glm::vec4 RBTreeVisualizer::getBorderColor(RBColor color) const {
    return (color == RBColor::RED) ? colors::semantic::error : colors::mocha::text;
}

std::vector<int> RBTreeVisualizer::collectInorderTraversal() {
    std::vector<int> result;
    m_rbTree.inorderTraversal([&result](const int& val) {
        result.push_back(val);
    });
    return result;
}

void RBTreeVisualizer::updateCaseExplanation(const FixupCaseInfo& info) {
    m_currentCase = info;
}

} // namespace dsav
