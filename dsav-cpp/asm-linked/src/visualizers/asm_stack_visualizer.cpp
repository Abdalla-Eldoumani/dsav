/**
 * @file asm_stack_visualizer.cpp
 * @brief Implementation of stack visualizer with assembly backend
 */

#include "asm_stack_visualizer.hpp"
#include <sstream>
#include <iomanip>

namespace dsav {

AsmStackVisualizer::AsmStackVisualizer() {
    // Initialize assembly stack (clears it)
    stack_clear();

    // Sync visual state
    syncFromAssembly();

    m_statusText = "Stack initialized (assembly backend)";
}

void AsmStackVisualizer::update(float deltaTime) {
    // Update animations
    m_animator.update(deltaTime * m_speed);
}

void AsmStackVisualizer::renderVisualization() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetCursorScreenPos();

    // Draw capacity indicator
    int capacity = stack_get_capacity();
    int top = stack_get_top();
    int size = top + 1;

    // Draw empty stack slots
    for (int i = 0; i < capacity; ++i) {
        glm::vec2 pos = calculatePosition(i);
        ImVec2 topLeft(windowPos.x + pos.x, windowPos.y + pos.y);
        ImVec2 bottomRight(topLeft.x + ELEMENT_WIDTH, topLeft.y + ELEMENT_HEIGHT);

        // Draw empty slot outline
        ImU32 outlineColor = IM_COL32(60, 60, 60, 100);
        drawList->AddRect(topLeft, bottomRight, outlineColor, 4.0f, 0, 1.5f);
    }

    // Draw stack elements
    for (const auto& element : m_elements) {
        glm::vec2 pos = element.position;
        ImVec2 topLeft(windowPos.x + pos.x, windowPos.y + pos.y);
        ImVec2 bottomRight(topLeft.x + ELEMENT_WIDTH, topLeft.y + ELEMENT_HEIGHT);

        // Draw filled rectangle
        ImU32 fillColor = ImGui::ColorConvertFloat4ToU32(colors::toImGui(element.color));
        ImU32 borderColor = ImGui::ColorConvertFloat4ToU32(colors::toImGui(element.borderColor));

        drawList->AddRectFilled(topLeft, bottomRight, fillColor, 4.0f);
        drawList->AddRect(topLeft, bottomRight, borderColor, 4.0f, 0, 2.0f);

        // Draw value text (centered)
        ImVec2 textSize = ImGui::CalcTextSize(element.label.c_str());
        ImVec2 textPos(
            topLeft.x + (ELEMENT_WIDTH - textSize.x) * 0.5f,
            topLeft.y + (ELEMENT_HEIGHT - textSize.y) * 0.5f
        );

        drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), element.label.c_str());

        // Draw sublabel (TOP indicator)
        if (!element.sublabel.empty()) {
            ImVec2 sublabelSize = ImGui::CalcTextSize(element.sublabel.c_str());
            ImVec2 sublabelPos(
                topLeft.x + (ELEMENT_WIDTH - sublabelSize.x) * 0.5f,
                topLeft.y - 20.0f
            );
            drawList->AddText(sublabelPos,
                ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::green)),
                element.sublabel.c_str());
        }
    }

    // Draw info text
    std::ostringstream info;
    info << "Size: " << size << " / " << capacity;
    ImVec2 infoPos(windowPos.x + START_X, windowPos.y + 20.0f);
    drawList->AddText(infoPos, IM_COL32(255, 255, 255, 255), info.str().c_str());
}

void AsmStackVisualizer::renderControls() {
    ImGui::Text("Stack Operations");
    ImGui::Separator();

    // Input for push value
    ImGui::SetNextItemWidth(150.0f);
    ImGui::InputInt("Value", &m_inputValue);

    // Push button
    if (ImGui::Button("Push", ImVec2(150, 0))) {
        pushValue(m_inputValue);
    }

    ImGui::SameLine();

    // Pop button
    if (ImGui::Button("Pop", ImVec2(150, 0))) {
        popValue();
    }

    // Peek button
    if (ImGui::Button("Peek", ImVec2(150, 0))) {
        peekValue();
    }

    ImGui::SameLine();

    // Clear button
    if (ImGui::Button("Clear", ImVec2(150, 0))) {
        reset();
    }

    ImGui::Separator();

    // Status text
    ImGui::TextWrapped("%s", m_statusText.c_str());

    // Display last peeked value
    if (m_lastPeekedValue != 0) {
        ImGui::TextColored(colors::toImGui(colors::mocha::green),
            "Last Peeked: %d", m_lastPeekedValue);
    }

    // Display last popped value
    if (m_lastPoppedValue != 0) {
        ImGui::TextColored(colors::toImGui(colors::mocha::yellow),
            "Last Popped: %d", m_lastPoppedValue);
    }
}

