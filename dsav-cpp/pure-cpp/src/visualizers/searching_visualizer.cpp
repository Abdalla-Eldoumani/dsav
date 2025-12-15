/**
 * @file searching_visualizer.cpp
 * @brief Implementation of search algorithm visualizer
 */

#include "visualizers/searching_visualizer.hpp"
#include "color_scheme.hpp"
#include "animation.hpp"
#include <algorithm>
#include <random>
#include <sstream>

namespace dsav {

SearchingVisualizer::SearchingVisualizer() {
    // Initialize with a random array
    randomizeArray();
    syncVisuals();
    m_statusText = "Ready to search. Set target value and click 'Start Search'.";
}

void SearchingVisualizer::update(float deltaTime) {
    // Update animations
    m_animator.update(deltaTime);

    // Auto-step when playing
    if (!m_isPaused && m_isSearching) {
        m_timeSinceLastStep += deltaTime * m_speed;

        // Convert delay from ms to seconds
        float delaySeconds = m_stepDelay / 1000.0f;

        if (m_timeSinceLastStep >= delaySeconds) {
            m_timeSinceLastStep = 0.0f;
            executeStep();
        }
    }
}

void SearchingVisualizer::renderVisualization() {
    ImGui::Begin("Search Visualization");

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    // Ensure minimum canvas size
    if (canvasSize.x < 50.0f) canvasSize.x = 900.0f;
    if (canvasSize.y < 50.0f) canvasSize.y = 600.0f;

    // Draw canvas background
    drawList->AddRectFilled(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::base))
    );

    // Draw elements as rectangles
    for (size_t i = 0; i < m_elements.size(); ++i) {
        const auto& elem = m_elements[i];

        ImVec2 topLeft = ImVec2(
            canvasPos.x + elem.position.x,
            canvasPos.y + elem.position.y
        );
        ImVec2 bottomRight = ImVec2(
            topLeft.x + ELEMENT_WIDTH,
            topLeft.y + ELEMENT_HEIGHT
        );

        // Draw filled rectangle
        drawList->AddRectFilled(
            topLeft,
            bottomRight,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(elem.color)),
            4.0f
        );

        // Draw border
        drawList->AddRect(
            topLeft,
            bottomRight,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(elem.borderColor)),
            4.0f,
            0,
            2.0f
        );

        // Draw value label centered
        ImVec2 labelPos = ImVec2(
            topLeft.x + (ELEMENT_WIDTH - ImGui::CalcTextSize(elem.label.c_str()).x) / 2.0f,
            topLeft.y + (ELEMENT_HEIGHT - ImGui::CalcTextSize(elem.label.c_str()).y) / 2.0f
        );
        drawList->AddText(
            labelPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::semantic::textPrimary)),
            elem.label.c_str()
        );

        // Draw index label below
        std::string indexLabel = "[" + std::to_string(i) + "]";
        ImVec2 indexPos = ImVec2(
            topLeft.x + (ELEMENT_WIDTH - ImGui::CalcTextSize(indexLabel.c_str()).x) / 2.0f,
            bottomRight.y + 5.0f
        );
        drawList->AddText(
            indexPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::semantic::textSecondary)),
            indexLabel.c_str()
        );
    }

    // Draw algorithm info and target
    std::string algoName;
    switch (m_currentAlgorithm) {
        case Algorithm::LinearSearch: algoName = "Linear Search"; break;
        case Algorithm::BinarySearch: algoName = "Binary Search"; break;
    }

    std::string targetInfo = algoName + " | Target: " + std::to_string(m_target);
    ImVec2 infoPos = ImVec2(canvasPos.x + 10.0f, canvasPos.y + 10.0f);
    drawList->AddText(
        infoPos,
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::semantic::active)),
        targetInfo.c_str()
    );

    // Draw bounds for binary search
    if (m_currentAlgorithm == Algorithm::BinarySearch && m_binarySearcher) {
        int left = m_binarySearcher->getLeftBound();
        int right = m_binarySearcher->getRightBound();

        if (left >= 0 && right >= 0 && left < static_cast<int>(m_elements.size()) &&
            right < static_cast<int>(m_elements.size())) {

            std::string boundsInfo = "Bounds: [" + std::to_string(left) + ", " + std::to_string(right) + "]";
            ImVec2 boundsPos = ImVec2(canvasPos.x + 10.0f, canvasPos.y + 30.0f);
            drawList->AddText(
                boundsPos,
                ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::yellow)),
                boundsInfo.c_str()
            );
        }
    }

    ImGui::Dummy(canvasSize);
    ImGui::End();
}

