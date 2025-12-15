cmake_minimum_required(VERSION 3.16)
project(DSAV VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ===== Find System Packages =====

# Find GLFW
find_package(glfw3 REQUIRED)
if(NOT glfw3_FOUND)
    message(FATAL_ERROR "GLFW3 not found. Install with: sudo apt install libglfw3-dev")
endif()

# Find GLM
find_package(glm REQUIRED)
if(NOT glm_FOUND)
    message(FATAL_ERROR "GLM not found. Install with: sudo apt install libglm-dev")
endif()

# Dear ImGui - Fetch from git since it's not typically packaged
include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)

    # Create ImGui library
    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )

    target_link_libraries(imgui PUBLIC glfw)
endif()

# ===== GLAD - OpenGL Loader =====
# Note: GLAD is included directly in the project (common/include/glad)
# Generate from https://glad.dav1d.de/ with OpenGL 3.3 Core profile

add_library(glad STATIC
    common/src/glad.c
)

target_include_directories(glad PUBLIC
    common/include
)

# ===== Common Library =====
# Shared code between pure-cpp and asm-linked versions

add_library(dsav-common STATIC
    common/src/renderer.cpp
    common/src/animation.cpp
    common/src/ui_components.cpp
    common/src/color_scheme.cpp
)

target_include_directories(dsav-common PUBLIC
    common/include
)

target_link_libraries(dsav-common PUBLIC
    glfw
    glad
    imgui
    glm::glm
)

# ===== Build Options =====

option(BUILD_ASM_LINKED "Force build assembly-linked version (requires ARM toolchain)" OFF)

# ===== Subdirectories =====

# Pure C++ version
add_subdirectory(pure-cpp)

# Assembly-linked version
# Build if: (1) ARM64 architecture, OR (2) User forces with -DBUILD_ASM_LINKED=ON
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64" OR BUILD_ASM_LINKED)
    if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
        message(WARNING "Building asm-linked on non-ARM system - requires ARM cross-compiler (gcarm)")
    endif()
    add_subdirectory(asm-linked)
else()
    message(STATUS "Skipping asm-linked (not ARM64). Use -DBUILD_ASM_LINKED=ON to force build.")
endif()
