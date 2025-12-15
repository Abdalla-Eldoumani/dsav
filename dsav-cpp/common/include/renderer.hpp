/**
 * @file renderer.hpp
 * @brief OpenGL rendering utilities for DSAV
 *
 * Provides helper functions and classes for rendering visual elements
 * using OpenGL and ImGui's DrawList.
 */

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <imgui.h>  // Need full definition for default arguments

namespace dsav {

/**
 * @brief Represents a single visual element (array cell, node, etc.)
 */
struct VisualElement {
    glm::vec2 position{0.0f, 0.0f};           // Position in screen space
    glm::vec2 size{80.0f, 60.0f};             // Size of the element
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};  // Fill color
    glm::vec4 borderColor{1.0f, 1.0f, 1.0f, 1.0f};  // Border color

    float borderWidth = 2.0f;                 // Border thickness
    float cornerRadius = 8.0f;                // Rounded corner radius
    float scale = 1.0f;                       // Scale multiplier (for animations)
    float rotation = 0.0f;                    // Rotation in radians (future use)

    std::string label;                        // Main value to display
    std::string sublabel;                     // Index or additional info

    bool isHighlighted = false;               // Highlight state
    bool isAnimating = false;                 // Currently animating
};

/**
 * @brief Render a single visual element using ImGui's DrawList
 *
 * @param drawList ImGui draw list to render into
 * @param elem Element to render
 * @param offset Offset to apply to position (for scrolling/panning)
 */
void renderElement(ImDrawList* drawList, const VisualElement& elem, const ImVec2& offset = ImVec2(0, 0));

/**
 * @brief Render a line/edge connecting two points
 *
 * @param drawList ImGui draw list to render into
 * @param start Start position
 * @param end End position
 * @param color Line color
 * @param thickness Line thickness
 */
void renderLine(ImDrawList* drawList, const glm::vec2& start, const glm::vec2& end,
                const glm::vec4& color, float thickness = 2.0f);

/**
 * @brief Render an arrow from start to end
 *
 * @param drawList ImGui draw list to render into
 * @param start Start position
 * @param end End position
 * @param color Arrow color
 * @param thickness Arrow line thickness
 * @param arrowSize Size of arrow head
 */
void renderArrow(ImDrawList* drawList, const glm::vec2& start, const glm::vec2& end,
                 const glm::vec4& color, float thickness = 2.0f, float arrowSize = 10.0f);

} // namespace dsav
