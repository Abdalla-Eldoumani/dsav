/**
 * @file queue_visualizer.cpp
 * @brief Implementation of Queue visualizer
 */

#include "visualizers/queue_visualizer.hpp"
#include "ui_components.hpp"
#include <sstream>
#include <iomanip>

namespace dsav {

QueueVisualizer::QueueVisualizer(size_t maxSize)
    : m_statusText("Queue is empty") {
    (void)maxSize;  // Unused for now, queue has fixed size
    syncVisuals();
}

void QueueVisualizer::update(float deltaTime) {
    // Update animations
    m_animator.update(deltaTime);

    // Update status if not animating
    if (!isAnimating()) {
        if (m_queue.isEmpty()) {
            m_statusText = "Queue is empty";
        } else {
            std::ostringstream oss;
            oss << "Queue has " << m_queue.size() << " element(s)";
            if (auto front = m_queue.peek()) {
                oss << " | Front: " << *front;
            }
            m_statusText = oss.str();
        }
    }
}

void QueueVisualizer::renderVisualization() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    // Draw background
    drawList->AddRectFilled(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::mantle))
    );

    // Calculate total width of queue capacity
    float totalQueueWidth = m_queue.capacity() * ELEMENT_WIDTH +
                            (m_queue.capacity() - 1) * ELEMENT_SPACING;

    // Calculate horizontal offset to center the queue
    float horizontalOffset = (canvasSize.x - totalQueueWidth) / 2.0f;

    // Ensure minimum padding
    if (horizontalOffset < 20.0f) {
        horizontalOffset = 20.0f;
    }

    // Draw capacity indicator (ghost boxes showing circular buffer)
    for (size_t i = 0; i < m_queue.capacity(); ++i) {
        glm::vec2 pos = calculatePosition(i);
        VisualElement ghost;
        ghost.position = glm::vec2(canvasPos.x + horizontalOffset + pos.x, canvasPos.y + pos.y);
        ghost.size = glm::vec2(ELEMENT_WIDTH, ELEMENT_HEIGHT);
        ghost.color = colors::withAlpha(colors::mocha::surface1, 0.3f);
        ghost.borderColor = colors::withAlpha(colors::mocha::overlay0, 0.5f);
        ghost.borderWidth = 1.0f;

        // Draw array index below ghost box
        std::string indexLabel = std::to_string(i);
        ImVec2 indexSize = ImGui::CalcTextSize(indexLabel.c_str());
        ImVec2 indexPos = ImVec2(
            ghost.position.x + (ELEMENT_WIDTH - indexSize.x) / 2.0f,
            ghost.position.y + ELEMENT_HEIGHT + 5.0f
        );
        drawList->AddText(
            indexPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
            indexLabel.c_str()
        );

        renderElement(drawList, ghost);
    }

    // Draw actual queue elements
    for (auto& elem : m_elements) {
        VisualElement renderElem = elem;
        renderElem.position.x += canvasPos.x + horizontalOffset;
        renderElem.position.y += canvasPos.y;
        renderElement(drawList, renderElem);
    }

    // Draw "FRONT" indicator
    if (!m_queue.isEmpty()) {
        glm::vec2 frontPos = calculatePosition(m_queue.frontIndex());
        ImVec2 frontArrowPos = ImVec2(
            canvasPos.x + horizontalOffset + frontPos.x + ELEMENT_WIDTH / 2.0f - 30.0f,
            canvasPos.y + frontPos.y - 25.0f
        );
        drawList->AddText(
            frontArrowPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::green)),
            "FRONT ↓"
        );
    }

    // Draw "REAR" indicator (where next element will be added)
    if (!m_queue.isFull()) {
        glm::vec2 rearPos = calculatePosition(m_queue.rearIndex());
        ImVec2 rearArrowPos = ImVec2(
            canvasPos.x + horizontalOffset + rearPos.x + ELEMENT_WIDTH / 2.0f - 25.0f,
            canvasPos.y + rearPos.y + ELEMENT_HEIGHT + 35.0f
        );
        drawList->AddText(
            rearArrowPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::blue)),
            "REAR ↑"
        );
    }

    // Draw circular buffer indicator
    ImVec2 circularTextPos = ImVec2(
        canvasPos.x + 20.0f,
        canvasPos.y + canvasSize.y - 30.0f
    );
    drawList->AddText(
        circularTextPos,
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
        "Circular Buffer: Elements wrap around when reaching the end"
    );
}

