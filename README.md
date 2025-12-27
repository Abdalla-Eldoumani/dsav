# Data Structures & Algorithms Visualizer

Educational tool that shows how data structures and algorithms work through visual animations.

## What's Inside

This project has three versions:

### 1. Assembly Version (`dsav-assembly/`)
Terminal-based visualizer written in ARMv8 assembly. Shows colored animations using ANSI escape codes.

**Data Structures:** Array, Stack, Queue, Linked List, Binary Search Tree
**Sorting:** Bubble, Selection, Insertion, Merge, Quick Sort (with detailed step-by-step animations)
**Searching:** Linear, Binary Search (with smart sorted-array detection)
**Features:** User-configurable animation speed (100-2500ms), animated search traversals, table-style displays

**Runs on:** ARM64 Linux (native or WSL with ARM compiler)

### 2. C++ Version (`dsav-cpp/`)
OpenGL-based graphical visualizer with two implementations:

#### Pure C++ (`pure-cpp/`)
Everything written in C++. Works on any system with OpenGL support.

#### Assembly-Linked (`asm-linked/`)
C++ handles graphics, assembly handles data structure operations. Shows how to call assembly code from C++.

**Data Structures:** Same as assembly version
**Graphics:** OpenGL 3.3, Dear ImGui for controls
**Runs on:** x86/ARM Linux (pure-cpp), ARM64 Linux only (asm-linked)

### 3. Rust Version (`dsav-rust/`)
Memory-safe implementation with modern graphical interface using egui and OpenGL.

**Data Structures:** Array, Stack, Queue, Linked List, Binary Search Tree
**Features:** Step-by-step animation, interactive controls, scrolling panels
**Graphics:** OpenGL 3.3, egui for UI
**Runs on:** Windows, Linux, macOS (any platform with OpenGL 3.3+)

## Quick Start

**Assembly version:**
```bash
cd dsav-assembly
make
./dsav
```

**C++ version:**
```bash
cd dsav-cpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./pure-cpp/dsav-pure
```

**Rust version:**
```bash
cd dsav-rust
cargo build --release
cargo run --release --bin dsav-gui
```

## Requirements

**Assembly:**
- ARM64 processor or ARM cross-compiler
- Linux with gcc and m4
- Terminal with ANSI color support

**C++:**
- CMake 3.16+
- GLFW3, GLM
- OpenGL 3.3+ support
- C++17 compiler

**Rust:**
- Rust 1.84.0+
- OpenGL 3.3+ support
- Cargo (comes with Rust)

## Project Structure

```
dsav/
├── dsav-assembly/     # ARM assembly implementation
├── dsav-cpp/          # C++ implementations
│   ├── common/        # Shared graphics code
│   ├── pure-cpp/      # Pure C++ version
│   └── asm-linked/    # C++ + Assembly version
└── dsav-rust/         # Rust implementation
    ├── dsav-core/     # Core library (data structures)
    └── dsav-gui/      # GUI application (egui + OpenGL)
```

## Notes

- Assembly version is for learning low-level programming
- Pure C++ version works on any modern system
- Assembly-linked version shows C++/Assembly integration
- Rust version demonstrates memory safety and modern UI
- All versions have identical features and behavior
