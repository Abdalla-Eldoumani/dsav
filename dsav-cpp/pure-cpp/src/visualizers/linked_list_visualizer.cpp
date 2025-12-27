/**
 * @file linked_list_visualizer.cpp
 * @brief Implementation of Linked List visualizer
 */

#include "visualizers/linked_list_visualizer.hpp"
#include "ui_components.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <random>
#include <ctime>

namespace dsav {

LinkedListVisualizer::LinkedListVisualizer()
    : m_statusText("Linked list is empty") {
    // Initialize with a few nodes for demonstration
    m_list.insertBack(10);
    m_list.insertBack(20);
    m_list.insertBack(30);
    syncVisuals();
}

void LinkedListVisualizer::update(float deltaTime) {
    // Update animations
    m_animator.update(deltaTime);

    // Update status if not animating
    if (!isAnimating()) {
        if (m_list.isEmpty()) {
            m_statusText = "Linked list is empty";
        } else {
            std::ostringstream oss;
            oss << "List has " << m_list.size() << " node(s)";
            m_statusText = oss.str();
        }
    }
}

void LinkedListVisualizer::renderVisualization() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    // Draw background
    drawList->AddRectFilled(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::mantle))
    );

    // Calculate scaled dimensions with zoom
    float scaledNodeWidth = NODE_WIDTH * m_zoomLevel;
    float scaledNodeHeight = NODE_HEIGHT * m_zoomLevel;
    float scaledNodeSpacing = NODE_SPACING * m_zoomLevel;

    // Calculate total width with zoom applied
    float totalWidth = 0.0f;
    if (!m_visualNodes.empty()) {
        totalWidth = m_visualNodes.size() * scaledNodeWidth +
                     (m_visualNodes.size() - 1) * scaledNodeSpacing;
    }

    // Calculate horizontal offset for centering
    float horizontalOffset = (canvasSize.x - totalWidth) / 2.0f;
    if (horizontalOffset < 100.0f) {
        horizontalOffset = 100.0f;
    }

    // Apply camera offset
    horizontalOffset += m_cameraOffsetX;

    // Calculate interaction hitbox (smaller, only where elements are)
    float hitboxPadding = 40.0f;
    ImVec2 hitboxPos = ImVec2(
        canvasPos.x + std::max(100.0f, horizontalOffset - hitboxPadding - 80.0f),  // Include HEAD label
        canvasPos.y + START_Y * m_zoomLevel - hitboxPadding
    );
    ImVec2 hitboxSize = ImVec2(
        std::min(canvasSize.x - 120.0f, totalWidth + hitboxPadding * 2 + 80.0f),
        scaledNodeHeight + hitboxPadding * 2
    );

    // Make sure hitbox stays within canvas
    if (hitboxPos.x < canvasPos.x + 20.0f) {
        hitboxSize.x -= (canvasPos.x + 20.0f - hitboxPos.x);
        hitboxPos.x = canvasPos.x + 20.0f;
    }
    if (hitboxPos.x + hitboxSize.x > canvasPos.x + canvasSize.x - 20.0f) {
        hitboxSize.x = canvasPos.x + canvasSize.x - 20.0f - hitboxPos.x;
    }

    // Set cursor position for the hitbox
    ImGui::SetCursorScreenPos(hitboxPos);
    ImGui::InvisibleButton("linkedlist_canvas", hitboxSize);
    bool isHovered = ImGui::IsItemHovered();
    bool isActive = ImGui::IsItemActive();

    // Handle mouse drag for panning
    if (isActive && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        if (!m_isDragging) {
            m_isDragging = true;
            m_lastMousePos = ImGui::GetMousePos();
        } else {
            ImVec2 currentMousePos = ImGui::GetMousePos();
            float deltaX = currentMousePos.x - m_lastMousePos.x;
            m_cameraOffsetX += deltaX;
            m_lastMousePos = currentMousePos;
        }
    } else {
        m_isDragging = false;
    }

    // Handle mouse wheel
    if (isHovered) {
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.0f) {
            // Check if Ctrl is held for zoom
            if (ImGui::GetIO().KeyCtrl) {
                // Zoom in/out
                float oldZoom = m_zoomLevel;
                float zoomDelta = wheel * 0.1f;
                m_zoomLevel += zoomDelta;

                // Clamp zoom level
                if (m_zoomLevel < 0.3f) m_zoomLevel = 0.3f;
                if (m_zoomLevel > 3.0f) m_zoomLevel = 3.0f;

                // Adjust camera offset to zoom toward mouse position
                ImVec2 mousePos = ImGui::GetMousePos();
                float mouseRelativeX = mousePos.x - canvasPos.x - horizontalOffset;
                float zoomRatio = m_zoomLevel / oldZoom;

                // Adjust offset so zoom focuses on mouse position
                m_cameraOffsetX = m_cameraOffsetX * zoomRatio + mouseRelativeX * (1.0f - zoomRatio);
            } else {
                // Regular horizontal scrolling
                m_cameraOffsetX += wheel * 50.0f;
            }
        }
    }

    // Recalculate with potentially updated zoom
    scaledNodeWidth = NODE_WIDTH * m_zoomLevel;
    scaledNodeHeight = NODE_HEIGHT * m_zoomLevel;
    scaledNodeSpacing = NODE_SPACING * m_zoomLevel;

    totalWidth = 0.0f;
    if (!m_visualNodes.empty()) {
        totalWidth = m_visualNodes.size() * scaledNodeWidth +
                     (m_visualNodes.size() - 1) * scaledNodeSpacing;
    }

    horizontalOffset = (canvasSize.x - totalWidth) / 2.0f;
    if (horizontalOffset < 100.0f) {
        horizontalOffset = 100.0f;
    }
    horizontalOffset += m_cameraOffsetX;

    // Clamp camera offset to prevent scrolling too far
    float maxOffset = 0.0f;
    float minOffset = canvasSize.x - totalWidth - 120.0f;
    if (minOffset > maxOffset) minOffset = maxOffset;

    if (m_cameraOffsetX > (maxOffset - ((canvasSize.x - totalWidth) / 2.0f) + 100.0f)) {
        m_cameraOffsetX = maxOffset - ((canvasSize.x - totalWidth) / 2.0f) + 100.0f;
        horizontalOffset = maxOffset + 100.0f;
    }
    if (horizontalOffset < minOffset) {
        m_cameraOffsetX = minOffset - ((canvasSize.x - totalWidth) / 2.0f);
        horizontalOffset = minOffset;
    }

    // Draw "HEAD →" indicator (positioned close to first node)
    if (!m_visualNodes.empty() && !m_visualNodes[0].isNull) {
        float scaledY = START_Y * m_zoomLevel;
        float firstNodeX = START_X * m_zoomLevel;  // First node's actual X position
        ImVec2 headTextPos = ImVec2(
            canvasPos.x + horizontalOffset + firstNodeX - 60.0f,  // 60px left of first node
            canvasPos.y + scaledY + scaledNodeHeight / 2.0f - 10.0f
        );
        drawList->AddText(
            headTextPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::green)),
            "HEAD →"
        );
    }

    // Draw nodes and arrows with zoom
    for (size_t i = 0; i < m_visualNodes.size(); ++i) {
        const auto& visualNode = m_visualNodes[i];

        float scaledX = visualNode.position.x * m_zoomLevel;
        float scaledY = START_Y * m_zoomLevel;

        // Create VisualElement for rendering
        VisualElement elem;
        elem.position = glm::vec2(
            canvasPos.x + horizontalOffset + scaledX,
            canvasPos.y + scaledY
        );
        elem.size = glm::vec2(scaledNodeWidth, scaledNodeHeight);
        elem.color = visualNode.color;
        elem.borderColor = visualNode.borderColor;
        elem.borderWidth = 2.0f;
        elem.label = visualNode.label;
        elem.sublabel = "";

        renderElement(drawList, elem);

        // Draw arrow to next node
        if (visualNode.hasNext && i < m_visualNodes.size() - 1) {
            ImVec2 arrowStart = ImVec2(
                elem.position.x + scaledNodeWidth,
                elem.position.y + scaledNodeHeight / 2.0f
            );

            float nextScaledX = m_visualNodes[i + 1].position.x * m_zoomLevel;
            ImVec2 arrowEnd = ImVec2(
                canvasPos.x + horizontalOffset + nextScaledX,
                canvasPos.y + scaledY + scaledNodeHeight / 2.0f
            );

            drawArrow(drawList, arrowStart, arrowEnd,
                     ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::blue)));
        }
    }

    // Draw info text if empty (only NULL node)
    if (m_list.isEmpty()) {
        ImVec2 textPos = ImVec2(
            canvasPos.x + canvasSize.x / 2.0f - 120.0f,
            canvasPos.y + canvasSize.y / 2.0f
        );
        drawList->AddText(
            textPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
            "Linked list is empty. Use Insert to add nodes."
        );
    }

    // Show interaction hints
    if (!m_list.isEmpty()) {
        std::string hintText = "Drag to pan | Scroll to move | Ctrl+Scroll to zoom";
        if (m_zoomLevel != 1.0f) {
            hintText += " (Zoom: " + std::to_string(static_cast<int>(m_zoomLevel * 100)) + "%)";
        }
        ImVec2 hintSize = ImGui::CalcTextSize(hintText.c_str());
        ImVec2 hintPos = ImVec2(
            canvasPos.x + canvasSize.x - hintSize.x - 10.0f,
            canvasPos.y + 10.0f
        );
        drawList->AddText(
            hintPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay0)),
            hintText.c_str()
        );
    }
}

