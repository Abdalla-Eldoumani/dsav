/**
 * @file main.cpp
 * @brief Entry point for DSAV Pure C++ version
 *
 * Initializes GLFW window, OpenGL context, and Dear ImGui, then runs the main application loop.
 */

#include <iostream>
#include <string>
#include <memory>

// OpenGL and windowing
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE  // Tell GLFW not to include OpenGL headers (we use GLAD)
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

// DSAV visualizers
#include "visualizers/stack_visualizer.hpp"
#include "visualizers/queue_visualizer.hpp"
#include "visualizers/array_visualizer.hpp"
#include "visualizers/linked_list_visualizer.hpp"
#include "visualizers/bst_visualizer.hpp"
#include "visualizers/sorting_visualizer.hpp"
#include "visualizers/searching_visualizer.hpp"

// GLM for math
#include <glm/glm.hpp>

// ===== Application Configuration =====

constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;
constexpr const char* WINDOW_TITLE = "DSAV - Data Structures & Algorithms Visualizer";
constexpr const char* GLSL_VERSION = "#version 330 core";

// ===== Forward Declarations =====

static void glfwErrorCallback(int error, const char* description);
static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// ===== Application State =====

struct ApplicationState {
    bool showDemoWindow = false;
    bool showSidebar = true;
    bool showLogPanel = true;
    bool showVisualization = true;

    // Current visualizer
    std::unique_ptr<dsav::IVisualizer> currentVisualizer;

    std::string statusMessage = "Ready";
};

// ===== Main Function =====

