/**
 * @file color_scheme.cpp
 * @brief Implementation of color scheme utilities and ImGui theming
 */

#include "color_scheme.hpp"
#include <imgui.h>
#include <algorithm>

namespace dsav::colors {

ImVec4 toImGui(const glm::vec4& c) {
    return ImVec4(c.r, c.g, c.b, c.a);
}

glm::vec4 fromImGui(const ImVec4& c) {
    return glm::vec4(c.x, c.y, c.z, c.w);
}

void applyImGuiTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // ===== Color Palette =====

    // Window and panels
    colors[ImGuiCol_WindowBg]           = toImGui(mocha::base);
    colors[ImGuiCol_ChildBg]            = toImGui(mocha::mantle);
    colors[ImGuiCol_PopupBg]            = toImGui(mocha::surface0);
    colors[ImGuiCol_Border]             = toImGui(mocha::surface1);
    colors[ImGuiCol_BorderShadow]       = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);  // Transparent

    // Frame backgrounds (inputs, combo boxes, etc.)
    colors[ImGuiCol_FrameBg]            = toImGui(mocha::surface0);
    colors[ImGuiCol_FrameBgHovered]     = toImGui(mocha::surface1);
    colors[ImGuiCol_FrameBgActive]      = toImGui(mocha::surface2);

    // Title bar
    colors[ImGuiCol_TitleBg]            = toImGui(mocha::mantle);
    colors[ImGuiCol_TitleBgActive]      = toImGui(mocha::crust);
    colors[ImGuiCol_TitleBgCollapsed]   = toImGui(mocha::mantle);

    // Menu bar
    colors[ImGuiCol_MenuBarBg]          = toImGui(mocha::surface0);

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg]        = toImGui(mocha::surface0);
    colors[ImGuiCol_ScrollbarGrab]      = toImGui(mocha::surface2);
    colors[ImGuiCol_ScrollbarGrabHovered] = toImGui(mocha::overlay0);
    colors[ImGuiCol_ScrollbarGrabActive]  = toImGui(mocha::overlay1);

    // Checkmark and sliders
    colors[ImGuiCol_CheckMark]          = toImGui(mocha::blue);
    colors[ImGuiCol_SliderGrab]         = toImGui(mocha::blue);
    colors[ImGuiCol_SliderGrabActive]   = toImGui(mocha::sky);

    // Buttons
    colors[ImGuiCol_Button]             = toImGui(mocha::surface1);
    colors[ImGuiCol_ButtonHovered]      = toImGui(mocha::surface2);
    colors[ImGuiCol_ButtonActive]       = toImGui(mocha::overlay0);

    // Headers (collapsing headers, tree nodes, etc.)
    colors[ImGuiCol_Header]             = toImGui(mocha::surface1);
    colors[ImGuiCol_HeaderHovered]      = toImGui(mocha::surface2);
    colors[ImGuiCol_HeaderActive]       = toImGui(mocha::overlay0);

    // Separator
    colors[ImGuiCol_Separator]          = toImGui(mocha::surface2);
    colors[ImGuiCol_SeparatorHovered]   = toImGui(mocha::overlay0);
    colors[ImGuiCol_SeparatorActive]    = toImGui(mocha::overlay1);

    // Resize grip
    colors[ImGuiCol_ResizeGrip]         = toImGui(withAlpha(mocha::blue, 0.25f));
    colors[ImGuiCol_ResizeGripHovered]  = toImGui(withAlpha(mocha::blue, 0.67f));
    colors[ImGuiCol_ResizeGripActive]   = toImGui(mocha::blue);

    // Tabs
    colors[ImGuiCol_Tab]                = toImGui(mocha::surface0);
    colors[ImGuiCol_TabHovered]         = toImGui(mocha::surface2);
    colors[ImGuiCol_TabActive]          = toImGui(mocha::surface1);
    colors[ImGuiCol_TabUnfocused]       = toImGui(mocha::surface0);
    colors[ImGuiCol_TabUnfocusedActive] = toImGui(mocha::surface1);

    // Docking
    colors[ImGuiCol_DockingPreview]     = toImGui(withAlpha(mocha::blue, 0.7f));
    colors[ImGuiCol_DockingEmptyBg]     = toImGui(mocha::base);

    // Table
    colors[ImGuiCol_TableHeaderBg]      = toImGui(mocha::surface0);
    colors[ImGuiCol_TableBorderStrong]  = toImGui(mocha::surface2);
    colors[ImGuiCol_TableBorderLight]   = toImGui(mocha::surface1);
    colors[ImGuiCol_TableRowBg]         = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);  // Transparent
    colors[ImGuiCol_TableRowBgAlt]      = toImGui(withAlpha(mocha::surface0, 0.5f));

    // Text
    colors[ImGuiCol_Text]               = toImGui(mocha::text);
    colors[ImGuiCol_TextDisabled]       = toImGui(mocha::overlay0);
    colors[ImGuiCol_TextSelectedBg]     = toImGui(withAlpha(mocha::blue, 0.35f));

    // Drag and drop
    colors[ImGuiCol_DragDropTarget]     = toImGui(mocha::yellow);

    // Navigation
    colors[ImGuiCol_NavHighlight]       = toImGui(mocha::blue);
    colors[ImGuiCol_NavWindowingHighlight] = toImGui(mocha::blue);
    colors[ImGuiCol_NavWindowingDimBg]  = toImGui(withAlpha(mocha::overlay0, 0.7f));

    // Modal window dim background
    colors[ImGuiCol_ModalWindowDimBg]   = toImGui(withAlpha(mocha::crust, 0.73f));

    // ===== Style Settings =====

    // Window
    style.WindowPadding                 = ImVec2(12.0f, 12.0f);
    style.WindowRounding                = 8.0f;
    style.WindowBorderSize              = 1.0f;
    style.WindowMinSize                 = ImVec2(100.0f, 100.0f);
    style.WindowTitleAlign              = ImVec2(0.5f, 0.5f);  // Center title

    // Child windows
    style.ChildRounding                 = 6.0f;
    style.ChildBorderSize               = 1.0f;

    // Popup
    style.PopupRounding                 = 6.0f;
    style.PopupBorderSize               = 1.0f;

    // Frame (inputs, combo boxes, etc.)
    style.FramePadding                  = ImVec2(8.0f, 4.0f);
    style.FrameRounding                 = 4.0f;
    style.FrameBorderSize               = 0.0f;

    // Items
    style.ItemSpacing                   = ImVec2(12.0f, 8.0f);
    style.ItemInnerSpacing              = ImVec2(8.0f, 6.0f);

    // Indent
    style.IndentSpacing                 = 20.0f;

    // Columns
    style.ColumnsMinSpacing             = 6.0f;

    // Scrollbar
    style.ScrollbarSize                 = 14.0f;
    style.ScrollbarRounding             = 9.0f;

    // Grab (sliders, etc.)
    style.GrabMinSize                   = 12.0f;
    style.GrabRounding                  = 4.0f;

    // Tabs
    style.TabRounding                   = 4.0f;
    style.TabBorderSize                 = 0.0f;

    // Color buttons
    style.ColorButtonPosition           = ImGuiDir_Right;

    // Alignment
    style.ButtonTextAlign               = ImVec2(0.5f, 0.5f);  // Center button text
    style.SelectableTextAlign           = ImVec2(0.0f, 0.5f);  // Left-align selectables

    // Safe area padding (for docking)
    style.DisplaySafeAreaPadding        = ImVec2(4.0f, 4.0f);

    // Anti-aliasing
    style.AntiAliasedLines              = true;
    style.AntiAliasedLinesUseTex        = true;
    style.AntiAliasedFill               = true;

    // Curve tessellation tolerance
    style.CurveTessellationTol          = 1.25f;

    // Circle rendering
    style.CircleTessellationMaxError    = 0.3f;
}

glm::vec4 lerp(const glm::vec4& a, const glm::vec4& b, float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    return glm::vec4(
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        a.a + (b.a - a.a) * t
    );
}

glm::vec4 darken(const glm::vec4& color, float factor) {
    factor = std::clamp(factor, 0.0f, 1.0f);
    return glm::vec4(
        color.r * factor,
        color.g * factor,
        color.b * factor,
        color.a  // Preserve alpha
    );
}

glm::vec4 lighten(const glm::vec4& color, float factor) {
    factor = std::clamp(factor, 0.0f, 1.0f);
    return glm::vec4(
        color.r + (1.0f - color.r) * factor,
        color.g + (1.0f - color.g) * factor,
        color.b + (1.0f - color.b) * factor,
        color.a  // Preserve alpha
    );
}

glm::vec4 withAlpha(const glm::vec4& color, float alpha) {
    return glm::vec4(color.r, color.g, color.b, std::clamp(alpha, 0.0f, 1.0f));
}

} // namespace dsav::colors