void LinkedListVisualizer::renderControls() {
    ImGui::Begin("Linked List Controls");

    // Status
    ui::StatusText(m_statusText.c_str(), "info");
    ImGui::Separator();

    // Operation mode selection
    ImGui::Text("Operation Mode:");
    const char* modes[] = {
        "Insert Front",
        "Insert Back",
        "Insert At",
        "Delete Front",
        "Delete Back",
        "Delete At",
        "Search"
    };
    int currentModeIdx = static_cast<int>(m_currentMode);
    if (ImGui::Combo("##Mode", &currentModeIdx, modes, IM_ARRAYSIZE(modes))) {
        m_currentMode = static_cast<OperationMode>(currentModeIdx);
    }

    ImGui::Separator();

    // Input fields
    ImGui::Text("Parameters:");
    ImGui::PushItemWidth(150.0f);

    // Value input (for Insert and Search)
    if (m_currentMode == OperationMode::InsertFront ||
        m_currentMode == OperationMode::InsertBack ||
        m_currentMode == OperationMode::InsertAt ||
        m_currentMode == OperationMode::Search) {
        ImGui::InputInt("Value", &m_inputValue);
    }

    // Index input (for InsertAt and DeleteAt)
    if (m_currentMode == OperationMode::InsertAt ||
        m_currentMode == OperationMode::DeleteAt) {
        ImGui::InputInt("Index", &m_inputIndex);
        if (m_inputIndex < 0) m_inputIndex = 0;
    }

    ImGui::PopItemWidth();

    ImGui::Spacing();

    // Execute button
    ImGui::BeginDisabled(isAnimating());

    bool canExecute = true;
    std::string buttonLabel = "Execute";
    std::string tooltipText = "";

    switch (m_currentMode) {
        case OperationMode::InsertFront:
            buttonLabel = "Insert Front";
            tooltipText = "Insert node at the beginning of the list";
            break;
        case OperationMode::InsertBack:
            buttonLabel = "Insert Back";
            tooltipText = "Insert node at the end of the list";
            break;
        case OperationMode::InsertAt:
            buttonLabel = "Insert At";
            tooltipText = "Insert node at specific index";
            canExecute = static_cast<size_t>(m_inputIndex) <= m_list.size();
            break;
        case OperationMode::DeleteFront:
            buttonLabel = "Delete Front";
            tooltipText = "Delete first node";
            canExecute = !m_list.isEmpty();
            break;
        case OperationMode::DeleteBack:
            buttonLabel = "Delete Back";
            tooltipText = "Delete last node";
            canExecute = !m_list.isEmpty();
            break;
        case OperationMode::DeleteAt:
            buttonLabel = "Delete At";
            tooltipText = "Delete node at specific index";
            canExecute = !m_list.isEmpty() && static_cast<size_t>(m_inputIndex) < m_list.size();
            break;
        case OperationMode::Search:
            buttonLabel = "Search";
            tooltipText = "Search for value in list";
            canExecute = !m_list.isEmpty();
            break;
    }

    if (!canExecute) {
        ImGui::BeginDisabled();
    }

    if (ui::ButtonPrimary(buttonLabel.c_str(), ImVec2(200, 0))) {
        switch (m_currentMode) {
            case OperationMode::InsertFront:
                insertFrontValue(m_inputValue);
                break;
            case OperationMode::InsertBack:
                insertBackValue(m_inputValue);
                break;
            case OperationMode::InsertAt:
                insertAtValue(static_cast<size_t>(m_inputIndex), m_inputValue);
                break;
            case OperationMode::DeleteFront:
                deleteFrontValue();
                break;
            case OperationMode::DeleteBack:
                deleteBackValue();
                break;
            case OperationMode::DeleteAt:
                deleteAtValue(static_cast<size_t>(m_inputIndex));
                break;
            case OperationMode::Search:
                searchValue(m_inputValue);
                break;
        }
    }

    if (!canExecute) {
        ImGui::EndDisabled();
    }

    ImGui::EndDisabled();
    ui::Tooltip(tooltipText.c_str());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Initialize operation
    ImGui::Text("Initialize:");
    ImGui::PushItemWidth(150.0f);
    ImGui::InputInt("Count", &m_initCount);
    if (m_initCount < 1) m_initCount = 1;
    if (m_initCount > 20) m_initCount = 20;  // Reasonable limit
    ImGui::PopItemWidth();

    ImGui::BeginDisabled(isAnimating());
    if (ui::ButtonPrimary("Initialize Random", ImVec2(200, 0))) {
        initializeRandom(static_cast<size_t>(m_initCount));
    }
    ImGui::EndDisabled();
    ui::Tooltip("Fill list with random values (clears existing list)");

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

    // List info
    ImGui::Text("List Info:");
    ImGui::Text("Size: %zu nodes", m_list.size());

    ImGui::End();
}

