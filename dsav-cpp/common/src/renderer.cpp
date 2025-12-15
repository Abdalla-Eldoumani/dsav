/**
 * @file renderer.cpp
 * @brief Implementation of rendering utilities
 */

#include "renderer.hpp"
#include "color_scheme.hpp"
#include <imgui.h>
#include <cmath>

namespace dsav {

void renderElement(ImDrawList* drawList, const VisualElement& elem, const ImVec2& offset) {
    if (!drawList) return;

    // Calculate screen position with offset and scale
    ImVec2 min = ImVec2(
        elem.position.x * elem.scale + offset.x,
        elem.position.y * elem.scale + offset.y
    );
    ImVec2 max = ImVec2(
        min.x + elem.size.x * elem.scale,
        min.y + elem.size.y * elem.scale
    );

    // Convert colors to ImGui format
    ImU32 fillColor = ImGui::ColorConvertFloat4ToU32(colors::toImGui(elem.color));
    ImU32 borderCol = ImGui::ColorConvertFloat4ToU32(colors::toImGui(elem.borderColor));

    // Draw rounded rectangle background
    drawList->AddRectFilled(min, max, fillColor, elem.cornerRadius);

    // Draw border
    if (elem.borderWidth > 0.0f) {
        drawList->AddRect(min, max, borderCol, elem.cornerRadius, 0, elem.borderWidth);
    }

    // Draw main label (centered)
    if (!elem.label.empty()) {
        ImVec2 textSize = ImGui::CalcTextSize(elem.label.c_str());
        ImVec2 textPos = ImVec2(
            min.x + (max.x - min.x - textSize.x) * 0.5f,
            min.y + (max.y - min.y - textSize.y) * 0.5f
        );
        drawList->AddText(textPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::semantic::textPrimary)),
            elem.label.c_str());
    }

    // Draw sublabel below the element
    if (!elem.sublabel.empty()) {
        ImVec2 subSize = ImGui::CalcTextSize(elem.sublabel.c_str());
        ImVec2 subPos = ImVec2(
            min.x + (max.x - min.x - subSize.x) * 0.5f,
            max.y + 4.0f  // Slightly below the element
        );
        drawList->AddText(subPos,
            ImGui::ColorConvertFloat4ToU32(colors::toImGui(colors::semantic::textSecondary)),
            elem.sublabel.c_str());
    }

    // Optional: Draw highlight glow effect
    if (elem.isHighlighted) {
        float glowRadius = elem.cornerRadius + 4.0f;
        ImVec2 glowMin = ImVec2(min.x - 2.0f, min.y - 2.0f);
        ImVec2 glowMax = ImVec2(max.x + 2.0f, max.y + 2.0f);
        ImU32 glowColor = ImGui::ColorConvertFloat4ToU32(
            colors::toImGui(colors::withAlpha(colors::semantic::highlight, 0.5f))
        );
        drawList->AddRect(glowMin, glowMax, glowColor, glowRadius, 0, 2.0f);
    }
}

void renderLine(ImDrawList* drawList, const glm::vec2& start, const glm::vec2& end,
                const glm::vec4& color, float thickness) {
    if (!drawList) return;

    ImVec2 p1 = ImVec2(start.x, start.y);
    ImVec2 p2 = ImVec2(end.x, end.y);
    ImU32 col = ImGui::ColorConvertFloat4ToU32(colors::toImGui(color));

    drawList->AddLine(p1, p2, col, thickness);
}

void renderArrow(ImDrawList* drawList, const glm::vec2& start, const glm::vec2& end,
                 const glm::vec4& color, float thickness, float arrowSize) {
    if (!drawList) return;

    ImVec2 p1 = ImVec2(start.x, start.y);
    ImVec2 p2 = ImVec2(end.x, end.y);
    ImU32 col = ImGui::ColorConvertFloat4ToU32(colors::toImGui(color));

    // Draw line
    drawList->AddLine(p1, p2, col, thickness);

    // Calculate arrow head
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float length = std::sqrt(dx * dx + dy * dy);

    if (length > 0.0f) {
        // Normalize direction
        dx /= length;
        dy /= length;

        // Perpendicular vector
        float px = -dy;
        float py = dx;

        // Arrow head points
        ImVec2 arrowTip = p2;
        ImVec2 arrowLeft = ImVec2(
            p2.x - dx * arrowSize + px * (arrowSize * 0.5f),
            p2.y - dy * arrowSize + py * (arrowSize * 0.5f)
        );
        ImVec2 arrowRight = ImVec2(
            p2.x - dx * arrowSize - px * (arrowSize * 0.5f),
            p2.y - dy * arrowSize - py * (arrowSize * 0.5f)
        );

        // Draw filled triangle for arrow head
        drawList->AddTriangleFilled(arrowTip, arrowLeft, arrowRight, col);
    }
}

} // namespace dsav
