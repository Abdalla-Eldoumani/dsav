/**
 * @file sorting_visualizer.cpp
 * @brief Implementation of sorting algorithm visualizer
 */

#include "visualizers/sorting_visualizer.hpp"
#include "color_scheme.hpp"
#include "animation.hpp"
#include <algorithm>
#include <random>
#include <sstream>
#include <cmath>
#include <iomanip>

namespace dsav {

SortingVisualizer::SortingVisualizer() {
    // Initialize with a random array
    randomizeArray();
    syncVisuals();
    m_statusText = "Ready to sort. Click 'Start Sort' or 'Step' to begin.";
}

void SortingVisualizer::update(float deltaTime) {
    // Update animations
    m_animator.update(deltaTime);

    // Auto-step when playing
    if (!m_isPaused && m_isSorting) {
        m_timeSinceLastStep += deltaTime * m_speed;

        // Convert delay from ms to seconds
        float delaySeconds = m_stepDelay / 1000.0f;

        if (m_timeSinceLastStep >= delaySeconds) {
            m_timeSinceLastStep = 0.0f;
            executeStep();
        }
    }
}

void SortingVisualizer::renderVisualization() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();

    // Ensure minimum canvas size
    if (canvasSize.x < 50.0f) canvasSize.x = 800.0f;
    if (canvasSize.y < 50.0f) canvasSize.y = 600.0f;

    // Draw canvas background
    drawList->AddRectFilled(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::mocha::base))
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

    // Draw grid lines (with zoom and offset)
    ImU32 gridColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
    for (int i = 0; i <= 10; ++i) {
        float y = (canvasSize.y / 10.0f) * i * m_zoomLevel;
        drawList->AddLine(
            ImVec2(canvasPos.x, canvasPos.y + y + verticalOffset),
            ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + y + verticalOffset),
            gridColor,
            1.0f
        );
    }

    // Draw elements as vertical bars (with zoom and offset)
    for (size_t i = 0; i < m_elements.size(); ++i) {
        const auto& elem = m_elements[i];

        // Calculate bar height based on value (scaled)
        float barHeight = elem.value * ELEMENT_HEIGHT_SCALE * m_zoomLevel;
        float scaledWidth = ELEMENT_WIDTH * m_zoomLevel;
        float scaledX = elem.position.x * m_zoomLevel;
        float scaledBaseY = BASE_Y * m_zoomLevel;

        // Draw bar (from bottom up)
        ImVec2 topLeft = ImVec2(
            canvasPos.x + scaledX + horizontalOffset,
            canvasPos.y + scaledBaseY - barHeight + verticalOffset
        );
        ImVec2 bottomRight = ImVec2(
            canvasPos.x + scaledX + scaledWidth + horizontalOffset,
            canvasPos.y + scaledBaseY + verticalOffset
        );

        // Draw filled bar
        drawList->AddRectFilled(
            topLeft,
            bottomRight,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(elem.color)),
            4.0f * m_zoomLevel
        );

        // Draw border
        drawList->AddRect(
            topLeft,
            bottomRight,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(elem.borderColor)),
            4.0f * m_zoomLevel,
            0,
            2.0f
        );

        // Draw value label on top of bar
        ImVec2 labelPos = ImVec2(
            topLeft.x + (scaledWidth - ImGui::CalcTextSize(elem.label.c_str()).x) / 2.0f,
            topLeft.y - 20.0f * m_zoomLevel
        );
        drawList->AddText(
            labelPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::semantic::textPrimary)),
            elem.label.c_str()
        );

        // Draw index label at bottom
        std::string indexLabel = "[" + std::to_string(i) + "]";
        ImVec2 indexPos = ImVec2(
            topLeft.x + (scaledWidth - ImGui::CalcTextSize(indexLabel.c_str()).x) / 2.0f,
            bottomRight.y + 5.0f * m_zoomLevel
        );
        drawList->AddText(
            indexPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::semantic::textSecondary)),
            indexLabel.c_str()
        );
    }

    // Draw algorithm info
    std::string algoName;
    switch (m_currentAlgorithm) {
        case Algorithm::BubbleSort:    algoName = "Bubble Sort"; break;
        case Algorithm::SelectionSort: algoName = "Selection Sort"; break;
        case Algorithm::InsertionSort: algoName = "Insertion Sort"; break;
        case Algorithm::MergeSort:     algoName = "Merge Sort"; break;
        case Algorithm::QuickSort:     algoName = "Quick Sort"; break;
    }

    ImVec2 algoTextPos = ImVec2(canvasPos.x + 10.0f, canvasPos.y + 10.0f);
    drawList->AddText(
        algoTextPos,
        ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::semantic::active)),
        algoName.c_str()
    );

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

    // Reserve space for the canvas
    ImGui::Dummy(canvasSize);
}