void SearchingVisualizer::renderControls() {
    ImGui::Begin("Search Controls");

    // Algorithm selection
    ImGui::Text("Algorithm:");
    static const char* algorithmNames[] = { "Linear Search", "Binary Search" };
    int currentAlgo = static_cast<int>(m_currentAlgorithm);
    if (ImGui::Combo("##Algorithm", &currentAlgo, algorithmNames, 2)) {
        m_currentAlgorithm = static_cast<Algorithm>(currentAlgo);
        reset();
    }

    ImGui::Separator();

    // Array controls
    ImGui::Text("Array Configuration:");
    if (ImGui::SliderInt("Array Size", &m_arraySize, 5, MAX_ARRAY_SIZE)) {
        if (!m_isSearching) {
            randomizeArray();
        }
    }

    if (ImGui::Button("Randomize Array", ImVec2(-1, 0))) {
        randomizeArray();
    }

    if (ImGui::Button("Sort Array (for Binary Search)", ImVec2(-1, 0))) {
        sortArray();
        syncVisuals();
        m_statusText = "Array sorted for binary search";
    }

    ImGui::Separator();

    // Target input
    ImGui::Text("Search Target:");
    ImGui::InputInt("Target Value", &m_target);
    m_target = std::clamp(m_target, 1, MAX_VALUE);

    ImGui::Separator();

    // Playback controls
    ImGui::Text("Playback:");

    ImGui::BeginGroup();
    if (ImGui::Button("⏮ Reset")) {
        reset();
    }
    ImGui::SameLine();

    if (m_isPaused) {
        if (ImGui::Button("▶ Play")) {
            play();
        }
    } else {
        if (ImGui::Button("⏸ Pause")) {
            pause();
        }
    }
    ImGui::SameLine();

    if (ImGui::Button("⏩ Step")) {
        step();
    }
    ImGui::SameLine();

    if (ImGui::Button("Start Search")) {
        startSearch();
    }
    ImGui::EndGroup();

    ImGui::Separator();

    // Speed control
    ImGui::Text("Speed:");
    if (ImGui::SliderFloat("##Speed", &m_speed, 0.1f, 5.0f, "%.1fx")) {
        setSpeed(m_speed);
    }

    if (ImGui::SliderInt("Step Delay (ms)", &m_stepDelay, 10, 2000)) {
        // Delay updated
    }

    ImGui::Separator();

    // Status
    ImGui::TextColored(
        colors::toImGui(colors::semantic::active),
        "Status:"
    );
    ImGui::TextWrapped("%s", m_statusText.c_str());

    // Statistics
    ImGui::Separator();
    ImGui::Text("Array Size: %zu", m_array.size());
    ImGui::Text("State: %s", m_isSearching ? "Searching" : "Idle");

    ImGui::End();
}

void SearchingVisualizer::play() {
    m_isPaused = false;
    if (!m_isSearching) {
        startSearch();
    }
    m_statusText = "Playing...";
}

void SearchingVisualizer::pause() {
    m_isPaused = true;
    m_statusText = "Paused";
}

void SearchingVisualizer::step() {
    if (!m_isSearching) {
        startSearch();
    }
    executeStep();
}

