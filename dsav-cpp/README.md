# Data Structures & Algorithms Visualizer - C++

OpenGL visualizer with Dear ImGui controls. Two builds live here: a
feature-complete pure C++ version, and a proof-of-concept that renders in
C++ while the data structures run in ARMv8 assembly.

## Versions

- `pure-cpp/` builds `dsav-pure` everywhere: array, stack, queue, linked
  list, binary search tree, and red-black tree visualizers plus sorting
  (bubble, selection, insertion, merge, quick) and searching (linear,
  binary). Step-by-step playback, animated red-black fixups with case
  explanations, and per-visualizer camera controls.
- `asm-linked/` builds `dsav-asm-linked` on ARM64 only (or anywhere with
  `-DBUILD_ASM_LINKED=ON`): the stack visualizer backed by the real
  assembly from `../dsav-assembly` through its accessor functions
  (`stack_push`, `stack_get_data`, ...). The other structures are stubs
  marked coming soon. Building it runs `make` in the sibling assembly
  project, so that toolchain must work too.

## Requirements

- CMake 3.16+ and a C++17 compiler
- GLFW3 and GLM as system packages
  (`apt install libglfw3-dev libglm-dev` / `brew install glfw glm`)
- OpenGL 3.3+
- Network access on the first configure: Dear ImGui is fetched from its
  repository (docking branch) at configure time. GLAD is vendored in
  `common/`.

## Building

    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build .
    ./pure-cpp/dsav-pure

Those paths assume a single-config generator (Make, Ninja). With a
multi-config generator (Visual Studio) pass `--config Release` to the
build and look for `pure-cpp/Release/dsav-pure.exe`.

Cross-compiling the asm-linked build from x86:

    cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_ASM_LINKED=ON \
             -DCMAKE_TOOLCHAIN_FILE=../arm-toolchain.cmake
    cmake --build .
    qemu-aarch64 ./asm-linked/dsav-asm-linked

## Controls

- Sidebar picks the visualizer; buttons drive the operations; play,
  pause, and step control playback; the speed slider runs 0.1x-5x.
- Camera: drag to pan, wheel to scroll (Shift+wheel horizontal on
  trees), Ctrl+wheel to zoom 0.3x-3x centered on the cursor.
- ESC quits, F11 toggles fullscreen.

## Layout

    common/       shared renderer, animation system, colors, vendored GLAD
    pure-cpp/     data structures, algorithms, and visualizers in C++
    asm-linked/   the assembly-backed stack visualizer and its interface

Colors follow Catppuccin Mocha: yellow comparing, orange swapping, green
sorted, blue active, red removed.
