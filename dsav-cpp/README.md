# Data Structures & Algorithms Visualizer - C++

Graphical visualizer using OpenGL and Dear ImGui. Two versions available.

## Versions

### Pure C++ (`pure-cpp/`)
Everything in C++. Works on any system with OpenGL.

### Assembly-Linked (`asm-linked/`)
C++ handles graphics, assembly handles data structures. Shows C++/Assembly integration.

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
- Merge Sort
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

- Click data structure names to switch
- Use buttons to perform operations
- Play/Pause animation playback
- Step through one operation at a time
- Adjust speed slider (0.1x - 5x)

## Notes

- Pure C++ works on x86 and ARM
- Assembly-linked requires ARM64
- Both versions have identical features
- Graphics require OpenGL 3.3+ support
- Window size: 1280x720 (pure), 1920x1080 (asm)