void AsmStackVisualizer::play() {
    m_isPaused = false;
}

void AsmStackVisualizer::pause() {
    m_isPaused = true;
}

void AsmStackVisualizer::step() {
    // Step through one animation frame
    m_animator.update(0.016f);  // ~60fps
}

void AsmStackVisualizer::reset() {
    // Clear assembly stack
    stack_clear();

    // Clear animations
    m_animator.clear();

    // Sync visuals
    syncFromAssembly();

    m_statusText = "Stack cleared";
    m_lastPoppedValue = 0;
    m_lastPeekedValue = 0;
}

void AsmStackVisualizer::setSpeed(float speed) {
    m_speed = speed;
}

std::string AsmStackVisualizer::getStatusText() const {
    return m_statusText;
}

bool AsmStackVisualizer::isAnimating() const {
    return m_animator.hasAnimations();
}

bool AsmStackVisualizer::isPaused() const {
    return m_isPaused;
}

void AsmStackVisualizer::pushValue(int value) {
    // Call assembly function
    int success = stack_push(value);

    if (success) {
        // Animation: Element appears and drops into place
        size_t oldSize = m_elements.size();

        // Sync from assembly to get new state
        syncFromAssembly();

        if (m_elements.size() > oldSize) {
            // Animate the new element dropping in
            auto& newElement = m_elements.back();
            float targetY = newElement.position.y;
            newElement.position.y = -100.0f;  // Start above view

            // Create drop animation
            m_animator.enqueue(createMoveAnimation(
                newElement.position,
                glm::vec2(newElement.position.x, targetY),
                0.3f
            ));

            // Flash green to indicate success
            m_animator.enqueue(createColorAnimation(
                newElement.color,
                colors::semantic::sorted,
                0.2f
            ));
            m_animator.enqueue(createColorAnimation(
                newElement.color,
                colors::semantic::elementBase,
                0.2f
            ));
        }

        std::ostringstream oss;
        oss << "Pushed " << value;
        m_statusText = oss.str();
    } else {
        m_statusText = "Stack Overflow! Cannot push.";
    }
}

void AsmStackVisualizer::popValue() {
    int value = 0;
    int success = stack_pop(&value);

    if (success) {
        m_lastPoppedValue = value;

        // Sync from assembly immediately
        syncFromAssembly();

        std::ostringstream oss;
        oss << "Popped " << value;
        m_statusText = oss.str();
    } else {
        m_statusText = "Stack Underflow! Cannot pop.";
    }
}

void AsmStackVisualizer::peekValue() {
    int value = 0;
    int success = stack_peek(&value);

    if (success) {
        m_lastPeekedValue = value;

        // Flash the top element
        if (!m_elements.empty()) {
            auto& topElement = m_elements.back();

            m_animator.enqueue(createColorAnimation(
                topElement.color,
                colors::mocha::blue,
                0.15f
            ));
            m_animator.enqueue(createColorAnimation(
                topElement.color,
                colors::semantic::elementBase,
                0.15f
            ));
        }

        std::ostringstream oss;
        oss << "Peeked: " << value;
        m_statusText = oss.str();
    } else {
        m_statusText = "Stack is empty! Cannot peek.";
    }
}

void AsmStackVisualizer::syncFromAssembly() {
    // Read current state from assembly
    int* data = stack_get_data();
    int top = stack_get_top();
    int capacity = stack_get_capacity();

    // Clear existing visual elements
    m_elements.clear();

    // Create visual elements for each stack item
    for (int i = 0; i <= top && i < capacity; ++i) {
        VisualElement elem;
        elem.label = std::to_string(data[i]);
        elem.sublabel = (i == top) ? "TOP" : "";
        elem.position = calculatePosition(i);
        elem.size = glm::vec2(ELEMENT_WIDTH, ELEMENT_HEIGHT);
        elem.color = colors::semantic::elementBase;
        elem.borderColor = (i == top) ? colors::semantic::active : colors::semantic::elementBorder;
        m_elements.push_back(elem);
    }
}

glm::vec2 AsmStackVisualizer::calculatePosition(size_t index) const {
    float x = START_X;
    float y = START_Y - (index * (ELEMENT_HEIGHT + ELEMENT_SPACING));
    return glm::vec2(x, y);
}

} // namespace dsav