void LinkedListVisualizer::insertFrontValue(int value) {
    // Update status
    std::ostringstream oss;
    oss << "Inserting " << value << " at front...";
    m_statusText = oss.str();

    // Insert into actual list
    m_list.insertFront(value);

    // Recreate visuals
    syncVisuals();

    // Animate: highlight new node (first one)
    if (!m_visualNodes.empty()) {
        auto& newNode = m_visualNodes.front();

        Animation flashGreen = createColorAnimation(
            newNode.color,
            colors::semantic::sorted,
            0.3f
        );
        m_animator.enqueue(flashGreen);

        Animation flashBack = createColorAnimation(
            newNode.color,
            colors::semantic::elementBase,
            0.3f
        );
        flashBack.onComplete = [this, value]() {
            std::ostringstream oss;
            oss << "Inserted " << value << " at front";
            m_statusText = oss.str();
        };
        m_animator.enqueue(flashBack);
    }
}

void LinkedListVisualizer::insertBackValue(int value) {
    // Update status
    std::ostringstream oss;
    oss << "Inserting " << value << " at back...";
    m_statusText = oss.str();

    // Insert into actual list
    m_list.insertBack(value);

    // Recreate visuals
    syncVisuals();

    // Animate: highlight new node (last one)
    if (!m_visualNodes.empty()) {
        auto& newNode = m_visualNodes.back();

        Animation flashGreen = createColorAnimation(
            newNode.color,
            colors::semantic::sorted,
            0.3f
        );
        m_animator.enqueue(flashGreen);

        Animation flashBack = createColorAnimation(
            newNode.color,
            colors::semantic::elementBase,
            0.3f
        );
        flashBack.onComplete = [this, value]() {
            std::ostringstream oss;
            oss << "Inserted " << value << " at back";
            m_statusText = oss.str();
        };
        m_animator.enqueue(flashBack);
    }
}