void SortingVisualizer::renderControls() {
    ImGui::Begin("Sorting Controls");

    // Algorithm selection
    ImGui::Text("Algorithm:");
    static const char* algorithmNames[] = {
        "Bubble Sort", "Selection Sort", "Insertion Sort", "Merge Sort", "Quick Sort"
    };
    int currentAlgo = static_cast<int>(m_currentAlgorithm);
    if (ImGui::Combo("##Algorithm", &currentAlgo, algorithmNames, 5)) {
        m_currentAlgorithm = static_cast<Algorithm>(currentAlgo);
        reset();
    }

    ImGui::Separator();

    // Array controls
    ImGui::Text("Array Configuration:");
    if (ImGui::SliderInt("Array Size", &m_arraySize, 5, MAX_ARRAY_SIZE)) {
        if (!m_isSorting) {
            randomizeArray();
        }
    }

    if (ImGui::Button("Randomize Array", ImVec2(-1, 0))) {
        randomizeArray();
    }

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

    if (ImGui::Button("Start Sort")) {
        startSort();
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
    ImGui::Text("State: %s", m_isSorting ? "Sorting" : "Idle");

    ImGui::End();
}

void SortingVisualizer::play() {
    m_isPaused = false;
    if (!m_isSorting) {
        startSort();
    }
    m_statusText = "Playing...";
}

void SortingVisualizer::pause() {
    m_isPaused = true;
    m_statusText = "Paused";
}

void SortingVisualizer::step() {
    if (!m_isSorting) {
        startSort();
    }
    executeStep();
}

void SortingVisualizer::reset() {
    m_isPaused = true;
    m_isSorting = false;
    m_timeSinceLastStep = 0.0f;
    m_animator.clear();

    // Reset algorithm steppers
    m_bubbleSorter.reset();
    m_selectionSorter.reset();
    m_insertionSorter.reset();
    m_mergeSorter.reset();
    m_quickSorter.reset();

    // Regenerate random array
    randomizeArray();

    m_statusText = "Reset complete. Ready to sort.";
}

void SortingVisualizer::setSpeed(float speed) {
    m_speed = std::clamp(speed, 0.1f, 5.0f);
    m_animator.setSpeedMultiplier(m_speed);
}

std::string SortingVisualizer::getStatusText() const {
    return m_statusText;
}

bool SortingVisualizer::isAnimating() const {
    return m_animator.hasAnimations();
}

bool SortingVisualizer::isPaused() const {
    return m_isPaused;
}

void SortingVisualizer::startSort() {
    m_isSorting = true;
    m_isPaused = false;
    m_timeSinceLastStep = 0.0f;

    // Create appropriate stepper based on selected algorithm
    switch (m_currentAlgorithm) {
        case Algorithm::BubbleSort:
            m_bubbleSorter = std::make_unique<algorithms::BubbleSortStepper>(m_array);
            m_statusText = "Starting Bubble Sort...";
            break;
        case Algorithm::SelectionSort:
            m_selectionSorter = std::make_unique<algorithms::SelectionSortStepper>(m_array);
            m_statusText = "Starting Selection Sort...";
            break;
        case Algorithm::InsertionSort:
            m_insertionSorter = std::make_unique<algorithms::InsertionSortStepper>(m_array);
            m_statusText = "Starting Insertion Sort...";
            break;
        case Algorithm::MergeSort:
            m_mergeSorter = std::make_unique<algorithms::MergeSortStepper>(m_array);
            m_statusText = "Starting Merge Sort...";
            break;
        case Algorithm::QuickSort:
            m_quickSorter = std::make_unique<algorithms::QuickSortStepper>(m_array);
            m_statusText = "Starting Quick Sort...";
            break;
    }

    syncVisuals();
}

void SortingVisualizer::randomizeArray() {
    m_array.clear();
    m_array.resize(m_arraySize);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(10, MAX_VALUE);

    for (int& val : m_array) {
        val = dist(gen);
    }

    // Reset sorting state
    m_isSorting = false;
    m_isPaused = true;

    syncVisuals();
}

void SortingVisualizer::setArray(const std::vector<int>& arr) {
    m_array = arr;
    m_arraySize = static_cast<int>(arr.size());
    m_isSorting = false;
    m_isPaused = true;
    syncVisuals();
}

void SortingVisualizer::syncVisuals() {
    m_elements.clear();
    m_elements.reserve(m_array.size());

    for (size_t i = 0; i < m_array.size(); ++i) {
        VisualSortElement elem;
        elem.position = calculatePosition(i);
        elem.size = glm::vec2(ELEMENT_WIDTH, m_array[i] * ELEMENT_HEIGHT_SCALE);
        elem.color = colors::semantic::elementBase;
        elem.borderColor = colors::semantic::active;
        elem.label = std::to_string(m_array[i]);
        elem.value = m_array[i];
        elem.isSorted = false;

        m_elements.push_back(elem);
    }

    updateColors();
}

glm::vec2 SortingVisualizer::calculatePosition(size_t index) const {
    float x = START_X + index * (ELEMENT_WIDTH + ELEMENT_SPACING);
    float y = BASE_Y;  // All bars share same baseline
    return glm::vec2(x, y);
}

void SortingVisualizer::executeStep() {
    if (!m_isSorting) {
        return;
    }

    bool continueSort = false;

    switch (m_currentAlgorithm) {
        case Algorithm::BubbleSort:
            if (m_bubbleSorter) {
                continueSort = m_bubbleSorter->step();
                if (!continueSort) {
                    m_statusText = "Bubble Sort complete!";
                    m_isSorting = false;
                    m_isPaused = true;
                } else {
                    auto state = m_bubbleSorter->getState();
                    int i = m_bubbleSorter->getIndexI();
                    int j = m_bubbleSorter->getIndexJ();

                    if (state == algorithms::SortState::Comparing) {
                        std::ostringstream oss;
                        oss << "Comparing: arr[" << j << "]=" << m_array[j]
                            << " and arr[" << (j+1) << "]=" << m_array[j+1];
                        m_statusText = oss.str();
                    } else if (state == algorithms::SortState::Swapping) {
                        std::ostringstream oss;
                        oss << "Swapping: arr[" << j << "] ↔ arr[" << (j+1) << "]";
                        m_statusText = oss.str();
                    }
                }
            }
            break;

        case Algorithm::SelectionSort:
            if (m_selectionSorter) {
                continueSort = m_selectionSorter->step();
                if (!continueSort) {
                    m_statusText = "Selection Sort complete!";
                    m_isSorting = false;
                    m_isPaused = true;
                } else {
                    auto state = m_selectionSorter->getState();
                    int current = m_selectionSorter->getCurrentIndex();
                    int minIdx = m_selectionSorter->getMinIndex();

                    if (state == algorithms::SortState::Comparing) {
                        std::ostringstream oss;
                        oss << "Finding minimum in unsorted portion. Current min index: " << minIdx;
                        m_statusText = oss.str();
                    } else if (state == algorithms::SortState::Swapping) {
                        std::ostringstream oss;
                        oss << "Swapping minimum to position " << current;
                        m_statusText = oss.str();
                    }
                }
            }
            break;

        case Algorithm::InsertionSort:
            if (m_insertionSorter) {
                continueSort = m_insertionSorter->step();
                if (!continueSort) {
                    m_statusText = "Insertion Sort complete!";
                    m_isSorting = false;
                    m_isPaused = true;
                } else {
                    auto state = m_insertionSorter->getState();
                    int current = m_insertionSorter->getCurrentIndex();

                    if (state == algorithms::SortState::Swapping) {
                        std::ostringstream oss;
                        oss << "Inserting element at index " << current << " into sorted portion";
                        m_statusText = oss.str();
                    }
                }
            }
            break;

        case Algorithm::MergeSort:
            if (m_mergeSorter) {
                continueSort = m_mergeSorter->step();
                if (!continueSort) {
                    m_statusText = "Merge Sort complete!";
                    m_isSorting = false;
                    m_isPaused = true;
                } else {
                    int left = m_mergeSorter->getLeftIndex();
                    int mid = m_mergeSorter->getMidIndex();
                    int right = m_mergeSorter->getRightIndex();
                    std::ostringstream oss;
                    oss << "Merging [" << left << ".." << mid << "] (yellow) with ["
                        << (mid + 1) << ".." << right << "] (orange)";
                    m_statusText = oss.str();
                }
            }
            break;

        case Algorithm::QuickSort:
            if (m_quickSorter) {
                continueSort = m_quickSorter->step();
                if (!continueSort) {
                    m_statusText = "Quick Sort complete!";
                    m_isSorting = false;
                    m_isPaused = true;
                } else {
                    int pivot = m_quickSorter->getPivotIndex();
                    std::ostringstream oss;
                    oss << "Partitioning around pivot at index " << pivot;
                    m_statusText = oss.str();
                }
            }
            break;
    }

    // Update visuals and colors
    syncVisuals();
    updateColors();
}

void SortingVisualizer::updateColors() {
    // Reset all to base color
    for (auto& elem : m_elements) {
        elem.color = colors::semantic::elementBase;
        elem.borderColor = colors::semantic::active;
        elem.isSorted = false;
    }

    if (!m_isSorting) {
        return;
    }

    // Update colors based on current algorithm state
    switch (m_currentAlgorithm) {
        case Algorithm::BubbleSort:
            if (m_bubbleSorter) {
                int i = m_bubbleSorter->getIndexI();
                int j = m_bubbleSorter->getIndexJ();
                auto state = m_bubbleSorter->getState();

                // Highlight elements being compared/swapped
                if (j >= 0 && j < static_cast<int>(m_elements.size())) {
                    if (state == algorithms::SortState::Comparing) {
                        m_elements[j].color = colors::semantic::comparing;
                        if (j + 1 < static_cast<int>(m_elements.size())) {
                            m_elements[j + 1].color = colors::semantic::comparing;
                        }
                    } else if (state == algorithms::SortState::Swapping) {
                        m_elements[j].color = colors::semantic::swapping;
                        if (j + 1 < static_cast<int>(m_elements.size())) {
                            m_elements[j + 1].color = colors::semantic::swapping;
                        }
                    }
                }

                // Highlight sorted elements (at the end)
                const auto& sortedIndices = m_bubbleSorter->getSortedIndices();
                for (int idx : sortedIndices) {
                    if (idx >= 0 && idx < static_cast<int>(m_elements.size())) {
                        m_elements[idx].color = colors::semantic::sorted;
                        m_elements[idx].isSorted = true;
                    }
                }
            }
            break;

        case Algorithm::SelectionSort:
            if (m_selectionSorter) {
                int current = m_selectionSorter->getCurrentIndex();
                int minIdx = m_selectionSorter->getMinIndex();
                int compareIdx = m_selectionSorter->getCompareIndex();

                // Highlight current minimum
                if (minIdx >= 0 && minIdx < static_cast<int>(m_elements.size())) {
                    m_elements[minIdx].color = colors::semantic::highlight;
                }

                // Highlight element being compared
                if (compareIdx >= 0 && compareIdx < static_cast<int>(m_elements.size())) {
                    m_elements[compareIdx].color = colors::semantic::comparing;
                }

                // Highlight sorted elements
                const auto& sortedIndices = m_selectionSorter->getSortedIndices();
                for (int idx : sortedIndices) {
                    if (idx >= 0 && idx < static_cast<int>(m_elements.size())) {
                        m_elements[idx].color = colors::semantic::sorted;
                        m_elements[idx].isSorted = true;
                    }
                }
            }
            break;

        case Algorithm::InsertionSort:
            if (m_insertionSorter) {
                int current = m_insertionSorter->getCurrentIndex();
                int compare = m_insertionSorter->getCompareIndex();

                // Highlight element being inserted
                if (current >= 0 && current < static_cast<int>(m_elements.size())) {
                    m_elements[current].color = colors::semantic::comparing;
                }

                // Highlight comparison position
                if (compare >= 0 && compare < static_cast<int>(m_elements.size())) {
                    m_elements[compare].color = colors::semantic::swapping;
                }

                // Highlight sorted elements
                const auto& sortedIndices = m_insertionSorter->getSortedIndices();
                for (int idx : sortedIndices) {
                    if (idx >= 0 && idx < static_cast<int>(m_elements.size())) {
                        m_elements[idx].color = colors::semantic::sorted;
                        m_elements[idx].isSorted = true;
                    }
                }
            }
            break;

        case Algorithm::MergeSort:
            if (m_mergeSorter) {
                int left = m_mergeSorter->getLeftIndex();
                int mid = m_mergeSorter->getMidIndex();
                int right = m_mergeSorter->getRightIndex();

                // Highlight left subarray (being merged) - Yellow
                for (int i = left; i >= 0 && i <= mid && i < static_cast<int>(m_elements.size()); ++i) {
                    m_elements[i].color = colors::semantic::comparing;
                }

                // Highlight right subarray (being merged) - Peach/Orange
                for (int i = mid + 1; i >= 0 && i <= right && i < static_cast<int>(m_elements.size()); ++i) {
                    m_elements[i].color = colors::semantic::swapping;
                }

                // Highlight sorted elements - Green
                const auto& sortedIndices = m_mergeSorter->getSortedIndices();
                for (int idx : sortedIndices) {
                    if (idx >= 0 && idx < static_cast<int>(m_elements.size())) {
                        m_elements[idx].color = colors::semantic::sorted;
                        m_elements[idx].isSorted = true;
                    }
                }
            }
            break;

        case Algorithm::QuickSort:
            if (m_quickSorter) {
                int pivot = m_quickSorter->getPivotIndex();
                int left = m_quickSorter->getLeftIndex();
                int right = m_quickSorter->getRightIndex();

                // Highlight pivot
                if (pivot >= 0 && pivot < static_cast<int>(m_elements.size())) {
                    m_elements[pivot].color = colors::semantic::highlight;
                }

                // Highlight elements being compared
                if (left >= 0 && left < static_cast<int>(m_elements.size())) {
                    m_elements[left].color = colors::semantic::comparing;
                }
                if (right >= 0 && right < static_cast<int>(m_elements.size()) && right != pivot) {
                    m_elements[right].color = colors::semantic::comparing;
                }

                // Highlight sorted elements
                const auto& sortedIndices = m_quickSorter->getSortedIndices();
                for (int idx : sortedIndices) {
                    if (idx >= 0 && idx < static_cast<int>(m_elements.size())) {
                        m_elements[idx].color = colors::semantic::sorted;
                        m_elements[idx].isSorted = true;
                    }
                }
            }
            break;
    }
}

} // namespace dsav
