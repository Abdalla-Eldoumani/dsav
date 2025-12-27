# Data Structures & Algorithms Visualizer - C++

Graphical visualizer using OpenGL and Dear ImGui. Two versions available.

## Versions

### Pure C++ (`pure-cpp/`)
Everything in C++. Works on any system with OpenGL.

**Recent Updates:**
- Camera controls (pan, zoom, scroll) for all visualizers
- Random initialization for data structures (1-20 elements)
- Improved merge sort visualization with subarray highlighting
- Fixed window management (no overlapping windows)
- Linked list HEAD indicator now points directly to first node
- Linked list NULL displayed as actual node box
- BST random initialization creates balanced trees

### Assembly-Linked (`asm-linked/`)
C++ handles graphics, assembly handles data structures. Shows C++/Assembly integration.
Contains basic stack visualizer only. Not feature-complete.

## Features

**Data Structures:**
- Array
- Stack
- Queue
- Linked List
- Binary Search Tree

**Sorting:**
- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort (with division visualization)
- Quick Sort

**Searching:**
- Linear Search
- Binary Search

**Graphics:**
- OpenGL 3.3 rendering
- Dear ImGui controls
- Smooth animations
- Adjustable speed
- Step-by-step mode

**Camera Controls (Pure C++ only):**
- Drag to pan visualization
- Mouse wheel to scroll
- Ctrl + Mouse wheel to zoom (0.3x - 3.0x)
- Zoom centers on mouse cursor
- All visualizers support camera controls

## Requirements

- CMake 3.16+
- C++17 compiler
- GLFW3
- GLM
- OpenGL 3.3+

**For asm-linked version:**
- ARM64 Linux (or ARM cross-compiler)
- Assembly object files from `../dsav-assembly/`

## Building

```bash
mkdir build && cd build

# Build both versions
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Or force asm-linked on x86 (for cross-compilation)
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_ASM_LINKED=ON
cmake --build .
```

## Running

**Pure C++ version:**
```bash
./pure-cpp/dsav-pure
```

**Assembly-linked version (ARM64 only):**
```bash
./asm-linked/dsav-asm-linked
```

## Project Structure

```
dsav-cpp/
├── CMakeLists.txt           # Root build config
├── arm-toolchain.cmake      # ARM cross-compilation setup
│
├── common/                  # Shared code
│   ├── include/
│   │   ├── visualizer.hpp   # Base interface
│   │   ├── renderer.hpp     # OpenGL utilities
│   │   ├── animation.hpp    # Animation system
│   │   ├── color_scheme.hpp # Catppuccin colors
│   │   └── ui_components.hpp
│   └── src/                 # Implementation files
│
├── pure-cpp/
│   ├── include/
│   │   ├── data_structures/ # C++ data structures
│   │   ├── algorithms/      # C++ algorithms
│   │   └── visualizers/     # Visualizer classes
│   ├── src/                 # Implementation files
│   └── CMakeLists.txt
│
└── asm-linked/
    ├── include/
    │   ├── asm_interface.hpp     # Assembly function declarations
    │   └── asm_stack_visualizer.hpp
    ├── src/
    │   ├── main.cpp
    │   └── visualizers/          # Calls assembly, syncs visuals
    └── CMakeLists.txt
```

## How It Works

**Pure C++:**
1. C++ data structures store data
2. Visualizer reads state, draws graphics
3. User clicks buttons → calls C++ functions
4. Animations play → screen updates

**Assembly-Linked:**
1. Assembly data structures store data
2. C++ calls assembly functions (e.g., `stack_push(42)`)
3. C++ reads assembly state via accessor functions
4. Visualizer draws based on assembly data
5. OpenGL renders the result

## Building for ARM Cross-Compilation

If compiling on x86 for ARM target:

```bash
# Install ARM toolchain
sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

# Build with toolchain file
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DBUILD_ASM_LINKED=ON \
         -DCMAKE_TOOLCHAIN_FILE=../arm-toolchain.cmake
cmake --build .

# Binary is ARM64, run with QEMU or on ARM hardware
qemu-aarch64 ./asm-linked/dsav-asm-linked
```

## Dependencies

The build system automatically downloads Dear ImGui. Other dependencies:

**Ubuntu/Debian:**
```bash
sudo apt install libglfw3-dev libglm-dev
```

**macOS:**
```bash
brew install glfw glm
```

## Controls

**Basic:**
- Click data structure names to switch
- Use buttons to perform operations
- Play/Pause animation playback
- Step through one operation at a time
- Adjust speed slider (0.1x - 5x)

**Camera (Pure C++ only):**
- Drag to pan
- Scroll to move vertically
- Shift + Scroll for horizontal movement
- Ctrl + Scroll to zoom in/out
- ESC to exit
- F11 for fullscreen

## Visualization Details

**Merge Sort:**
- Left subarray highlighted in yellow
- Right subarray highlighted in orange
- Status shows range being merged: "Merging [0..2] (yellow) with [3..5] (orange)"

**Linked List:**
- HEAD indicator positioned 50px from first node
- NULL displayed as semi-transparent node box at end
- Arrows show pointer connections

**Binary Search Tree:**
- Random initialization uses shuffled values to prevent degenerate trees
- Supports zoom and pan for navigating large trees

**Color Scheme (Catppuccin Mocha):**
- Yellow: Comparing elements
- Orange: Swapping elements
- Green: Sorted/final position
- Blue: Active/selected
- Red: Deleted/error

## Notes

- Pure C++ works on x86 and ARM
- Assembly-linked requires ARM64
- Pure C++ is feature-complete
- Assembly-linked is proof-of-concept only
- Graphics require OpenGL 3.3+ support
- Window size: 1920x1080 (pure-cpp), adjustable in source