void LinkedListVisualizer::insertAtValue(size_t index, int value) {
    if (index > m_list.size()) {
        m_statusText = "Error: Index out of range!";
        return;
    }

    // Update status
    std::ostringstream oss;
    oss << "Inserting " << value << " at index " << index << "...";
    m_statusText = oss.str();

    // Insert into actual list
    m_list.insertAt(index, value);

    // Recreate visuals
    syncVisuals();

    // Animate: highlight new node
    if (index < m_visualNodes.size()) {
        auto& newNode = m_visualNodes[index];

        Animation flashGreen = createColorAnimation(
            newNode.color,
            colors::semantic::sorted,
            0.3f
        );
        m_animator.enqueue(flashGreen);

        Animation flashBack = createColorAnimation(
            newNode.color,
            colors::semantic::elementBase,
            0.3f
        );
        flashBack.onComplete = [this, value, index]() {
            std::ostringstream oss;
            oss << "Inserted " << value << " at index " << index;
            m_statusText = oss.str();
        };
        m_animator.enqueue(flashBack);
    }
}

void LinkedListVisualizer::deleteFrontValue() {
    if (m_list.isEmpty()) {
        m_statusText = "Error: List is empty!";
        return;
    }

    // Get the value before deleting
    auto current = m_list.head();
    int value = current->data;

    // Update status
    std::ostringstream oss;
    oss << "Deleting front node...";
    m_statusText = oss.str();

    // Animate deletion
    if (!m_visualNodes.empty()) {
        auto& node = m_visualNodes.front();

        Animation flashRed = createColorAnimation(node.color, colors::semantic::error, 0.3f);
        flashRed.onComplete = [this, value]() {
            // Actually delete from list
            m_list.deleteFront();
            syncVisuals();

            std::ostringstream oss;
            oss << "Deleted " << value << " from front";
            m_statusText = oss.str();
        };
        m_animator.enqueue(flashRed);
    }
}

