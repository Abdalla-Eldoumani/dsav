/**
 * @file stack_visualizer.cpp
 * @brief Implementation of Stack visualizer
 */

#include "visualizers/stack_visualizer.hpp"
#include "ui_components.hpp"
#include <sstream>
#include <iomanip>

namespace dsav {

StackVisualizer::StackVisualizer(size_t maxSize)
    : m_statusText("Stack is empty") {
    (void)maxSize;  // Unused for now, stack has fixed size
    syncVisuals();
}

void StackVisualizer::update(float deltaTime) {
    // Update animations
    m_animator.update(deltaTime);

    // Update status if not animating
    if (!isAnimating()) {
        if (m_stack.isEmpty()) {
            m_statusText = "Stack is empty";
        } else {
            std::ostringstream oss;
            oss << "Stack has " << m_stack.size() << " element(s)";
            if (auto top = m_stack.peek()) {
                oss << " | Top: " << *top;
            }
            m_statusText = oss.str();
        }
    }
}

void StackVisualizer::renderVisualization() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    // Draw background
    drawList->AddRectFilled(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::mantle))
    );

    // Calculate total height of full stack capacity
    float totalStackHeight = m_stack.capacity() * ELEMENT_HEIGHT +
                             (m_stack.capacity() - 1) * ELEMENT_SPACING;

    // Calculate bottom padding to center the stack vertically
    float bottomPadding = (canvasSize.y - totalStackHeight) / 2.0f;

    // Ensure minimum padding
    if (bottomPadding < 20.0f) {
        bottomPadding = 20.0f;
    }

    // Lambda to convert local coordinates to screen coordinates
    // Local y=0 is at bottom, grows upward -> Screen y grows downward
    auto toScreenPos = [&](const glm::vec2& localPos) -> ImVec2 {
        return ImVec2(
            canvasPos.x + localPos.x,
            canvasPos.y + canvasSize.y - bottomPadding - localPos.y - ELEMENT_HEIGHT
        );
    };

    // Draw capacity indicator (ghost boxes showing max capacity)
    for (size_t i = 0; i < m_stack.capacity(); ++i) {
        glm::vec2 localPos = calculatePosition(i);
        ImVec2 screenPos = toScreenPos(localPos);

        VisualElement ghost;
        ghost.position = glm::vec2(screenPos.x, screenPos.y);
        ghost.size = glm::vec2(ELEMENT_WIDTH, ELEMENT_HEIGHT);
        ghost.color = colors::withAlpha(colors::mocha::surface1, 0.3f);
        ghost.borderColor = colors::withAlpha(colors::mocha::overlay0, 0.5f);
        ghost.borderWidth = 1.0f;

        renderElement(drawList, ghost);
    }

    // Draw actual stack elements
    for (auto& elem : m_elements) {
        ImVec2 screenPos = toScreenPos(elem.position);

        VisualElement renderElem = elem;
        renderElem.position = glm::vec2(screenPos.x, screenPos.y);
        renderElement(drawList, renderElem);
    }

    // Draw "TOP" label
    if (!m_stack.isEmpty()) {
        glm::vec2 topLocalPos = calculatePosition(m_stack.size() - 1);
        ImVec2 topScreenPos = toScreenPos(topLocalPos);
        ImVec2 labelPos = ImVec2(
            topScreenPos.x + ELEMENT_WIDTH + 20.0f,
            topScreenPos.y + ELEMENT_HEIGHT / 2.0f - 10.0f
        );
        drawList->AddText(
            labelPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::blue)),
            "← TOP"
        );
    }

    // Draw base label
    glm::vec2 baseLocalPos = calculatePosition(0);
    ImVec2 baseScreenPos = toScreenPos(baseLocalPos);
    ImVec2 basePos = ImVec2(
        baseScreenPos.x - 60.0f,
        baseScreenPos.y + ELEMENT_HEIGHT / 2.0f - 10.0f
    );
    drawList->AddText(
        basePos,
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
        "BASE →"
    );
}

void StackVisualizer::renderControls() {
    ImGui::Begin("Stack Controls");

    // Status
    ui::StatusText(m_statusText.c_str(), "info");
    ImGui::Separator();

    // Input and operations
    ImGui::Text("Operations:");
    ImGui::PushItemWidth(150.0f);
    ImGui::InputInt("Value", &m_inputValue);
    ImGui::PopItemWidth();

    ImGui::BeginDisabled(m_stack.isFull() || isAnimating());
    if (ui::ButtonSuccess("Push", ImVec2(100, 0))) {
        pushValue(m_inputValue);
    }
    ImGui::EndDisabled();
    ui::Tooltip("Add element to top of stack");

    ImGui::SameLine();

    ImGui::BeginDisabled(m_stack.isEmpty() || isAnimating());
    if (ui::ButtonDanger("Pop", ImVec2(100, 0))) {
        popValue();
    }
    ImGui::EndDisabled();
    ui::Tooltip("Remove element from top of stack");

    ImGui::BeginDisabled(m_stack.isEmpty() || isAnimating());
    if (ImGui::Button("Peek", ImVec2(100, 0))) {
        peekValue();
    }
    ImGui::EndDisabled();
    ui::Tooltip("View top element without removing");

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

    // Stack info
    ImGui::Text("Stack Info:");
    ImGui::Text("Size: %zu / %zu", m_stack.size(), m_stack.capacity());
    ImGui::ProgressBar(
        static_cast<float>(m_stack.size()) / static_cast<float>(m_stack.capacity()),
        ImVec2(-1, 0)
    );

    ImGui::End();
}