int main(int argc, char** argv) {
    std::cout << "DSAV - Data Structures & Algorithms Visualizer\n";
    std::cout << "Pure C++ Version with OpenGL\n";
    std::cout << "==============================================\n\n";

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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Required on Mac
    #endif

    // Enable MSAA (anti-aliasing)
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
    glEnable(GL_MULTISAMPLE);  // MSAA
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ===== 3. Initialize Dear ImGui =====

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Enable docking and viewports
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup ImGui style
    ImGui::StyleColorsDark();
    dsav::colors::applyImGuiTheme();  // Apply Catppuccin Mocha theme

    // When viewports are enabled, tweak WindowRounding/WindowBg for platform windows
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    // Load fonts (optional - use default for now)
    // TODO: Load custom fonts from assets/fonts/

    std::cout << "Initialization complete!\n";
    std::cout << "Press ESC to exit\n\n";

    // ===== 4. Application State =====

    ApplicationState appState;

    // Initialize with Stack visualizer
    appState.currentVisualizer = std::make_unique<dsav::StackVisualizer>(16);

    // ===== 5. Main Loop =====

    // Track delta time
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
                    appState.statusMessage = "Reset!";
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
                ImGui::MenuItem("Log Panel", nullptr, &appState.showLogPanel);
                ImGui::Separator();
                ImGui::MenuItem("ImGui Demo", nullptr, &appState.showDemoWindow);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About")) {
                    appState.statusMessage = "DSAV v1.0.0 - Educational Visualizer";
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
            ImGui::Text("Data Structures & Algorithms");
            ImGui::Separator();

            // Data Structures Section
            if (ImGui::CollapsingHeader("Data Structures", ImGuiTreeNodeFlags_DefaultOpen)) {
                // Array button
                bool isArrayActive = appState.currentVisualizer &&
                                     appState.currentVisualizer->getName() == "Array";
                if (isArrayActive) {
                    ImGui::PushStyleColor(ImGuiCol_Button,
                        dsav::colors::toImGui(dsav::colors::semantic::active));
                }
                if (ImGui::Button("Array", ImVec2(-1, 0))) {
                    appState.currentVisualizer = std::make_unique<dsav::ArrayVisualizer>();
                    appState.statusMessage = "Array selected";
                }
                if (isArrayActive) {
                    ImGui::PopStyleColor();
                }

                // Stack button
                bool isStackActive = appState.currentVisualizer &&
                                     appState.currentVisualizer->getName() == "Stack";
                if (isStackActive) {
                    ImGui::PushStyleColor(ImGuiCol_Button,
                        dsav::colors::toImGui(dsav::colors::semantic::active));
                }
                if (ImGui::Button("Stack", ImVec2(-1, 0))) {
                    appState.currentVisualizer = std::make_unique<dsav::StackVisualizer>(16);
                    appState.statusMessage = "Stack selected";
                }
                if (isStackActive) {
                    ImGui::PopStyleColor();
                }

                // Queue button
                bool isQueueActive = appState.currentVisualizer &&
                                     appState.currentVisualizer->getName() == "Queue";
                if (isQueueActive) {
                    ImGui::PushStyleColor(ImGuiCol_Button,
                        dsav::colors::toImGui(dsav::colors::semantic::active));
                }
                if (ImGui::Button("Queue", ImVec2(-1, 0))) {
                    appState.currentVisualizer = std::make_unique<dsav::QueueVisualizer>(16);
                    appState.statusMessage = "Queue selected";
                }
                if (isQueueActive) {
                    ImGui::PopStyleColor();
                }

                // Linked List button
                bool isLinkedListActive = appState.currentVisualizer &&
                                          appState.currentVisualizer->getName() == "Linked List";
                if (isLinkedListActive) {
                    ImGui::PushStyleColor(ImGuiCol_Button,
                        dsav::colors::toImGui(dsav::colors::semantic::active));
                }
                if (ImGui::Button("Linked List", ImVec2(-1, 0))) {
                    appState.currentVisualizer = std::make_unique<dsav::LinkedListVisualizer>();
                    appState.statusMessage = "Linked List selected";
                }
                if (isLinkedListActive) {
                    ImGui::PopStyleColor();
                }

                // Binary Search Tree button
                bool isBSTActive = appState.currentVisualizer &&
                                   appState.currentVisualizer->getName() == "Binary Search Tree";
                if (isBSTActive) {
                    ImGui::PushStyleColor(ImGuiCol_Button,
                        dsav::colors::toImGui(dsav::colors::semantic::active));
                }
                if (ImGui::Button("Binary Search Tree", ImVec2(-1, 0))) {
                    appState.currentVisualizer = std::make_unique<dsav::BSTVisualizer>();
                    appState.statusMessage = "Binary Search Tree selected";
                }
                if (isBSTActive) {
                    ImGui::PopStyleColor();
                }
            }

            ImGui::Spacing();

            // Algorithms Section
            if (ImGui::CollapsingHeader("Algorithms")) {
                // Sorting Algorithms button
                bool isSortingActive = appState.currentVisualizer &&
                                       appState.currentVisualizer->getName() == "Sorting Algorithms";
                if (isSortingActive) {
                    ImGui::PushStyleColor(ImGuiCol_Button,
                        dsav::colors::toImGui(dsav::colors::semantic::active));
                }
                if (ImGui::Button("Sorting Algorithms", ImVec2(-1, 0))) {
                    appState.currentVisualizer = std::make_unique<dsav::SortingVisualizer>();
                    appState.statusMessage = "Sorting Algorithms selected";
                }
                if (isSortingActive) {
                    ImGui::PopStyleColor();
                }

                ImGui::Spacing();

                // Search Algorithms button
                bool isSearchingActive = appState.currentVisualizer &&
                                         appState.currentVisualizer->getName() == "Search Algorithms";
                if (isSearchingActive) {
                    ImGui::PushStyleColor(ImGuiCol_Button,
                        dsav::colors::toImGui(dsav::colors::semantic::active));
                }
                if (ImGui::Button("Search Algorithms", ImVec2(-1, 0))) {
                    appState.currentVisualizer = std::make_unique<dsav::SearchingVisualizer>();
                    appState.statusMessage = "Search Algorithms selected";
                }
                if (isSearchingActive) {
                    ImGui::PopStyleColor();
                }
            }

            ImGui::End();
        }

        // ===== Main Visualization Area =====

        if (appState.showVisualization) {
            ImGui::Begin("Visualization", &appState.showVisualization);

            if (appState.currentVisualizer) {
                appState.currentVisualizer->renderVisualization();
            } else {
                ImGui::TextColored(
                    dsav::colors::toImGui(dsav::colors::semantic::textSecondary),
                    "No visualizer selected. Choose a data structure from the sidebar."
                );
            }

            ImGui::End();
        }

        // ===== Visualizer Controls =====

        // Let the visualizer render its own control panel
        if (appState.currentVisualizer) {
            appState.currentVisualizer->renderControls();
        }

        // ===== Log Panel =====

        if (appState.showLogPanel) {
            ImGui::Begin("Log", &appState.showLogPanel);

            ImGui::TextColored(dsav::colors::toImGui(dsav::colors::mocha::green), "[INFO]");
            ImGui::SameLine();
            ImGui::Text("Application started successfully");

            ImGui::TextColored(dsav::colors::toImGui(dsav::colors::mocha::blue), "[INFO]");
            ImGui::SameLine();
            ImGui::Text("Ready for visualization");

            ImGui::TextColored(dsav::colors::toImGui(dsav::colors::mocha::yellow), "[WARN]");
            ImGui::SameLine();
            ImGui::Text("This is a demo warning message");

            ImGui::End();
        }

        // ===== ImGui Demo Window (for development) =====

        if (appState.showDemoWindow) {
            ImGui::ShowDemoWindow(&appState.showDemoWindow);
        }

        // ===== Rendering =====

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        // Clear with Catppuccin Mocha background color
        glClearColor(
            dsav::colors::mocha::base.r,
            dsav::colors::mocha::base.g,
            dsav::colors::mocha::base.b,
            dsav::colors::mocha::base.a
        );
        glClear(GL_COLOR_BUFFER_BIT);

        // Render ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // ===== 6. Cleanup =====

    std::cout << "\nShutting down...\n";

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Goodbye!\n";

    return 0;
}

// ===== GLFW Callbacks =====

static void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << "\n";
}

static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // ESC to close
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // F11 for fullscreen toggle
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        static bool isFullscreen = false;
        static int windowed_x, windowed_y, windowed_width, windowed_height;

        if (!isFullscreen) {
            // Save windowed position and size
            glfwGetWindowPos(window, &windowed_x, &windowed_y);
            glfwGetWindowSize(window, &windowed_width, &windowed_height);

            // Switch to fullscreen
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            // Switch back to windowed
            glfwSetWindowMonitor(window, nullptr, windowed_x, windowed_y, windowed_width, windowed_height, 0);
        }

        isFullscreen = !isFullscreen;
    }
}