void LinkedListVisualizer::deleteBackValue() {
    if (m_list.isEmpty()) {
        m_statusText = "Error: List is empty!";
        return;
    }

    // Get the last value
    int value = 0;
    auto current = m_list.head();
    while (current) {
        value = current->data;
        current = current->next;
    }

    // Update status
    std::ostringstream oss;
    oss << "Deleting back node...";
    m_statusText = oss.str();

    // Animate deletion
    if (!m_visualNodes.empty()) {
        auto& node = m_visualNodes.back();

        Animation flashRed = createColorAnimation(node.color, colors::semantic::error, 0.3f);
        flashRed.onComplete = [this, value]() {
            // Actually delete from list
            m_list.deleteBack();
            syncVisuals();

            std::ostringstream oss;
            oss << "Deleted " << value << " from back";
            m_statusText = oss.str();
        };
        m_animator.enqueue(flashRed);
    }
}

void LinkedListVisualizer::deleteAtValue(size_t index) {
    if (index >= m_list.size()) {
        m_statusText = "Error: Index out of range!";
        return;
    }

    // Get the value at index
    int value = 0;
    auto current = m_list.head();
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    value = current->data;

    // Update status
    std::ostringstream oss;
    oss << "Deleting node at index " << index << "...";
    m_statusText = oss.str();

    // Animate deletion
    if (index < m_visualNodes.size()) {
        auto& node = m_visualNodes[index];

        Animation flashRed = createColorAnimation(node.color, colors::semantic::error, 0.3f);
        flashRed.onComplete = [this, value, index]() {
            // Actually delete from list
            m_list.deleteAt(index);
            syncVisuals();

            std::ostringstream oss;
            oss << "Deleted " << value << " from index " << index;
            m_statusText = oss.str();
        };
        m_animator.enqueue(flashRed);
    }
}

void LinkedListVisualizer::searchValue(int value) {
    if (m_list.isEmpty()) {
        m_statusText = "Error: List is empty!";
        return;
    }

    // Update status
    std::ostringstream oss;
    oss << "Searching for " << value << "...";
    m_statusText = oss.str();

    // Animate: highlight each node being checked
    bool found = false;
    auto current = m_list.head();
    size_t index = 0;

    while (current) {
        if (index < m_visualNodes.size()) {
            auto& node = m_visualNodes[index];

            if (current->data == value && !found) {
                // Found! Highlight in green
                Animation highlightFound = createColorAnimation(
                    node.color,
                    colors::semantic::sorted,
                    0.3f
                );
                highlightFound.onComplete = [this, value, index]() {
                    std::ostringstream oss;
                    oss << "Found " << value << " at index " << index;
                    m_statusText = oss.str();
                };
                m_animator.enqueue(highlightFound);

                Animation restore = createColorAnimation(
                    node.color,
                    colors::semantic::elementBase,
                    0.3f
                );
                m_animator.enqueue(restore);
                found = true;
                break;
            } else {
                // Checking this node
                Animation checking = createColorAnimation(
                    node.color,
                    colors::semantic::comparing,
                    0.2f
                );
                m_animator.enqueue(checking);

                Animation restore = createColorAnimation(
                    node.color,
                    colors::semantic::elementBase,
                    0.2f
                );

                // If this is the last node and not found, update status
                if (!current->next && !found) {
                    restore.onComplete = [this, value]() {
                        std::ostringstream oss;
                        oss << "Value " << value << " not found in list";
                        m_statusText = oss.str();
                    };
                }

                m_animator.enqueue(restore);
            }
        }

        current = current->next;
        index++;
    }
}