void SearchingVisualizer::reset() {
    m_isPaused = true;
    m_isSearching = false;
    m_timeSinceLastStep = 0.0f;
    m_animator.clear();

    // Reset algorithm steppers
    m_linearSearcher.reset();
    m_binarySearcher.reset();

    // Regenerate random array
    randomizeArray();

    m_statusText = "Reset complete. Ready to search.";
}

void SearchingVisualizer::setSpeed(float speed) {
    m_speed = std::clamp(speed, 0.1f, 5.0f);
    m_animator.setSpeedMultiplier(m_speed);
}

std::string SearchingVisualizer::getStatusText() const {
    return m_statusText;
}

bool SearchingVisualizer::isAnimating() const {
    return m_animator.hasAnimations();
}

bool SearchingVisualizer::isPaused() const {
    return m_isPaused;
}

void SearchingVisualizer::startSearch() {
    // Binary search requires sorted array
    if (m_currentAlgorithm == Algorithm::BinarySearch) {
        sortArray();
        syncVisuals();
    }

    m_isSearching = true;
    m_isPaused = false;
    m_timeSinceLastStep = 0.0f;

    // Create appropriate stepper based on selected algorithm
    switch (m_currentAlgorithm) {
        case Algorithm::LinearSearch:
            m_linearSearcher = std::make_unique<algorithms::LinearSearchStepper>(m_array, m_target);
            m_statusText = "Starting Linear Search for " + std::to_string(m_target) + "...";
            break;
        case Algorithm::BinarySearch:
            m_binarySearcher = std::make_unique<algorithms::BinarySearchStepper>(m_array, m_target);
            m_statusText = "Starting Binary Search for " + std::to_string(m_target) + "...";
            break;
    }

    syncVisuals();
}

void SearchingVisualizer::randomizeArray() {
    m_array.clear();
    m_array.resize(m_arraySize);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(10, MAX_VALUE);

    for (int& val : m_array) {
        val = dist(gen);
    }

    // Reset searching state
    m_isSearching = false;
    m_isPaused = true;

    syncVisuals();
}

void SearchingVisualizer::sortArray() {
    std::sort(m_array.begin(), m_array.end());
}

void SearchingVisualizer::setArray(const std::vector<int>& arr) {
    m_array = arr;
    m_arraySize = static_cast<int>(arr.size());
    m_isSearching = false;
    m_isPaused = true;
    syncVisuals();
}

void SearchingVisualizer::syncVisuals() {
    m_elements.clear();
    m_elements.reserve(m_array.size());

    for (size_t i = 0; i < m_array.size(); ++i) {
        VisualSearchElement elem;
        elem.position = calculatePosition(i);
        elem.size = glm::vec2(ELEMENT_WIDTH, ELEMENT_HEIGHT);
        elem.color = colors::semantic::elementBase;
        elem.borderColor = colors::semantic::active;
        elem.label = std::to_string(m_array[i]);
        elem.value = m_array[i];
        elem.isChecked = false;
        elem.isFound = false;

        m_elements.push_back(elem);
    }

    updateColors();
}

glm::vec2 SearchingVisualizer::calculatePosition(size_t index) const {
    float x = START_X + index * (ELEMENT_WIDTH + ELEMENT_SPACING);
    float y = START_Y;
    return glm::vec2(x, y);
}

