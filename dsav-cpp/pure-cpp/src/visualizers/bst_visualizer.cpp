/**
 * @file bst_visualizer.cpp
 * @brief Implementation of BST visualizer
 */

#include "visualizers/bst_visualizer.hpp"
#include "ui_components.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <queue>
#include <random>
#include <algorithm>

namespace dsav {

BSTVisualizer::BSTVisualizer()
    : m_statusText("Binary Search Tree is empty") {
    // Initialize with a balanced tree for demonstration
    m_bst.insert(50);
    m_bst.insert(30);
    m_bst.insert(70);
    m_bst.insert(20);
    m_bst.insert(40);
    m_bst.insert(60);
    m_bst.insert(80);
    syncVisuals();
}

void BSTVisualizer::update(float deltaTime) {
    // Update animations
    m_animator.update(deltaTime);

    // Update status if not animating
    if (!isAnimating()) {
        if (m_bst.isEmpty()) {
            m_statusText = "Binary Search Tree is empty";
        } else {
            std::ostringstream oss;
            oss << "Tree has " << m_bst.size() << " node(s), Height: " << m_bst.height();
            m_statusText = oss.str();
        }
    }
}

void BSTVisualizer::renderVisualization() {
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
    auto root = m_bst.root();
    if (root) {
        std::function<void(std::shared_ptr<TreeNode<int>>)> drawConnections;
        drawConnections = [&](std::shared_ptr<TreeNode<int>> node) {
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
    float scaledRadius = NODE_RADIUS * m_zoomLevel;
    for (const auto& vnode : m_visualNodes) {
        // Apply zoom and camera offset to node position
        float scaledX = vnode.position.x * m_zoomLevel;
        float scaledY = vnode.position.y * m_zoomLevel;
        ImVec2 center = ImVec2(
            canvasPos.x + scaledX + horizontalOffset,
            canvasPos.y + scaledY + verticalOffset
        );

        // Draw circle
        drawList->AddCircleFilled(
            center,
            scaledRadius,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(vnode.color))
        );

        // Draw border
        drawList->AddCircle(
            center,
            scaledRadius,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(vnode.borderColor)),
            0,
            2.0f
        );

        // Draw label (centered)
        ImVec2 textSize = ImGui::CalcTextSize(vnode.label.c_str());
        ImVec2 textPos = ImVec2(
            center.x - textSize.x / 2.0f,
            center.y - textSize.y / 2.0f
        );
        drawList->AddText(
            textPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::text)),
            vnode.label.c_str()
        );
    }

    // Draw info text if empty
    if (m_bst.isEmpty()) {
        ImVec2 textPos = ImVec2(
            canvasPos.x + canvasSize.x / 2.0f - 120.0f,
            canvasPos.y + canvasSize.y / 2.0f
        );
        drawList->AddText(
            textPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
            "BST is empty. Use Insert to add nodes."
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

void BSTVisualizer::renderControls() {
    ImGui::Begin("BST Controls");

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
        "Traverse: Preorder",
        "Traverse: Postorder",
        "Traverse: Level-order",
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

    // Value input (for Insert, Delete, Search)
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
            tooltipText = "Insert value into BST";
            break;
        case OperationMode::Delete:
            buttonLabel = "Delete";
            tooltipText = "Delete value from BST";
            canExecute = !m_bst.isEmpty();
            break;
        case OperationMode::Search:
            buttonLabel = "Search";
            tooltipText = "Search for value in BST";
            canExecute = !m_bst.isEmpty();
            break;
        case OperationMode::TraverseInorder:
            buttonLabel = "Traverse Inorder";
            tooltipText = "Inorder traversal (Left-Root-Right)";
            canExecute = !m_bst.isEmpty();
            break;
        case OperationMode::TraversePreorder:
            buttonLabel = "Traverse Preorder";
            tooltipText = "Preorder traversal (Root-Left-Right)";
            canExecute = !m_bst.isEmpty();
            break;
        case OperationMode::TraversePostorder:
            buttonLabel = "Traverse Postorder";
            tooltipText = "Postorder traversal (Left-Right-Root)";
            canExecute = !m_bst.isEmpty();
            break;
        case OperationMode::TraverseLevelOrder:
            buttonLabel = "Traverse Level-order";
            tooltipText = "Level-order traversal (Breadth-first)";
            canExecute = !m_bst.isEmpty();
            break;
        case OperationMode::Initialize:
            buttonLabel = "Initialize Random";
            tooltipText = "Create BST with random values";
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
            case OperationMode::TraversePreorder:
                traversePreorder();
                break;
            case OperationMode::TraversePostorder:
                traversePostorder();
                break;
            case OperationMode::TraverseLevelOrder:
                traverseLevelOrder();
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

    // Tree info
    ImGui::Text("Tree Info:");
    ImGui::Text("Nodes: %zu", m_bst.size());
    if (!m_bst.isEmpty()) {
        ImGui::Text("Height: %d", m_bst.height());
    }

    ImGui::End();
}

void BSTVisualizer::insertValue(int value) {
    // Update status
    std::ostringstream oss;
    oss << "Inserting " << value << "...";
    m_statusText = oss.str();

    // Insert into actual BST
    m_bst.insert(value);

    // Recreate visuals
    syncVisuals();

    // Animate: find and highlight the new node
    for (auto& vnode : m_visualNodes) {
        if (vnode.value == value) {
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
                oss << "Inserted " << value;
                m_statusText = oss.str();
            };
            m_animator.enqueue(flashBack);
            break;
        }
    }
}

void BSTVisualizer::deleteValue(int value) {
    if (m_bst.isEmpty()) {
        m_statusText = "Error: Tree is empty!";
        return;
    }

    if (!m_bst.search(value)) {
        std::ostringstream oss;
        oss << "Value " << value << " not found in tree";
        m_statusText = oss.str();
        return;
    }

    // Update status
    std::ostringstream oss;
    oss << "Deleting " << value << "...";
    m_statusText = oss.str();

    // Find and animate the node before deleting
    for (auto& vnode : m_visualNodes) {
        if (vnode.value == value) {
            Animation flashRed = createColorAnimation(vnode.color, colors::semantic::error, 0.3f);
            flashRed.onComplete = [this, value]() {
                // Actually delete from tree
                m_bst.remove(value);
                syncVisuals();

                std::ostringstream oss;
                oss << "Deleted " << value;
                m_statusText = oss.str();
            };
            m_animator.enqueue(flashRed);
            break;
        }
    }
}

void BSTVisualizer::searchValue(int value) {
    if (m_bst.isEmpty()) {
        m_statusText = "Error: Tree is empty!";
        return;
    }

    // Update status
    std::ostringstream oss;
    oss << "Searching for " << value << "...";
    m_statusText = oss.str();

    // Traverse from root following BST property
    auto current = m_bst.root();
    bool found = false;

    while (current) {
        // Find visual node
        for (auto& vnode : m_visualNodes) {
            if (vnode.value == current->data) {
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
        // Not found - add completion callback to last animation
        if (m_animator.hasAnimations()) {
            // The last restore animation should update status
            std::ostringstream oss;
            oss << "Value " << value << " not found in tree";
            m_statusText = oss.str();
        }
    }
}

void BSTVisualizer::traverseInorder() {
    auto order = collectTraversalOrder("inorder");

    m_statusText = "Inorder traversal: Left-Root-Right";

    for (size_t i = 0; i < order.size(); ++i) {
        int value = order[i];
        for (auto& vnode : m_visualNodes) {
            if (vnode.value == value) {
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

void BSTVisualizer::traversePreorder() {
    auto order = collectTraversalOrder("preorder");

    m_statusText = "Preorder traversal: Root-Left-Right";

    for (size_t i = 0; i < order.size(); ++i) {
        int value = order[i];
        for (auto& vnode : m_visualNodes) {
            if (vnode.value == value) {
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
                        m_statusText = "Preorder traversal complete";
                    };
                }

                m_animator.enqueue(restore);
                break;
            }
        }
    }
}

void BSTVisualizer::traversePostorder() {
    auto order = collectTraversalOrder("postorder");

    m_statusText = "Postorder traversal: Left-Right-Root";

    for (size_t i = 0; i < order.size(); ++i) {
        int value = order[i];
        for (auto& vnode : m_visualNodes) {
            if (vnode.value == value) {
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
                        m_statusText = "Postorder traversal complete";
                    };
                }

                m_animator.enqueue(restore);
                break;
            }
        }
    }
}

void BSTVisualizer::traverseLevelOrder() {
    auto order = m_bst.levelOrderTraversal();

    m_statusText = "Level-order traversal: Breadth-first";

    for (size_t i = 0; i < order.size(); ++i) {
        int value = order[i];
        for (auto& vnode : m_visualNodes) {
            if (vnode.value == value) {
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
                        m_statusText = "Level-order traversal complete";
                    };
                }

                m_animator.enqueue(restore);
                break;
            }
        }
    }
}

void BSTVisualizer::play() {
    m_isPaused = false;
    m_animator.setPaused(false);
}

void BSTVisualizer::pause() {
    m_isPaused = true;
    m_animator.setPaused(true);
}

void BSTVisualizer::step() {
    // TODO: Implement step functionality
    m_statusText = "Step forward (not yet implemented)";
}

void BSTVisualizer::reset() {
    m_bst.clear();
    m_visualNodes.clear();
    m_nodePositions.clear();
    m_animator.clear();

    // Add initial balanced tree
    m_bst.insert(50);
    m_bst.insert(30);
    m_bst.insert(70);
    m_bst.insert(20);
    m_bst.insert(40);
    m_bst.insert(60);
    m_bst.insert(80);
    syncVisuals();

    m_statusText = "Tree reset";
    m_isPaused = true;
}

void BSTVisualizer::setSpeed(float speed) {
    m_speed = speed;
    m_animator.setSpeedMultiplier(speed);
}

std::string BSTVisualizer::getStatusText() const {
    return m_statusText;
}

bool BSTVisualizer::isAnimating() const {
    return m_animator.hasAnimations();
}

bool BSTVisualizer::isPaused() const {
    return m_isPaused;
}

void BSTVisualizer::syncVisuals() {
    m_visualNodes.clear();
    m_nodePositions.clear();

    // Calculate positions using recursive layout algorithm
    calculatePositions(m_bst.root(), START_X, START_Y, 200.0f);

    // Create visual nodes from positions
    for (const auto& [value, pos] : m_nodePositions) {
        VisualTreeNode vnode;
        vnode.position = pos;
        vnode.size = glm::vec2(NODE_RADIUS * 2, NODE_RADIUS * 2);
        vnode.color = colors::semantic::elementBase;
        vnode.borderColor = colors::semantic::elementBorder;
        vnode.label = std::to_string(value);
        vnode.value = value;

        m_visualNodes.push_back(vnode);
    }
}

void BSTVisualizer::calculatePositions(std::shared_ptr<TreeNode<int>> node, float x, float y, float xOffset) {
    if (!node) return;

    // Store position for this node
    m_nodePositions[node->data] = glm::vec2(x, y);

    // Calculate positions for children
    float nextY = y + VERTICAL_SPACING;
    float nextOffset = xOffset * 0.6f;  // Reduce offset for deeper levels

    if (node->left) {
        calculatePositions(node->left, x - xOffset, nextY, nextOffset);
    }

    if (node->right) {
        calculatePositions(node->right, x + xOffset, nextY, nextOffset);
    }
}

void BSTVisualizer::drawConnection(ImDrawList* drawList, const ImVec2& from, const ImVec2& to, ImU32 color) {
    // Draw line from parent to child
    drawList->AddLine(from, to, color, 2.0f);
}

std::vector<int> BSTVisualizer::collectTraversalOrder(const std::string& type) {
    std::vector<int> result;

    if (type == "inorder") {
        m_bst.inorderTraversal([&result](const int& val) {
            result.push_back(val);
        });
    } else if (type == "preorder") {
        m_bst.preorderTraversal([&result](const int& val) {
            result.push_back(val);
        });
    } else if (type == "postorder") {
        m_bst.postorderTraversal([&result](const int& val) {
            result.push_back(val);
        });
    }

    return result;
}

void BSTVisualizer::initializeRandom(size_t count) {
    // Clear existing tree and animations
    m_bst.clear();
    m_visualNodes.clear();
    m_nodePositions.clear();
    m_animator.clear();

    // Reset camera
    m_cameraOffsetX = 0.0f;
    m_cameraOffsetY = 0.0f;
    m_zoomLevel = 1.0f;

    // Generate unique random values in a VECTOR (not set, to avoid sorting)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 99);
    std::vector<int> values;

    // Keep generating until we have enough unique values
    while (values.size() < count) {
        int val = dis(gen);
        // Check if value is already in vector
        if (std::find(values.begin(), values.end(), val) == values.end()) {
            values.push_back(val);
        }
    }

    // Shuffle the values to ensure random insertion order
    std::shuffle(values.begin(), values.end(), gen);

    // Insert values into BST in shuffled order
    for (int value : values) {
        m_bst.insert(value);
    }

    // Sync visuals
    syncVisuals();

    // Update status immediately (no animation blocking)
    std::ostringstream oss;
    oss << "Initialized BST with " << count << " nodes, Height: " << m_bst.height();
    m_statusText = oss.str();
}

} // namespace dsav
