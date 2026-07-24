# Data Structures & Algorithms Visualizer - Rust

egui + OpenGL visualizer built as a two-crate workspace: the data
structures and algorithms live in a pure library, the GUI binary draws
them.

## Features

- Array, stack, queue, linked list, binary search tree, and red-black
  tree, seeded with sample data on startup
- Sorting (bubble, selection, insertion, merge, quick) and searching
  (linear, binary with auto-sort) animated over the array
- Playback: play/pause, step forward and back, jump to either end, a
  step counter with a progress bar, and a logarithmic 0.25x-4x speed
  slider
- Ten switchable themes (Vibrant is the default; Tokyo Night, Dracula,
  Gruvbox Dark, One Dark, Nord, Solarized Dark, Catppuccin Mocha,
  Catppuccin Latte, High Contrast)
- Tree views zoom with Ctrl+Scroll; red-black trees can show their NIL
  leaves

## Requirements

- Rust 1.84.0+ (edition 2021)
- OpenGL 3.3+
- Windows, Linux, or macOS

## Building

    cargo run --release --bin dsav-gui

    cargo test --workspace              # dsav-core's unit tests
    cargo fmt --all
    cargo clippy --workspace -- -D warnings

## Layout

    dsav-core/    the library: structures/, algorithms/, the Visualizable
                  trait, rendering state, and every unit test -- no GUI
                  dependencies
    dsav-gui/     the binary: winit + glutin window, glow GL context,
                  egui UI and drawing in app.rs, themes in colors.rs

Windowing is winit 0.30 with a glutin 0.32 OpenGL 3.3 context; egui 0.30
renders through egui_glow; math is glam; errors are thiserror in the
library and anyhow at the edges.