void SearchingVisualizer::executeStep() {
    if (!m_isSearching) {
        return;
    }

    bool continueSearch = false;

    switch (m_currentAlgorithm) {
        case Algorithm::LinearSearch:
            if (m_linearSearcher) {
                continueSearch = m_linearSearcher->step();

                if (!continueSearch) {
                    auto state = m_linearSearcher->getState();
                    int result = m_linearSearcher->getResult();

                    if (state == algorithms::SearchState::Found) {
                        m_statusText = "Found " + std::to_string(m_target) + " at index " + std::to_string(result) + "!";
                    } else {
                        m_statusText = "Value " + std::to_string(m_target) + " not found in array.";
                    }

                    m_isSearching = false;
                    m_isPaused = true;
                } else {
                    int idx = m_linearSearcher->getCurrentIndex();
                    std::ostringstream oss;
                    oss << "Checking index " << idx << ": value = " << m_array[idx];
                    m_statusText = oss.str();
                }
            }
            break;

        case Algorithm::BinarySearch:
            if (m_binarySearcher) {
                continueSearch = m_binarySearcher->step();

                if (!continueSearch) {
                    auto state = m_binarySearcher->getState();
                    int result = m_binarySearcher->getResult();

                    if (state == algorithms::SearchState::Found) {
                        m_statusText = "Found " + std::to_string(m_target) + " at index " + std::to_string(result) + "!";
                    } else {
                        m_statusText = "Value " + std::to_string(m_target) + " not found in array.";
                    }

                    m_isSearching = false;
                    m_isPaused = true;
                } else {
                    int mid = m_binarySearcher->getMidIndex();
                    int left = m_binarySearcher->getLeftBound();
                    int right = m_binarySearcher->getRightBound();

                    std::ostringstream oss;
                    oss << "Checking middle (index " << mid << "): value = " << m_array[mid]
                        << " | Bounds: [" << left << ", " << right << "]";
                    m_statusText = oss.str();
                }
            }
            break;
    }

    // Update visuals and colors
    syncVisuals();
    updateColors();
}

void SearchingVisualizer::updateColors() {
    // Reset all to base color
    for (auto& elem : m_elements) {
        elem.color = colors::semantic::elementBase;
        elem.borderColor = colors::semantic::active;
        elem.isChecked = false;
        elem.isFound = false;
    }

    if (!m_isSearching) {
        return;
    }

    // Update colors based on current algorithm state
    switch (m_currentAlgorithm) {
        case Algorithm::LinearSearch:
            if (m_linearSearcher) {
                int currentIdx = m_linearSearcher->getCurrentIndex();
                auto state = m_linearSearcher->getState();

                // Highlight all checked elements
                for (int i = 0; i < currentIdx && i < static_cast<int>(m_elements.size()); ++i) {
                    m_elements[i].color = colors::semantic::textSecondary;
                    m_elements[i].isChecked = true;
                }

                // Highlight current element being checked
                if (currentIdx >= 0 && currentIdx < static_cast<int>(m_elements.size())) {
                    if (state == algorithms::SearchState::Checking) {
                        m_elements[currentIdx].color = colors::semantic::comparing;
                    } else if (state == algorithms::SearchState::Found) {
                        m_elements[currentIdx].color = colors::semantic::sorted;
                        m_elements[currentIdx].isFound = true;
                    }
                }

                // If found, highlight the found element
                int result = m_linearSearcher->getResult();
                if (result >= 0 && result < static_cast<int>(m_elements.size())) {
                    m_elements[result].color = colors::semantic::sorted;
                    m_elements[result].isFound = true;
                }
            }
            break;

        case Algorithm::BinarySearch:
            if (m_binarySearcher) {
                int left = m_binarySearcher->getLeftBound();
                int right = m_binarySearcher->getRightBound();
                int mid = m_binarySearcher->getMidIndex();
                auto state = m_binarySearcher->getState();

                // Highlight search range
                for (int i = left; i <= right && i < static_cast<int>(m_elements.size()); ++i) {
                    m_elements[i].color = colors::mocha::surface1;
                }

                // Highlight middle element being checked
                if (mid >= 0 && mid < static_cast<int>(m_elements.size())) {
                    if (state == algorithms::SearchState::Checking) {
                        m_elements[mid].color = colors::semantic::comparing;
                    } else if (state == algorithms::SearchState::Found) {
                        m_elements[mid].color = colors::semantic::sorted;
                        m_elements[mid].isFound = true;
                    }
                }

                // If found, highlight the found element
                int result = m_binarySearcher->getResult();
                if (result >= 0 && result < static_cast<int>(m_elements.size())) {
                    m_elements[result].color = colors::semantic::sorted;
                    m_elements[result].isFound = true;
                }
            }
            break;
    }
}

} // namespace dsav
