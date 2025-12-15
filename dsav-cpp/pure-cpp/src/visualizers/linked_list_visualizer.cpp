/**
 * @file linked_list_visualizer.cpp
 * @brief Implementation of Linked List visualizer
 */

#include "visualizers/linked_list_visualizer.hpp"
#include "ui_components.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

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

    // Calculate total width and center horizontally
    float totalWidth = 0.0f;
    if (!m_visualNodes.empty()) {
        totalWidth = m_visualNodes.size() * NODE_WIDTH +
                     (m_visualNodes.size() - 1) * NODE_SPACING;
    }

    float horizontalOffset = (canvasSize.x - totalWidth) / 2.0f;
    if (horizontalOffset < 100.0f) {
        horizontalOffset = 100.0f;
    }

    // Draw "HEAD →" indicator
    if (!m_visualNodes.empty()) {
        ImVec2 headTextPos = ImVec2(
            canvasPos.x + horizontalOffset - 70.0f,
            canvasPos.y + START_Y + NODE_HEIGHT / 2.0f - 10.0f
        );
        drawList->AddText(
            headTextPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::green)),
            "HEAD →"
        );
    }

    // Draw nodes and arrows
    for (size_t i = 0; i < m_visualNodes.size(); ++i) {
        const auto& visualNode = m_visualNodes[i];

        // Create VisualElement for rendering
        VisualElement elem;
        elem.position = glm::vec2(
            canvasPos.x + horizontalOffset + visualNode.position.x,
            canvasPos.y + visualNode.position.y
        );
        elem.size = visualNode.size;
        elem.color = visualNode.color;
        elem.borderColor = visualNode.borderColor;
        elem.borderWidth = 2.0f;
        elem.label = visualNode.label;
        elem.sublabel = "";

        renderElement(drawList, elem);

        // Draw arrow to next node
        if (visualNode.hasNext && i < m_visualNodes.size() - 1) {
            ImVec2 arrowStart = ImVec2(
                elem.position.x + NODE_WIDTH,
                elem.position.y + NODE_HEIGHT / 2.0f
            );
            ImVec2 arrowEnd = ImVec2(
                canvasPos.x + horizontalOffset + m_visualNodes[i + 1].position.x,
                canvasPos.y + m_visualNodes[i + 1].position.y + NODE_HEIGHT / 2.0f
            );

            drawArrow(drawList, arrowStart, arrowEnd,
                     ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::blue)));
        }
    }

    // Draw "NULL" indicator at the end
    if (!m_visualNodes.empty()) {
        const auto& lastNode = m_visualNodes.back();
        ImVec2 nullPos = ImVec2(
            canvasPos.x + horizontalOffset + lastNode.position.x + NODE_WIDTH + 20.0f,
            canvasPos.y + lastNode.position.y + NODE_HEIGHT / 2.0f - 10.0f
        );
        drawList->AddText(
            nullPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
            "→ NULL"
        );
    }

    // Draw info text if empty
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

    while (current) {
        VisualNode vnode;
        vnode.position = calculatePosition(index);
        vnode.size = glm::vec2(NODE_WIDTH, NODE_HEIGHT);
        vnode.color = colors::semantic::elementBase;
        vnode.borderColor = colors::semantic::elementBorder;
        vnode.label = std::to_string(current->data);
        vnode.hasNext = (current->next != nullptr);

        m_visualNodes.push_back(vnode);

        current = current->next;
        index++;
    }
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