void LinkedListVisualizer::initializeRandom(size_t count) {
    // Clear existing list
    m_list.clear();
    m_visualNodes.clear();
    m_animator.clear();

    // Update status
    std::ostringstream oss;
    oss << "Initializing list with " << count << " random nodes...";
    m_statusText = oss.str();

    // Generate random values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 99);

    // Populate list with random values
    for (size_t i = 0; i < count; ++i) {
        m_list.insertBack(dis(gen));
    }

    // Sync visuals (includes NULL node)
    syncVisuals();

    // Animate each node appearing with a cascade effect
    for (size_t i = 0; i < m_visualNodes.size(); ++i) {
        // Skip the NULL node for animation
        if (m_visualNodes[i].isNull) continue;

        auto& node = m_visualNodes[i];

        // Flash to show appearance
        Animation fadeIn = createColorAnimation(node.color, colors::semantic::elementBase, 0.15f);
        if (i == m_visualNodes.size() - 2) {  // -2 because last is NULL node
            fadeIn.onComplete = [this, count]() {
                std::ostringstream oss;
                oss << "Initialized list with " << count << " random nodes";
                m_statusText = oss.str();
            };
        }
        m_animator.enqueue(fadeIn);
    }

    // Reset camera position and zoom
    m_cameraOffsetX = 0.0f;
    m_zoomLevel = 1.0f;
}

void LinkedListVisualizer::play() {
    m_isPaused = false;
    m_animator.setPaused(false);
}

void LinkedListVisualizer::pause() {
    m_isPaused = true;
    m_animator.setPaused(true);
}

void LinkedListVisualizer::step() {
    // TODO: Implement step functionality
    m_statusText = "Step forward (not yet implemented)";
}

void LinkedListVisualizer::reset() {
    m_list.clear();
    m_visualNodes.clear();
    m_animator.clear();

    // Add initial nodes
    m_list.insertBack(10);
    m_list.insertBack(20);
    m_list.insertBack(30);
    syncVisuals();

    m_statusText = "List reset";
    m_isPaused = true;
}

void LinkedListVisualizer::setSpeed(float speed) {
    m_speed = speed;
    m_animator.setSpeedMultiplier(speed);
}

std::string LinkedListVisualizer::getStatusText() const {
    return m_statusText;
}

bool LinkedListVisualizer::isAnimating() const {
    return m_animator.hasAnimations();
}

bool LinkedListVisualizer::isPaused() const {
    return m_isPaused;
}

void LinkedListVisualizer::syncVisuals() {
    m_visualNodes.clear();

    auto current = m_list.head();
    size_t index = 0;

    // Add regular data nodes
    while (current) {
        VisualNode vnode;
        vnode.position = calculatePosition(index);
        vnode.size = glm::vec2(NODE_WIDTH, NODE_HEIGHT);
        vnode.color = colors::semantic::elementBase;
        vnode.borderColor = colors::semantic::elementBorder;
        vnode.label = std::to_string(current->data);
        vnode.hasNext = true;  // Always has next (points to next node or NULL node)
        vnode.isNull = false;

        m_visualNodes.push_back(vnode);

        current = current->next;
        index++;
    }

    // Always add NULL terminator node at the end
    VisualNode nullNode;
    nullNode.position = calculatePosition(index);
    nullNode.size = glm::vec2(NODE_WIDTH, NODE_HEIGHT);
    nullNode.color = colors::withAlpha(colors::mocha::surface1, 0.5f);
    nullNode.borderColor = colors::mocha::overlay0;
    nullNode.label = "NULL";
    nullNode.hasNext = false;
    nullNode.isNull = true;

    m_visualNodes.push_back(nullNode);
}

glm::vec2 LinkedListVisualizer::calculatePosition(size_t index) const {
    // Nodes arranged horizontally
    float x = START_X + index * NODE_SPACING;
    float y = START_Y;
    return glm::vec2(x, y);
}

void LinkedListVisualizer::drawArrow(ImDrawList* drawList, const ImVec2& from, const ImVec2& to, ImU32 color) {
    // Draw line
    drawList->AddLine(from, to, color, 2.0f);

    // Draw arrowhead
    float angle = std::atan2(to.y - from.y, to.x - from.x);
    float arrowSize = 10.0f;

    ImVec2 p1 = ImVec2(
        to.x - arrowSize * std::cos(angle - 0.5f),
        to.y - arrowSize * std::sin(angle - 0.5f)
    );
    ImVec2 p2 = ImVec2(
        to.x - arrowSize * std::cos(angle + 0.5f),
        to.y - arrowSize * std::sin(angle + 0.5f)
    );

    drawList->AddTriangleFilled(to, p1, p2, color);
}

} // namespace dsav