void QueueVisualizer::renderControls() {
    ImGui::Begin("Queue Controls");

    // Status
    ui::StatusText(m_statusText.c_str(), "info");
    ImGui::Separator();

    // Input and operations
    ImGui::Text("Operations:");
    ImGui::PushItemWidth(150.0f);
    ImGui::InputInt("Value", &m_inputValue);
    ImGui::PopItemWidth();

    ImGui::BeginDisabled(m_queue.isFull() || isAnimating());
    if (ui::ButtonSuccess("Enqueue", ImVec2(120, 0))) {
        enqueueValue(m_inputValue);
    }
    ImGui::EndDisabled();
    ui::Tooltip("Add element to rear of queue");

    ImGui::SameLine();

    ImGui::BeginDisabled(m_queue.isEmpty() || isAnimating());
    if (ui::ButtonDanger("Dequeue", ImVec2(120, 0))) {
        dequeueValue();
    }
    ImGui::EndDisabled();
    ui::Tooltip("Remove element from front of queue");

    ImGui::BeginDisabled(m_queue.isEmpty() || isAnimating());
    if (ImGui::Button("Peek", ImVec2(120, 0))) {
        peekValue();
    }
    ImGui::EndDisabled();
    ui::Tooltip("View front element without removing");

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

    // Queue info
    ImGui::Text("Queue Info:");
    ImGui::Text("Size: %zu / %zu", m_queue.size(), m_queue.capacity());
    ImGui::ProgressBar(
        static_cast<float>(m_queue.size()) / static_cast<float>(m_queue.capacity()),
        ImVec2(-1, 0)
    );

    if (!m_queue.isEmpty()) {
        ImGui::Text("Front Index: %zu", m_queue.frontIndex());
        ImGui::Text("Rear Index: %zu", m_queue.rearIndex());
    }

    ImGui::End();
}

void QueueVisualizer::enqueueValue(int value) {
    // Check if queue is full
    if (m_queue.isFull()) {
        m_statusText = "Error: Queue Overflow!";
        return;
    }

    // Get the rear index before enqueueing
    size_t rearIdx = m_queue.rearIndex();

    // Enqueue to actual queue
    m_queue.enqueue(value);

    // Update status
    std::ostringstream oss;
    oss << "Enqueueing " << value << "...";
    m_statusText = oss.str();

    // Create visual element for the new value at rear position
    glm::vec2 targetPos = calculatePosition(rearIdx);

    VisualElement newElem;
    newElem.position = glm::vec2(targetPos.x + ELEMENT_WIDTH + 100.0f, targetPos.y);  // Start off-screen right
    newElem.size = glm::vec2(ELEMENT_WIDTH, ELEMENT_HEIGHT);
    newElem.color = colors::semantic::elementBase;
    newElem.borderColor = colors::semantic::active;
    newElem.label = std::to_string(value);
    newElem.sublabel = "";

    m_elements.push_back(newElem);

    // Animate: Slide in from right
    auto& elem = m_elements.back();
    Animation slideIn = createMoveAnimation(elem.position, targetPos, 0.4f);
    slideIn.easingFn = easing::easeOutBack;
    m_animator.enqueue(slideIn);

    // Flash green to indicate success
    Animation flashGreen = createColorAnimation(elem.color, colors::semantic::sorted, 0.2f);
    m_animator.enqueue(flashGreen);

    Animation flashBack = createColorAnimation(elem.color, colors::semantic::elementBase, 0.2f);
    flashBack.onComplete = [this, value]() {
        std::ostringstream oss;
        oss << "Enqueued " << value << " successfully";
        m_statusText = oss.str();
    };
    m_animator.enqueue(flashBack);
}

