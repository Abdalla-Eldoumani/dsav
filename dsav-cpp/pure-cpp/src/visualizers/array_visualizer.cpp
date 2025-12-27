/**
 * @file array_visualizer.cpp
 * @brief Implementation of Array visualizer
 */

#include "visualizers/array_visualizer.hpp"
#include "ui_components.hpp"
#include <sstream>
#include <iomanip>
#include <random>
#include <ctime>

namespace dsav {

ArrayVisualizer::ArrayVisualizer()
    : m_statusText("Array is empty") {
    // Initialize with a few elements for demonstration
    m_array.pushBack(10);
    m_array.pushBack(20);
    m_array.pushBack(30);
    syncVisuals();
}

void ArrayVisualizer::update(float deltaTime) {
    // Update animations
    m_animator.update(deltaTime);

    // Update status if not animating
    if (!isAnimating()) {
        if (m_array.isEmpty()) {
            m_statusText = "Array is empty";
        } else {
            std::ostringstream oss;
            oss << "Array has " << m_array.size() << " element(s)";
            m_statusText = oss.str();
        }
    }
}

void ArrayVisualizer::renderVisualization() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    // Draw background
    drawList->AddRectFilled(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::mantle))
    );

    // Calculate total width with zoom applied
    float scaledElementWidth = ELEMENT_WIDTH * m_zoomLevel;
    float scaledElementHeight = ELEMENT_HEIGHT * m_zoomLevel;
    float scaledSpacing = ELEMENT_SPACING * m_zoomLevel;

    float totalWidth = 0.0f;
    if (!m_elements.empty()) {
        totalWidth = m_elements.size() * scaledElementWidth +
                     (m_elements.size() - 1) * scaledSpacing;
    }

    // Calculate horizontal offset for centering
    float horizontalOffset = (canvasSize.x - totalWidth) / 2.0f;
    if (horizontalOffset < 20.0f) {
        horizontalOffset = 20.0f;
    }

    // Apply camera offset
    horizontalOffset += m_cameraOffsetX;

    // Calculate interaction hitbox (smaller, only where elements are)
    float hitboxPadding = 40.0f;
    ImVec2 hitboxPos = ImVec2(
        canvasPos.x + std::max(20.0f, horizontalOffset - hitboxPadding),
        canvasPos.y + START_Y - hitboxPadding
    );
    ImVec2 hitboxSize = ImVec2(
        std::min(canvasSize.x - 40.0f, totalWidth + hitboxPadding * 2),
        scaledElementHeight + 80.0f + hitboxPadding * 2 // Include space for index labels
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
    ImGui::InvisibleButton("array_canvas", hitboxSize);
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
                float zoomDelta = wheel * 0.1f; // Zoom speed
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
                m_cameraOffsetX += wheel * 50.0f; // Scroll speed multiplier
            }
        }
    }

    // Recalculate with potentially updated zoom
    scaledElementWidth = ELEMENT_WIDTH * m_zoomLevel;
    scaledElementHeight = ELEMENT_HEIGHT * m_zoomLevel;
    scaledSpacing = ELEMENT_SPACING * m_zoomLevel;

    totalWidth = 0.0f;
    if (!m_elements.empty()) {
        totalWidth = m_elements.size() * scaledElementWidth +
                     (m_elements.size() - 1) * scaledSpacing;
    }

    horizontalOffset = (canvasSize.x - totalWidth) / 2.0f;
    if (horizontalOffset < 20.0f) {
        horizontalOffset = 20.0f;
    }
    horizontalOffset += m_cameraOffsetX;

    // Clamp camera offset to prevent scrolling too far
    float maxOffset = 0.0f;
    float minOffset = canvasSize.x - totalWidth - 40.0f;
    if (minOffset > maxOffset) minOffset = maxOffset;

    if (m_cameraOffsetX > (maxOffset - ((canvasSize.x - totalWidth) / 2.0f) + 20.0f)) {
        m_cameraOffsetX = maxOffset - ((canvasSize.x - totalWidth) / 2.0f) + 20.0f;
        horizontalOffset = maxOffset + 20.0f;
    }
    if (horizontalOffset < minOffset) {
        m_cameraOffsetX = minOffset - ((canvasSize.x - totalWidth) / 2.0f);
        horizontalOffset = minOffset;
    }

    // Draw array elements with zoom applied
    for (size_t i = 0; i < m_elements.size(); ++i) {
        VisualElement renderElem = m_elements[i];

        // Apply zoom to position and size
        float scaledX = m_elements[i].position.x * m_zoomLevel;
        renderElem.position.x = canvasPos.x + horizontalOffset + scaledX;
        renderElem.position.y = canvasPos.y + START_Y;
        renderElem.size = glm::vec2(scaledElementWidth, scaledElementHeight);

        renderElement(drawList, renderElem);

        // Draw index below element
        std::string indexLabel = "[" + std::to_string(i) + "]";
        ImVec2 indexSize = ImGui::CalcTextSize(indexLabel.c_str());
        ImVec2 indexPos = ImVec2(
            renderElem.position.x + (scaledElementWidth - indexSize.x) / 2.0f,
            renderElem.position.y + scaledElementHeight + 5.0f
        );
        drawList->AddText(
            indexPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
            indexLabel.c_str()
        );
    }

    // Draw info text
    if (m_array.isEmpty()) {
        ImVec2 textPos = ImVec2(
            canvasPos.x + canvasSize.x / 2.0f - 100.0f,
            canvasPos.y + canvasSize.y / 2.0f
        );
        drawList->AddText(
            textPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::overlay1)),
            "Array is empty. Use Insert to add elements."
        );
    } else {
        // Show interaction hints
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

void ArrayVisualizer::renderControls() {
    ImGui::Begin("Array Controls");

    // Status
    ui::StatusText(m_statusText.c_str(), "info");
    ImGui::Separator();

    // Operation mode selection
    ImGui::Text("Operation Mode:");
    const char* modes[] = { "Insert", "Delete", "Search", "Access", "Update", "Initialize" };
    int currentModeIdx = static_cast<int>(m_currentMode);
    if (ImGui::Combo("##Mode", &currentModeIdx, modes, IM_ARRAYSIZE(modes))) {
        m_currentMode = static_cast<OperationMode>(currentModeIdx);
    }

    ImGui::Separator();

    // Input fields
    ImGui::Text("Parameters:");
    ImGui::PushItemWidth(150.0f);

    // Value input (for Insert, Search, Update)
    if (m_currentMode == OperationMode::Insert ||
        m_currentMode == OperationMode::Search ||
        m_currentMode == OperationMode::Update) {
        ImGui::InputInt("Value", &m_inputValue);
    }

    // Index input (for Insert, Delete, Access, Update)
    if (m_currentMode != OperationMode::Search && m_currentMode != OperationMode::Initialize) {
        ImGui::InputInt("Index", &m_inputIndex);
        if (m_inputIndex < 0) m_inputIndex = 0;
    }

    // Count input (for Initialize)
    if (m_currentMode == OperationMode::Initialize) {
        ImGui::InputInt("Count", &m_initCount);
        if (m_initCount < 1) m_initCount = 1;
        if (m_initCount > 20) m_initCount = 20;
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
            tooltipText = "Insert value at index (shifts elements right)";
            canExecute = static_cast<size_t>(m_inputIndex) <= m_array.size();
            break;
        case OperationMode::Delete:
            buttonLabel = "Delete";
            tooltipText = "Delete element at index (shifts elements left)";
            canExecute = !m_array.isEmpty() && static_cast<size_t>(m_inputIndex) < m_array.size();
            break;
        case OperationMode::Search:
            buttonLabel = "Search";
            tooltipText = "Search for value (linear search with animation)";
            canExecute = !m_array.isEmpty();
            break;
        case OperationMode::Access:
            buttonLabel = "Access";
            tooltipText = "Access element at index (highlight)";
            canExecute = !m_array.isEmpty() && static_cast<size_t>(m_inputIndex) < m_array.size();
            break;
        case OperationMode::Update:
            buttonLabel = "Update";
            tooltipText = "Update element at index with new value";
            canExecute = !m_array.isEmpty() && static_cast<size_t>(m_inputIndex) < m_array.size();
            break;
        case OperationMode::Initialize:
            buttonLabel = "Initialize Random";
            tooltipText = "Fill array with random values (clears existing array)";
            canExecute = true;
            break;
    }

    if (!canExecute) {
        ImGui::BeginDisabled();
    }

    if (ui::ButtonPrimary(buttonLabel.c_str(), ImVec2(200, 0))) {
        switch (m_currentMode) {
            case OperationMode::Insert:
                insertValue(static_cast<size_t>(m_inputIndex), m_inputValue);
                break;
            case OperationMode::Delete:
                deleteValue(static_cast<size_t>(m_inputIndex));
                break;
            case OperationMode::Search:
                searchValue(m_inputValue);
                break;
            case OperationMode::Access:
                accessValue(static_cast<size_t>(m_inputIndex));
                break;
            case OperationMode::Update:
                updateValue(static_cast<size_t>(m_inputIndex), m_inputValue);
                break;
            case OperationMode::Initialize:
                initializeRandomArray(static_cast<size_t>(m_initCount));
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

    // Array info
    ImGui::Text("Array Info:");
    ImGui::Text("Size: %zu", m_array.size());
    ImGui::Text("Capacity: %zu", m_array.capacity());

    ImGui::End();
}

void ArrayVisualizer::insertValue(size_t index, int value) {
    if (index > m_array.size()) {
        m_statusText = "Error: Index out of range!";
        return;
    }

    // Update status
    std::ostringstream oss;
    oss << "Inserting " << value << " at index " << index << "...";
    m_statusText = oss.str();

    // Insert into actual array
    m_array.insert(index, value);

    // Recreate all visual elements (we'll animate the shift)
    syncVisuals();

    // Animate: highlight new element
    if (index < m_elements.size()) {
        auto& newElem = m_elements[index];

        Animation flashGreen = createColorAnimation(newElem.color, colors::semantic::sorted, 0.3f);
        m_animator.enqueue(flashGreen);

        Animation flashBack = createColorAnimation(newElem.color, colors::semantic::elementBase, 0.3f);
        flashBack.onComplete = [this, value, index]() {
            std::ostringstream oss;
            oss << "Inserted " << value << " at index " << index;
            m_statusText = oss.str();
        };
        m_animator.enqueue(flashBack);
    }
}

void ArrayVisualizer::deleteValue(size_t index) {
    if (index >= m_array.size()) {
        m_statusText = "Error: Index out of range!";
        return;
    }

    int value = m_array[index];

    // Update status
    std::ostringstream oss;
    oss << "Deleting element at index " << index << "...";
    m_statusText = oss.str();

    // Animate deletion
    if (index < m_elements.size()) {
        auto& elem = m_elements[index];

        Animation flashRed = createColorAnimation(elem.color, colors::semantic::error, 0.3f);
        flashRed.onComplete = [this, index, value]() {
            // Actually delete from array
            m_array.deleteAt(index);
            syncVisuals();

            std::ostringstream oss;
            oss << "Deleted " << value << " from index " << index;
            m_statusText = oss.str();
        };
        m_animator.enqueue(flashRed);
    }
}

void ArrayVisualizer::searchValue(int value) {
    if (m_array.isEmpty()) {
        m_statusText = "Error: Array is empty!";
        return;
    }

    // Update status
    std::ostringstream oss;
    oss << "Searching for " << value << "...";
    m_statusText = oss.str();

    // Animate: highlight each element being checked
    bool found = false;
    for (size_t i = 0; i < m_array.size(); ++i) {
        if (i < m_elements.size()) {
            auto& elem = m_elements[i];

            if (m_array[i] == value && !found) {
                // Found! Highlight in green
                Animation highlightFound = createColorAnimation(
                    elem.color,
                    colors::semantic::sorted,
                    0.3f
                );
                highlightFound.onComplete = [this, value, i]() {
                    std::ostringstream oss;
                    oss << "Found " << value << " at index " << i;
                    m_statusText = oss.str();
                };
                m_animator.enqueue(highlightFound);

                Animation restore = createColorAnimation(
                    elem.color,
                    colors::semantic::elementBase,
                    0.3f
                );
                m_animator.enqueue(restore);
                found = true;
                break;
            } else {
                // Checking this element
                Animation checking = createColorAnimation(
                    elem.color,
                    colors::semantic::comparing,
                    0.2f
                );
                m_animator.enqueue(checking);

                Animation restore = createColorAnimation(
                    elem.color,
                    colors::semantic::elementBase,
                    0.2f
                );
                if (i == m_array.size() - 1 && !found) {
                    restore.onComplete = [this, value]() {
                        std::ostringstream oss;
                        oss << "Value " << value << " not found in array";
                        m_statusText = oss.str();
                    };
                }
                m_animator.enqueue(restore);
            }
        }
    }
}

void ArrayVisualizer::accessValue(size_t index) {
    if (index >= m_array.size()) {
        m_statusText = "Error: Index out of range!";
        return;
    }

    int value = m_array[index];

    // Update status
    std::ostringstream oss;
    oss << "Accessing index " << index << ": value = " << value;
    m_statusText = oss.str();

    // Animate: highlight accessed element
    if (index < m_elements.size()) {
        auto& elem = m_elements[index];

        Animation highlight = createColorAnimation(elem.color, colors::semantic::highlight, 0.3f);
        m_animator.enqueue(highlight);

        Animation restore = createColorAnimation(elem.color, colors::semantic::elementBase, 0.3f);
        m_animator.enqueue(restore);
    }
}

void ArrayVisualizer::updateValue(size_t index, int value) {
    if (index >= m_array.size()) {
        m_statusText = "Error: Index out of range!";
        return;
    }

    int oldValue = m_array[index];

    // Update status
    std::ostringstream oss;
    oss << "Updating index " << index << " from " << oldValue << " to " << value << "...";
    m_statusText = oss.str();

    // Animate: highlight, then update
    if (index < m_elements.size()) {
        auto& elem = m_elements[index];

        Animation highlight = createColorAnimation(elem.color, colors::semantic::comparing, 0.3f);
        highlight.onComplete = [this, index, value, oldValue]() {
            // Update the actual array
            m_array.update(index, value);
            // Update the visual label
            if (index < m_elements.size()) {
                m_elements[index].label = std::to_string(value);
            }

            std::ostringstream oss;
            oss << "Updated index " << index << " from " << oldValue << " to " << value;
            m_statusText = oss.str();
        };
        m_animator.enqueue(highlight);

        Animation restore = createColorAnimation(elem.color, colors::semantic::elementBase, 0.3f);
        m_animator.enqueue(restore);
    }
}

void ArrayVisualizer::initializeRandomArray(size_t count) {
    // Clear existing array
    m_array.clear();
    m_animator.clear();

    // Update status
    std::ostringstream oss;
    oss << "Initializing array with " << count << " random elements...";
    m_statusText = oss.str();

    // Generate random values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 99);

    // Populate array with random values
    for (size_t i = 0; i < count; ++i) {
        m_array.pushBack(dis(gen));
    }

    // Sync visuals
    syncVisuals();

    // Animate each element appearing with a cascade effect
    for (size_t i = 0; i < m_elements.size(); ++i) {
        auto& elem = m_elements[i];

        // Start with transparent/invisible color, fade in
        Animation fadeIn = createColorAnimation(elem.color, colors::semantic::elementBase, 0.2f);
        if (i == m_elements.size() - 1) {
            fadeIn.onComplete = [this, count]() {
                std::ostringstream oss;
                oss << "Initialized array with " << count << " random elements";
                m_statusText = oss.str();
            };
        }
        m_animator.enqueue(fadeIn);
    }

    // Reset camera position and zoom
    m_cameraOffsetX = 0.0f;
    m_zoomLevel = 1.0f;
}

void ArrayVisualizer::play() {
    m_isPaused = false;
    m_animator.setPaused(false);
}

void ArrayVisualizer::pause() {
    m_isPaused = true;
    m_animator.setPaused(true);
}

void ArrayVisualizer::step() {
    // TODO: Implement step functionality
    m_statusText = "Step forward (not yet implemented)";
}

void ArrayVisualizer::reset() {
    m_array.clear();
    m_elements.clear();
    m_animator.clear();

    // Add initial elements
    m_array.pushBack(10);
    m_array.pushBack(20);
    m_array.pushBack(30);
    syncVisuals();

    m_statusText = "Array reset";
    m_isPaused = true;
}

void ArrayVisualizer::setSpeed(float speed) {
    m_speed = speed;
    m_animator.setSpeedMultiplier(speed);
}

std::string ArrayVisualizer::getStatusText() const {
    return m_statusText;
}

bool ArrayVisualizer::isAnimating() const {
    return m_animator.hasAnimations();
}

bool ArrayVisualizer::isPaused() const {
    return m_isPaused;
}

void ArrayVisualizer::syncVisuals() {
    m_elements.clear();

    for (size_t i = 0; i < m_array.size(); ++i) {
        VisualElement elem;
        elem.position = calculatePosition(i);
        elem.size = glm::vec2(ELEMENT_WIDTH, ELEMENT_HEIGHT);
        elem.color = colors::semantic::elementBase;
        elem.borderColor = colors::semantic::elementBorder;
        elem.label = std::to_string(m_array[i]);
        elem.sublabel = "";

        m_elements.push_back(elem);
    }
}

glm::vec2 ArrayVisualizer::calculatePosition(size_t index) const {
    // Elements arranged horizontally
    float x = START_X + index * (ELEMENT_WIDTH + ELEMENT_SPACING);
    float y = START_Y;
    return glm::vec2(x, y);
}

} // namespace dsav
