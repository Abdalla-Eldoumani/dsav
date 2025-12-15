/**
 * @file ui_components.cpp
 * @brief Implementation of reusable UI components
 */

#include "ui_components.hpp"
#include "color_scheme.hpp"
#include <imgui.h>

namespace dsav::ui {

bool ButtonPrimary(const char* label, const ImVec2& size) {
    ImGui::PushStyleColor(ImGuiCol_Button, colors::toImGui(colors::semantic::buttonPrimary));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colors::toImGui(colors::semantic::buttonHover));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colors::toImGui(colors::mocha::sapphire));

    bool clicked = ImGui::Button(label, size);

    ImGui::PopStyleColor(3);

    return clicked;
}

bool ButtonSuccess(const char* label, const ImVec2& size) {
    ImGui::PushStyleColor(ImGuiCol_Button, colors::toImGui(colors::semantic::success));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colors::toImGui(colors::lighten(colors::semantic::success, 0.1f)));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colors::toImGui(colors::darken(colors::semantic::success, 0.9f)));

    bool clicked = ImGui::Button(label, size);

    ImGui::PopStyleColor(3);

    return clicked;
}

bool ButtonDanger(const char* label, const ImVec2& size) {
    ImGui::PushStyleColor(ImGuiCol_Button, colors::toImGui(colors::semantic::danger));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colors::toImGui(colors::lighten(colors::semantic::danger, 0.1f)));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colors::toImGui(colors::darken(colors::semantic::danger, 0.9f)));

    bool clicked = ImGui::Button(label, size);

    ImGui::PopStyleColor(3);

    return clicked;
}

void PlaybackControls(bool isPaused,
                      std::function<void()> onPlay,
                      std::function<void()> onPause,
                      std::function<void()> onStep,
                      std::function<void()> onReset) {
    ImGui::BeginGroup();

    // Reset button
    if (ImGui::Button("⏮##reset")) {
        if (onReset) onReset();
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Reset to beginning");
    }

    ImGui::SameLine();

    // Play/Pause button
    if (isPaused) {
        if (ButtonSuccess("▶##play", ImVec2(50, 0))) {
            if (onPlay) onPlay();
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Play animation");
        }
    } else {
        if (ImGui::Button("⏸##pause", ImVec2(50, 0))) {
            if (onPause) onPause();
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Pause animation");
        }
    }

    ImGui::SameLine();

    // Step button
    if (ImGui::Button("⏩##step")) {
        if (onStep) onStep();
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Step forward one operation");
    }

    ImGui::EndGroup();
}

bool SpeedSlider(float& speed, float minSpeed, float maxSpeed) {
    ImGui::Text("Speed:");
    ImGui::SameLine();

    ImGui::PushItemWidth(200.0f);
    bool changed = ImGui::SliderFloat("##speed", &speed, minSpeed, maxSpeed, "%.1fx");
    ImGui::PopItemWidth();

    return changed;
}

void StatusText(const char* text, const char* type) {
    glm::vec4 color = colors::semantic::textPrimary;

    // Choose color based on type
    if (strcmp(type, "success") == 0) {
        color = colors::semantic::success;
    } else if (strcmp(type, "warning") == 0) {
        color = colors::semantic::warning;
    } else if (strcmp(type, "error") == 0) {
        color = colors::semantic::danger;
    } else if (strcmp(type, "info") == 0) {
        color = colors::semantic::info;
    }

    ImGui::TextColored(colors::toImGui(color), "%s", text);
}

void Tooltip(const char* text) {
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(text);
        ImGui::EndTooltip();
    }
}

bool BeginPanel(const char* label) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, colors::toImGui(colors::semantic::panel));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);

    bool result = ImGui::BeginChild(label, ImVec2(0, 0), true);

    return result;
}

void EndPanel() {
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void SeparatorText(const char* label) {
    ImGui::Separator();
    ImGui::TextColored(colors::toImGui(colors::semantic::textSecondary), "%s", label);
    ImGui::Separator();
}

} // namespace dsav::ui
