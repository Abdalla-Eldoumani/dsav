/**
 * @file color_scheme.hpp
 * @brief Catppuccin Mocha color palette for DSAV
 *
 * This file defines the complete color scheme used throughout the DSAV application.
 * We use the Catppuccin Mocha palette - a modern, accessible dark theme that's easy
 * on the eyes for extended use.
 *
 * Color Reference: https://github.com/catppuccin/catppuccin
 */

#pragma once

#include <glm/glm.hpp>

// Forward declare ImVec4 to avoid including imgui.h here
struct ImVec4;

namespace dsav::colors {

/**
 * @brief Catppuccin Mocha palette - Base colors
 *
 * These are the foundational colors from the Catppuccin Mocha theme.
 * All colors are defined as RGBA with values normalized to [0.0, 1.0].
 */
namespace mocha {
    // Base surface colors
    constexpr glm::vec4 base      {0.118f, 0.118f, 0.180f, 1.0f};  // #1E1E2E - Darkest background
    constexpr glm::vec4 mantle    {0.110f, 0.106f, 0.157f, 1.0f};  // #181825 - Slightly darker than base
    constexpr glm::vec4 crust     {0.071f, 0.067f, 0.118f, 1.0f};  // #11111B - Darkest surface

    // Surface variations
    constexpr glm::vec4 surface0  {0.192f, 0.196f, 0.267f, 1.0f};  // #313244 - Panels, cards
    constexpr glm::vec4 surface1  {0.267f, 0.278f, 0.353f, 1.0f};  // #45475A - Raised surfaces
    constexpr glm::vec4 surface2  {0.357f, 0.373f, 0.459f, 1.0f};  // #585B70 - Higher surfaces

    // Overlay colors
    constexpr glm::vec4 overlay0  {0.431f, 0.447f, 0.545f, 1.0f};  // #6C7086 - Disabled elements
    constexpr glm::vec4 overlay1  {0.502f, 0.518f, 0.612f, 1.0f};  // #7F849C - Subtle text
    constexpr glm::vec4 overlay2  {0.573f, 0.592f, 0.678f, 1.0f};  // #9399B2 - Muted elements

    // Text colors
    constexpr glm::vec4 subtext0  {0.651f, 0.675f, 0.757f, 1.0f};  // #A6ADC8 - Secondary text
    constexpr glm::vec4 subtext1  {0.725f, 0.753f, 0.835f, 1.0f};  // #BAC2DE - Dimmed text
    constexpr glm::vec4 text      {0.804f, 0.839f, 0.957f, 1.0f};  // #CDD6F4 - Primary text

    // Accent colors - Used for semantic meanings in visualizations
    constexpr glm::vec4 rosewater {0.961f, 0.878f, 0.851f, 1.0f};  // #F5E0DC
    constexpr glm::vec4 flamingo  {0.953f, 0.800f, 0.816f, 1.0f};  // #F2CDCD
    constexpr glm::vec4 pink      {0.961f, 0.753f, 0.882f, 1.0f};  // #F5C2E7
    constexpr glm::vec4 mauve     {0.796f, 0.651f, 0.969f, 1.0f};  // #CBA6F7 - Special/unique
    constexpr glm::vec4 red       {0.953f, 0.545f, 0.659f, 1.0f};  // #F38BA8 - Error/removed
    constexpr glm::vec4 maroon    {0.922f, 0.624f, 0.651f, 1.0f};  // #EBA0AC
    constexpr glm::vec4 peach     {0.980f, 0.702f, 0.529f, 1.0f};  // #FAB387 - Swapping/warning
    constexpr glm::vec4 yellow    {0.976f, 0.886f, 0.686f, 1.0f};  // #F9E2AF - Comparing/attention
    constexpr glm::vec4 green     {0.651f, 0.890f, 0.631f, 1.0f};  // #A6E3A1 - Success/sorted
    constexpr glm::vec4 teal      {0.580f, 0.886f, 0.843f, 1.0f};  // #94E2D5 - Highlight/info
    constexpr glm::vec4 sky       {0.537f, 0.878f, 0.988f, 1.0f};  // #89DCEB
    constexpr glm::vec4 sapphire  {0.455f, 0.827f, 0.914f, 1.0f};  // #74C7E9
    constexpr glm::vec4 blue      {0.537f, 0.706f, 0.980f, 1.0f};  // #89B4FA - Primary/active
    constexpr glm::vec4 lavender  {0.710f, 0.733f, 1.000f, 1.0f};  // #B4BBFF
}

/**
 * @brief Semantic color aliases for data structure visualization
 *
 * These names describe the PURPOSE of the color in the visualization,
 * making the code more readable and maintainable.
 */
namespace semantic {
    // UI Background colors
    constexpr auto background     = mocha::base;        // Main application background
    constexpr auto backgroundAlt  = mocha::mantle;      // Alternative background
    constexpr auto panel          = mocha::surface0;    // Panels and containers
    constexpr auto panelRaised    = mocha::surface1;    // Raised panels (modals, etc.)
    constexpr auto border         = mocha::surface2;    // Border colors

