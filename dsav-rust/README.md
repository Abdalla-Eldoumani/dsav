# Data Structures & Algorithms Visualizer - Rust

Graphical visualizer using egui and OpenGL. Memory-safe implementation with modern UI.

## Features

**Data Structures:**
- Array (insert, delete, update, search)
- Stack (push, pop, peek)
- Queue (enqueue, dequeue, peek)
- Linked List (insert, delete, update, search, traverse)
- Binary Search Tree (insert, delete, search, traversals)
- Red-Black Tree (insert, delete, search, traversals)

**Sorting:**
- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quick Sort

**Searching:**
- Linear Search
- Binary Search (auto-sorts array)

**Graphics:**
- egui immediate-mode UI
- OpenGL 3.3 rendering
- Catppuccin Mocha color theme
- Smooth animations
- Scrollable panels
- Adjustable speed
- Ctrl+Scroll zoom for tree views
- Optional NIL leaf visualization for Red-Black Trees

## Requirements

- Rust 1.84.0+
- OpenGL 3.3+
- Windows, Linux, or macOS

## Building

```bash
# Build workspace
cargo build --release

# Run GUI application
cargo run --release --bin dsav-gui

# Run tests
cargo test --workspace

# Format and lint
cargo fmt --all
cargo clippy --workspace -- -D warnings
```

## Project Structure

```
dsav-rust/
├── dsav-core/          # Core library
│   ├── src/
│   │   ├── error.rs    # Error types
│   │   ├── traits.rs   # Visualizable trait
│   │   ├── state.rs    # Rendering state
│   │   ├── structures/ # Data structure implementations
│   │   └── algorithms/ # Algorithm implementations
│   └── Cargo.toml
│
└── dsav-gui/           # GUI application
    ├── src/
    │   ├── main.rs     # Entry point
    │   ├── app.rs      # UI and logic
    │   ├── colors.rs   # Color palette
    │   └── renderer/   # Rendering utilities
    └── Cargo.toml
```

## Architecture

Clean separation of concerns:

**dsav-core**: Pure Rust library with no GUI dependencies. All data structure and algorithm logic. Fully testable.

**dsav-gui**: Binary application using dsav-core. Handles windowing, rendering, and user interaction.

## Controls

- Click data structure tabs to switch views
- Use buttons to perform operations
- Binary Search automatically sorts before searching
- Horizontal scrolling for Queue and Linked List
- Vertical scrolling for control panel
- Animation controls (play/pause/step)
- Speed slider
- Ctrl+Scroll to zoom tree views in/out
- Toggle NIL leaves display for Red-Black Trees

## Technology Stack

| Component | Library | Version |
|-----------|---------|---------|
| Language | Rust | 1.84.0+ |
| GUI | egui | 0.30 |
| OpenGL | glow | 0.16 |
| Window | winit | 0.30 |
| Context | glutin | 0.32 |
| Math | glam | 0.28 |
| Errors | thiserror + anyhow | 2.0 + 1.0 |

## Notes

- Workspace-based Cargo project
- Core library is reusable independently
- Educational focus over performance
- Comprehensive error handling with Result types
- All unsafe code is in OpenGL bindings only
