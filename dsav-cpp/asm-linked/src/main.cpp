/**
 * @file main.cpp
 * @brief Entry point for DSAV Assembly-Linked version
 *
 * Uses ARMv8 assembly for core data structure operations,
 * C++ for visualization, animation, and UI.
 */

#include <iostream>
#include <string>
#include <memory>

// OpenGL and windowing
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Dear ImGui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// DSAV common library
#include "color_scheme.hpp"
#include "renderer.hpp"
#include "animation.hpp"
#include "ui_components.hpp"

// Assembly-linked visualizers
#include "asm_stack_visualizer.hpp"

// GLM for math
#include <glm/glm.hpp>

// ===== Application Configuration =====

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr const char* WINDOW_TITLE = "DSAV - Assembly-Linked Version (ARMv8)";
constexpr const char* GLSL_VERSION = "#version 330 core";

// ===== Forward Declarations =====

static void glfwErrorCallback(int error, const char* description);
static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// ===== Application State =====

struct ApplicationState {
    bool showDemoWindow = false;
    bool showSidebar = true;
    bool showVisualization = true;

    // Current visualizer
    std::unique_ptr<dsav::IVisualizer> currentVisualizer;

    std::string statusMessage = "Ready - Using ARM64 Assembly Backend";
};

// ===== Main Function =====

int main(int argc, char** argv) {
    std::cout << "========================================\n";
    std::cout << "DSAV - Data Structures & Algorithms Visualizer\n";
    std::cout << "Assembly-Linked Version\n";
    std::cout << "ARMv8 AArch64 Assembly + C++ Visualization\n";
    std::cout << "========================================\n\n";

    // ===== 1. Initialize GLFW =====

    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    // Set OpenGL version (3.3 Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Enable MSAA
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    // ===== 2. Initialize GLAD =====

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Print OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n\n";

    // Enable OpenGL features
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ===== 3. Initialize Dear ImGui =====

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup ImGui style
    ImGui::StyleColorsDark();
    dsav::colors::applyImGuiTheme();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    std::cout << "Initialization complete!\n";
    std::cout << "Press ESC to exit\n\n";

    // ===== 4. Application State =====

    ApplicationState appState;

    // Initialize with Assembly Stack visualizer
    appState.currentVisualizer = std::make_unique<dsav::AsmStackVisualizer>();

    // ===== 5. Main Loop =====

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Poll events
        glfwPollEvents();

        // Start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Update visualizer
        if (appState.currentVisualizer) {
            appState.currentVisualizer->update(deltaTime);
        }

        // ===== Setup Dockspace =====

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar
            | ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        // Create dockspace
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        // ===== Menu Bar =====

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Reset", "Ctrl+R")) {
                    if (appState.currentVisualizer) {
                        appState.currentVisualizer->reset();
                    }
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "ESC")) {
                    glfwSetWindowShouldClose(window, true);
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Sidebar", nullptr, &appState.showSidebar);
                ImGui::MenuItem("Visualization", nullptr, &appState.showVisualization);
                ImGui::Separator();
                ImGui::MenuItem("ImGui Demo", nullptr, &appState.showDemoWindow);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About")) {
                    appState.statusMessage = "DSAV v1.0 - Assembly-Linked (ARMv8)";
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();  // DockSpaceWindow

        // ===== Sidebar =====

        if (appState.showSidebar) {
            ImGui::Begin("Control Panel", &appState.showSidebar);

            ImGui::TextColored(dsav::colors::toImGui(dsav::colors::mocha::blue), "DSAV");
            ImGui::Text("Assembly-Linked Version");
            ImGui::TextColored(dsav::colors::toImGui(dsav::colors::mocha::green), "ARMv8 Backend");
            ImGui::Separator();

            // Data Structures Section
            if (ImGui::CollapsingHeader("Data Structures (ASM)", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Stack button
                bool isStackActive = appState.currentVisualizer &&
                                     appState.currentVisualizer->getName() == "Stack (ASM)";
                if (isStackActive) {
                    ImGui::PushStyleColor(ImGuiCol_Button,
                        dsav::colors::toImGui(dsav::colors::semantic::active));
                }
                if (ImGui::Button("Stack", ImVec2(-1, 0))) {
                    appState.currentVisualizer = std::make_unique<dsav::AsmStackVisualizer>();
                    appState.statusMessage = "Stack (Assembly) selected";
                }
                if (isStackActive) {
                    ImGui::PopStyleColor();
                }

                // TODO: Add more visualizers
                ImGui::BeginDisabled();
                ImGui::Button("Queue (Coming Soon)", ImVec2(-1, 0));
                ImGui::Button("Linked List (Coming Soon)", ImVec2(-1, 0));
                ImGui::Button("BST (Coming Soon)", ImVec2(-1, 0));
                ImGui::EndDisabled();
            }

            ImGui::Separator();

            // Visualizer Controls
            if (appState.currentVisualizer) {
                appState.currentVisualizer->renderControls();
            }

            ImGui::End();
        }

        // ===== Visualization Window =====

        if (appState.showVisualization) {
            ImGui::Begin("Visualization");

            if (appState.currentVisualizer) {
                appState.currentVisualizer->renderVisualization();
            }

            ImGui::End();
        }

        // ===== ImGui Demo Window =====

        if (appState.showDemoWindow) {
            ImGui::ShowDemoWindow(&appState.showDemoWindow);
        }

        // ===== Rendering =====

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        // Clear with background color
        auto bgColor = dsav::colors::mocha::base;
        glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // ===== Cleanup =====

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "\nGoodbye!\n";
    return 0;
}

// ===== Callback Implementations =====

static void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << "\n";
}

static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