    // Text colors
    constexpr auto textPrimary    = mocha::text;        // Main text
    constexpr auto textSecondary  = mocha::subtext0;    // Secondary, less important text
    constexpr auto textDim        = mocha::overlay0;    // Dimmed, disabled text

    // Element base colors (for data structure elements at rest)
    constexpr auto elementBase    = mocha::surface1;    // Default element color
    constexpr auto elementBorder  = mocha::overlay2;    // Element border

    // Visualization state colors
    constexpr auto active         = mocha::blue;        // Currently active/selected element
    constexpr auto sorted         = mocha::green;       // Successfully sorted/completed
    constexpr auto comparing      = mocha::yellow;      // Elements being compared
    constexpr auto swapping       = mocha::peach;       // Elements being swapped
    constexpr auto error          = mocha::red;         // Error state or removed elements
    constexpr auto highlight      = mocha::teal;        // Highlighted for attention
    constexpr auto special        = mocha::mauve;       // Special markers (e.g., pivot in quicksort)

    // Tree/Graph specific colors
    constexpr auto nodeDefault    = mocha::surface1;    // Default node color
    constexpr auto nodeVisited    = mocha::blue;        // Visited node
    constexpr auto nodeCurrent    = mocha::yellow;      // Current node being processed
    constexpr auto edgeDefault    = mocha::overlay1;    // Default edge color
    constexpr auto edgeActive     = mocha::blue;        // Active edge

    // Stack/Queue specific colors
    constexpr auto topElement     = mocha::blue;        // Top of stack / front of queue
    constexpr auto bottomElement  = mocha::overlay2;    // Bottom element

    // UI accent colors
    constexpr auto buttonPrimary  = mocha::blue;        // Primary button
    constexpr auto buttonHover    = mocha::sky;         // Button hover state
    constexpr auto success        = mocha::green;       // Success messages
    constexpr auto warning        = mocha::peach;       // Warning messages
    constexpr auto danger         = mocha::red;         // Danger/error messages
    constexpr auto info           = mocha::teal;        // Info messages
}

/**
 * @brief Convert glm::vec4 color to ImGui's ImVec4 format
 *
 * @param c Color in glm::vec4 format (RGBA, 0.0-1.0)
 * @return ImVec4 color for use with ImGui functions
 */
ImVec4 toImGui(const glm::vec4& c);

/**
 * @brief Convert ImGui's ImVec4 to glm::vec4 format
 *
 * @param c Color in ImVec4 format
 * @return glm::vec4 color
 */
glm::vec4 fromImGui(const ImVec4& c);

/**
 * @brief Apply the Catppuccin Mocha theme to Dear ImGui
 *
 * This function sets up ImGui's style and colors to match the
 * Catppuccin Mocha palette, ensuring a consistent look throughout
 * the application.
 *
 * Call this once after ImGui initialization.
 */
void applyImGuiTheme();

/**
 * @brief Interpolate between two colors
 *
 * @param a First color
 * @param b Second color
 * @param t Interpolation factor (0.0 = all a, 1.0 = all b)
 * @return Interpolated color
 */
glm::vec4 lerp(const glm::vec4& a, const glm::vec4& b, float t);

/**
 * @brief Darken a color by a given factor
 *
 * @param color Original color
 * @param factor Darken factor (0.0 = black, 1.0 = unchanged)
 * @return Darkened color
 */
glm::vec4 darken(const glm::vec4& color, float factor);

/**
 * @brief Lighten a color by a given factor
 *
 * @param color Original color
 * @param factor Lighten factor (0.0 = unchanged, 1.0 = white)
 * @return Lightened color
 */
glm::vec4 lighten(const glm::vec4& color, float factor);

/**
 * @brief Adjust alpha transparency of a color
 *
 * @param color Original color
 * @param alpha New alpha value (0.0 = fully transparent, 1.0 = fully opaque)
 * @return Color with adjusted alpha
 */
glm::vec4 withAlpha(const glm::vec4& color, float alpha);

} // namespace dsav::colors
