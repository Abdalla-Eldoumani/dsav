/**
 * @file ui_components.hpp
 * @brief Reusable ImGui UI components for DSAV
 *
 * Provides custom widgets and UI helpers to maintain consistent
 * styling and behavior across the application.
 */

#pragma once

#include <string>
#include <functional>
#include <vector>
#include <imgui.h>  // Need full definition for ImVec2

namespace dsav::ui {

/**
 * @brief Render a styled button with primary accent color
 *
 * @param label Button text
 * @param size Button size (optional, auto-size if not specified)
 * @return true if button was clicked
 */
bool ButtonPrimary(const char* label, const ImVec2& size = ImVec2(0, 0));

/**
 * @brief Render a styled button with success (green) color
 *
 * @param label Button text
 * @param size Button size (optional)
 * @return true if button was clicked
 */
bool ButtonSuccess(const char* label, const ImVec2& size = ImVec2(0, 0));

/**
 * @brief Render a styled button with danger (red) color
 *
 * @param label Button text
 * @param size Button size (optional)
 * @return true if button was clicked
 */
bool ButtonDanger(const char* label, const ImVec2& size = ImVec2(0, 0));

/**
 * @brief Render playback control buttons (play, pause, step, reset)
 *
 * @param isPaused Current paused state
 * @param onPlay Callback for play button
 * @param onPause Callback for pause button
 * @param onStep Callback for step forward button
 * @param onReset Callback for reset button
 */
void PlaybackControls(bool isPaused,
                      std::function<void()> onPlay,
                      std::function<void()> onPause,
                      std::function<void()> onStep,
                      std::function<void()> onReset);

/**
 * @brief Render a speed slider control
 *
 * @param speed Current speed value (will be modified)
 * @param minSpeed Minimum speed
 * @param maxSpeed Maximum speed
 * @return true if speed was changed
 */
bool SpeedSlider(float& speed, float minSpeed = 0.1f, float maxSpeed = 5.0f);

/**
 * @brief Render a status text with icon/color
 *
 * @param text Status message
 * @param type Status type ("info", "success", "warning", "error")
 */
void StatusText(const char* text, const char* type = "info");

/**
 * @brief Render a tooltip when hovering over the previous item
 *
 * @param text Tooltip text
 */
void Tooltip(const char* text);

/**
 * @brief Begin a styled panel/group
 *
 * @param label Panel label/ID
 * @return true to continue rendering panel contents
 */
bool BeginPanel(const char* label);

/**
 * @brief End a styled panel/group
 */
void EndPanel();

/**
 * @brief Render a horizontal separator with label
 *
 * @param label Text to display in separator
 */
void SeparatorText(const char* label);

} // namespace dsav::ui