void StackVisualizer::pushValue(int value) {
    // Check if stack is full
    if (m_stack.isFull()) {
        m_statusText = "Error: Stack Overflow!";
        return;
    }

    // Push to actual stack
    m_stack.push(value);

    // Update status
    std::ostringstream oss;
    oss << "Pushing " << value << "...";
    m_statusText = oss.str();

    // Create visual element for the new value
    size_t newIndex = m_stack.size() - 1;
    glm::vec2 targetPos = calculatePosition(newIndex);

    VisualElement newElem;
    newElem.position = glm::vec2(targetPos.x, -100.0f);  // Start above screen
    newElem.size = glm::vec2(ELEMENT_WIDTH, ELEMENT_HEIGHT);
    newElem.color = colors::semantic::elementBase;
    newElem.borderColor = colors::semantic::active;
    newElem.label = std::to_string(value);
    newElem.sublabel = "[" + std::to_string(newIndex) + "]";

    m_elements.push_back(newElem);

    // Animate: Drop down from top
    auto& elem = m_elements.back();
    Animation dropAnim = createMoveAnimation(elem.position, targetPos, 0.4f);
    dropAnim.easingFn = easing::easeOutBounce;
    m_animator.enqueue(dropAnim);

    // Flash color to indicate success
    Animation flashGreen = createColorAnimation(elem.color, colors::semantic::sorted, 0.2f);
    m_animator.enqueue(flashGreen);

    Animation flashBack = createColorAnimation(elem.color, colors::semantic::elementBase, 0.2f);
    flashBack.onComplete = [this, value]() {
        std::ostringstream oss;
        oss << "Pushed " << value << " successfully";
        m_statusText = oss.str();
    };
    m_animator.enqueue(flashBack);
}

void StackVisualizer::popValue() {
    // Check if stack is empty
    if (m_stack.isEmpty()) {
        m_statusText = "Error: Stack Underflow!";
        return;
    }

    // Get the value before popping
    auto value = m_stack.peek();
    if (!value) return;

    // Update status
    std::ostringstream oss;
    oss << "Popping " << *value << "...";
    m_statusText = oss.str();

    // Animate: Flash red, then slide up and fade out
    auto& elem = m_elements.back();

    Animation flashRed = createColorAnimation(elem.color, colors::semantic::error, 0.2f);
    m_animator.enqueue(flashRed);

    Animation slideUp = createMoveAnimation(
        elem.position,
        glm::vec2(elem.position.x, -100.0f),
        0.4f
    );
    slideUp.easingFn = easing::easeIn;
    slideUp.onComplete = [this, value]() {
        // Remove visual element
        if (!m_elements.empty()) {
            m_elements.pop_back();
        }

        std::ostringstream oss;
        oss << "Popped " << *value << " successfully";
        m_statusText = oss.str();
    };
    m_animator.enqueue(slideUp);

    // Pop from actual stack
    m_stack.pop();
}

void StackVisualizer::peekValue() {
    auto value = m_stack.peek();
    if (value) {
        std::ostringstream oss;
        oss << "Top element: " << *value;
        m_statusText = oss.str();

        // Highlight the top element briefly
        if (!m_elements.empty()) {
            auto& elem = m_elements.back();
            glm::vec4 originalColor = elem.color;

            Animation highlight = createColorAnimation(elem.color, colors::semantic::highlight, 0.3f);
            m_animator.enqueue(highlight);

            Animation restore = createColorAnimation(elem.color, originalColor, 0.3f);
            m_animator.enqueue(restore);
        }
    }
}

void StackVisualizer::play() {
    m_isPaused = false;
    m_animator.setPaused(false);
}

void StackVisualizer::pause() {
    m_isPaused = true;
    m_animator.setPaused(true);
}

void StackVisualizer::step() {
    // TODO: Implement step functionality
    m_statusText = "Step forward (not yet implemented)";
}

void StackVisualizer::reset() {
    m_stack.clear();
    m_elements.clear();
    m_animator.clear();
    m_statusText = "Stack reset";
    m_isPaused = true;
}

void StackVisualizer::setSpeed(float speed) {
    m_speed = speed;
    m_animator.setSpeedMultiplier(speed);
}

std::string StackVisualizer::getStatusText() const {
    return m_statusText;
}

bool StackVisualizer::isAnimating() const {
    return m_animator.hasAnimations();
}

bool StackVisualizer::isPaused() const {
    return m_isPaused;
}

void StackVisualizer::syncVisuals() {
    m_elements.clear();

    for (size_t i = 0; i < m_stack.size(); ++i) {
        VisualElement elem;
        elem.position = calculatePosition(i);
        elem.size = glm::vec2(ELEMENT_WIDTH, ELEMENT_HEIGHT);
        elem.color = colors::semantic::elementBase;
        elem.borderColor = (i == m_stack.size() - 1)
            ? colors::semantic::active
            : colors::semantic::elementBorder;
        elem.label = std::to_string(m_stack.at(i));
        elem.sublabel = "[" + std::to_string(i) + "]";

        m_elements.push_back(elem);
    }
}

glm::vec2 StackVisualizer::calculatePosition(size_t index) const {
    // Stack grows upward from bottom (index 0 at y=0)
    // Higher indices have larger y values (we'll flip this when rendering)
    float x = START_X;
    float y = static_cast<float>(index) * (ELEMENT_HEIGHT + ELEMENT_SPACING);
    return glm::vec2(x, y);
}

} // namespace dsav
