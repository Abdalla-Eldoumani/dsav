/**
 * @file stack_visualizer.cpp
 * @brief Implementation of Stack visualizer
 */

#include "visualizers/stack_visualizer.hpp"
#include "ui_components.hpp"
#include <sstream>
#include <iomanip>
#include <random>
#include <ctime>

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

    // Calculate scaled dimensions with zoom
    float scaledElementWidth = ELEMENT_WIDTH * m_zoomLevel;
    float scaledElementHeight = ELEMENT_HEIGHT * m_zoomLevel;
    float scaledSpacing = ELEMENT_SPACING * m_zoomLevel;

    // Calculate total height of full stack capacity with zoom
    float totalStackHeight = m_stack.capacity() * scaledElementHeight +
                             (m_stack.capacity() - 1) * scaledSpacing;

    // Calculate bottom padding to center the stack vertically
    float bottomPadding = (canvasSize.y - totalStackHeight) / 2.0f;
    if (bottomPadding < 20.0f) {
        bottomPadding = 20.0f;
    }

    // Apply camera offset
    bottomPadding += m_cameraOffsetY;

    // Calculate interaction hitbox (smaller, only where elements are)
    float hitboxPadding = 40.0f;
    ImVec2 hitboxPos = ImVec2(
        canvasPos.x + START_X - hitboxPadding,
        canvasPos.y + std::max(20.0f, canvasSize.y - bottomPadding - totalStackHeight - hitboxPadding)
    );
    ImVec2 hitboxSize = ImVec2(
        scaledElementWidth + hitboxPadding * 2 + 100.0f, // Include space for labels
        std::min(canvasSize.y - 40.0f, totalStackHeight + hitboxPadding * 2)
    );

    // Make sure hitbox stays within canvas
    if (hitboxPos.y < canvasPos.y + 20.0f) {
        hitboxSize.y -= (canvasPos.y + 20.0f - hitboxPos.y);
        hitboxPos.y = canvasPos.y + 20.0f;
    }
    if (hitboxPos.y + hitboxSize.y > canvasPos.y + canvasSize.y - 20.0f) {
        hitboxSize.y = canvasPos.y + canvasSize.y - 20.0f - hitboxPos.y;
    }

    // Set cursor position for the hitbox
    ImGui::SetCursorScreenPos(hitboxPos);
    ImGui::InvisibleButton("stack_canvas", hitboxSize);
    bool isHovered = ImGui::IsItemHovered();
    bool isActive = ImGui::IsItemActive();

    // Handle mouse drag for panning
    if (isActive && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        if (!m_isDragging) {
            m_isDragging = true;
            m_lastMousePos = ImGui::GetMousePos();
        } else {
            ImVec2 currentMousePos = ImGui::GetMousePos();
            float deltaY = currentMousePos.y - m_lastMousePos.y;
            m_cameraOffsetY += deltaY;
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
                float mouseRelativeY = mousePos.y - canvasPos.y - (canvasSize.y - bottomPadding);
                float zoomRatio = m_zoomLevel / oldZoom;

                // Adjust offset so zoom focuses on mouse position
                m_cameraOffsetY = m_cameraOffsetY * zoomRatio + mouseRelativeY * (1.0f - zoomRatio);
            } else {
                // Regular vertical scrolling
                m_cameraOffsetY += wheel * 50.0f;
            }
        }
    }

    // Recalculate with potentially updated zoom
    scaledElementWidth = ELEMENT_WIDTH * m_zoomLevel;
    scaledElementHeight = ELEMENT_HEIGHT * m_zoomLevel;
    scaledSpacing = ELEMENT_SPACING * m_zoomLevel;

    totalStackHeight = m_stack.capacity() * scaledElementHeight +
                      (m_stack.capacity() - 1) * scaledSpacing;

    bottomPadding = (canvasSize.y - totalStackHeight) / 2.0f;
    if (bottomPadding < 20.0f) {
        bottomPadding = 20.0f;
    }
    bottomPadding += m_cameraOffsetY;

    // Clamp camera offset to prevent scrolling too far
    float minOffset = -totalStackHeight + canvasSize.y - 40.0f;
    float maxOffset = 0.0f;
    if (minOffset > maxOffset) minOffset = maxOffset;

    if (m_cameraOffsetY > (maxOffset + ((canvasSize.y - totalStackHeight) / 2.0f) - 20.0f)) {
        m_cameraOffsetY = maxOffset + ((canvasSize.y - totalStackHeight) / 2.0f) - 20.0f;
        bottomPadding = maxOffset + (canvasSize.y - totalStackHeight) / 2.0f - 20.0f + 20.0f;
    }
    if (bottomPadding - (canvasSize.y - totalStackHeight) / 2.0f < minOffset) {
        m_cameraOffsetY = minOffset - ((canvasSize.y - totalStackHeight) / 2.0f);
        bottomPadding = minOffset + (canvasSize.y - totalStackHeight) / 2.0f;
    }

    // Lambda to convert local coordinates to screen coordinates with zoom
    auto toScreenPos = [&](const glm::vec2& localPos, float elemHeight) -> ImVec2 {
        float scaledY = localPos.y * m_zoomLevel;
        return ImVec2(
            canvasPos.x + START_X,
            canvasPos.y + canvasSize.y - bottomPadding - scaledY - elemHeight
        );
    };

    // Draw capacity indicator (ghost boxes showing max capacity)
    for (size_t i = 0; i < m_stack.capacity(); ++i) {
        glm::vec2 localPos = calculatePosition(i);
        ImVec2 screenPos = toScreenPos(localPos, scaledElementHeight);

        VisualElement ghost;
        ghost.position = glm::vec2(screenPos.x, screenPos.y);
        ghost.size = glm::vec2(scaledElementWidth, scaledElementHeight);
        ghost.color = colors::withAlpha(colors::mocha::surface1, 0.3f);
        ghost.borderColor = colors::withAlpha(colors::mocha::overlay0, 0.5f);
        ghost.borderWidth = 1.0f;

        renderElement(drawList, ghost);
    }

    // Draw actual stack elements with zoom
    for (size_t i = 0; i < m_elements.size(); ++i) {
        glm::vec2 localPos = calculatePosition(i);
        ImVec2 screenPos = toScreenPos(localPos, scaledElementHeight);

        VisualElement renderElem = m_elements[i];
        renderElem.position = glm::vec2(screenPos.x, screenPos.y);
        renderElem.size = glm::vec2(scaledElementWidth, scaledElementHeight);
        renderElement(drawList, renderElem);
    }

    // Draw "TOP" label
    if (!m_stack.isEmpty()) {
        glm::vec2 topLocalPos = calculatePosition(m_stack.size() - 1);
        ImVec2 topScreenPos = toScreenPos(topLocalPos, scaledElementHeight);
        ImVec2 labelPos = ImVec2(
            topScreenPos.x + scaledElementWidth + 20.0f,
            topScreenPos.y + scaledElementHeight / 2.0f - 10.0f
        );
        drawList->AddText(
            labelPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::blue)),
            "← TOP"
        );
    }

    // Draw base label
    glm::vec2 baseLocalPos = calculatePosition(0);
    ImVec2 baseScreenPos = toScreenPos(baseLocalPos, scaledElementHeight);
    ImVec2 basePos = ImVec2(
        baseScreenPos.x - 60.0f,
        baseScreenPos.y + scaledElementHeight / 2.0f - 10.0f
    );
    drawList->AddText(
        basePos,
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
        "BASE →"
    );

    // Show interaction hints
    if (!m_stack.isEmpty()) {
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

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Initialize operation
    ImGui::Text("Initialize:");
    ImGui::PushItemWidth(150.0f);
    ImGui::InputInt("Count", &m_initCount);
    if (m_initCount < 1) m_initCount = 1;
    if (m_initCount > static_cast<int>(m_stack.capacity())) {
        m_initCount = static_cast<int>(m_stack.capacity());
    }
    ImGui::PopItemWidth();

    ImGui::BeginDisabled(isAnimating());
    if (ui::ButtonPrimary("Initialize Random", ImVec2(200, 0))) {
        initializeRandom(static_cast<size_t>(m_initCount));
    }
    ImGui::EndDisabled();
    ui::Tooltip("Fill stack with random values (clears existing stack)");

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

void StackVisualizer::initializeRandom(size_t count) {
    // Clear existing stack
    m_stack.clear();
    m_elements.clear();
    m_animator.clear();

    // Clamp count to capacity
    if (count > m_stack.capacity()) {
        count = m_stack.capacity();
    }

    // Update status
    std::ostringstream oss;
    oss << "Initializing stack with " << count << " random elements...";
    m_statusText = oss.str();

    // Generate random values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 99);

    // Populate stack with random values
    for (size_t i = 0; i < count; ++i) {
        m_stack.push(dis(gen));
    }

    // Sync visuals
    syncVisuals();

    // Animate each element appearing with a cascade effect from bottom to top
    for (size_t i = 0; i < m_elements.size(); ++i) {
        auto& elem = m_elements[i];

        // Flash to show appearance
        Animation fadeIn = createColorAnimation(elem.color, colors::semantic::elementBase, 0.15f);
        if (i == m_elements.size() - 1) {
            fadeIn.onComplete = [this, count]() {
                std::ostringstream oss;
                oss << "Initialized stack with " << count << " random elements";
                m_statusText = oss.str();
            };
        }
        m_animator.enqueue(fadeIn);
    }

    // Reset camera position and zoom
    m_cameraOffsetY = 0.0f;
    m_zoomLevel = 1.0f;
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