void QueueVisualizer::dequeueValue() {
    // Check if queue is empty
    if (m_queue.isEmpty()) {
        m_statusText = "Error: Queue Underflow!";
        return;
    }

    // Get the value before dequeueing
    auto value = m_queue.peek();
    if (!value) return;

    // Update status
    std::ostringstream oss;
    oss << "Dequeueing " << *value << "...";
    m_statusText = oss.str();

    // Find the element at the front (first in m_elements)
    if (m_elements.empty()) return;

    auto& elem = m_elements.front();

    // Animate: Flash red, then slide left and fade out
    Animation flashRed = createColorAnimation(elem.color, colors::semantic::error, 0.2f);
    m_animator.enqueue(flashRed);

    Animation slideOut = createMoveAnimation(
        elem.position,
        glm::vec2(elem.position.x - 150.0f, elem.position.y),
        0.4f
    );
    slideOut.easingFn = easing::easeIn;
    slideOut.onComplete = [this, value]() {
        // Remove visual element
        if (!m_elements.empty()) {
            m_elements.erase(m_elements.begin());
        }

        std::ostringstream oss;
        oss << "Dequeued " << *value << " successfully";
        m_statusText = oss.str();
    };
    m_animator.enqueue(slideOut);

    // Dequeue from actual queue
    m_queue.dequeue();
}

void QueueVisualizer::peekValue() {
    auto value = m_queue.peek();
    if (value) {
        std::ostringstream oss;
        oss << "Front element: " << *value;
        m_statusText = oss.str();

        // Highlight the front element briefly
        if (!m_elements.empty()) {
            auto& elem = m_elements.front();
            glm::vec4 originalColor = elem.color;

            Animation highlight = createColorAnimation(elem.color, colors::semantic::highlight, 0.3f);
            m_animator.enqueue(highlight);

            Animation restore = createColorAnimation(elem.color, originalColor, 0.3f);
            m_animator.enqueue(restore);
        }
    }
}

void QueueVisualizer::play() {
    m_isPaused = false;
    m_animator.setPaused(false);
}

void QueueVisualizer::pause() {
    m_isPaused = true;
    m_animator.setPaused(true);
}

void QueueVisualizer::step() {
    // TODO: Implement step functionality
    m_statusText = "Step forward (not yet implemented)";
}

void QueueVisualizer::reset() {
    m_queue.clear();
    m_elements.clear();
    m_animator.clear();
    m_statusText = "Queue reset";
    m_isPaused = true;
}

void QueueVisualizer::setSpeed(float speed) {
    m_speed = speed;
    m_animator.setSpeedMultiplier(speed);
}

std::string QueueVisualizer::getStatusText() const {
    return m_statusText;
}

bool QueueVisualizer::isAnimating() const {
    return m_animator.hasAnimations();
}

bool QueueVisualizer::isPaused() const {
    return m_isPaused;
}

void QueueVisualizer::syncVisuals() {
    m_elements.clear();

    // Iterate through queue elements from front to rear
    for (size_t i = 0; i < m_queue.size(); ++i) {
        size_t actualIndex = (m_queue.frontIndex() + i) % m_queue.capacity();

        VisualElement elem;
        elem.position = calculatePosition(actualIndex);
        elem.size = glm::vec2(ELEMENT_WIDTH, ELEMENT_HEIGHT);
        elem.color = colors::semantic::elementBase;
        elem.borderColor = (i == 0)
            ? colors::semantic::active  // Highlight front
            : colors::semantic::elementBorder;
        elem.label = std::to_string(m_queue.atPosition(i));
        elem.sublabel = "";

        m_elements.push_back(elem);
    }
}

glm::vec2 QueueVisualizer::calculatePosition(size_t arrayIndex) const {
    // Elements arranged horizontally in a line (circular buffer layout)
    float x = arrayIndex * (ELEMENT_WIDTH + ELEMENT_SPACING);
    float y = START_Y;
    return glm::vec2(x, y);
}

} // namespace dsav
