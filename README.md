# Data Structures & Algorithms Visualizer

One study tool, three implementations. Each animates the classic data
structures and algorithms so you can watch them work: an ARMv8 assembly
version in the terminal, a C++ version on OpenGL, and a Rust version on
egui.

## The versions

### Assembly (`dsav-assembly/`)

Terminal visualizer in AArch64 assembly, animated with ANSI escape codes on
a fixed 80x24 canvas. Array, stack, queue, linked list, binary search tree,
red-black tree (animated insert/delete fixups), heap, hash table, and graph;
sorting and searching over one shared array; towers of hanoi with the call
stack drawn beside the pegs. Every screen goes through one kernel, so the
palette and the frame live in a single pair of files. Adjustable animation
speed. Runs on ARM64 Linux, or any Linux with an AArch64 cross-compiler and
qemu. It also runs in the browser as the multi-file example in the
[aarch64 playground](https://github.com/Abdalla-Eldoumani/aarch64-playground).

### C++ (`dsav-cpp/`)

OpenGL 3.3 + Dear ImGui. `dsav-pure` implements everything in C++ with
step-by-step playback and drag/zoom camera controls, and builds on Linux,
macOS, and Windows. `dsav-asm-linked` (ARM64) is a proof-of-concept that
renders the stack visualizer over the real assembly implementation.

### Rust (`dsav-rust/`)

egui + OpenGL workspace: a pure library crate holds the structures,
algorithms, and unit tests; the GUI crate draws them with step-forward
and step-back playback and ten switchable color themes. Builds on
Windows, Linux, and macOS.

## Quick start

    # assembly
    cd dsav-assembly && make && ./dsav

    # c++
    cd dsav-cpp && mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build .
    ./pure-cpp/dsav-pure

    # rust
    cd dsav-rust && cargo run --release --bin dsav-gui

Each version's README carries its requirements, full build notes, and
controls.

## Layout

    dsav-assembly/     ARMv8 assembly, m4 + gcc, terminal ANSI animation
    dsav-cpp/          C++17, GLFW + GLM + vendored GLAD + fetched ImGui
    dsav-rust/         cargo workspace: dsav-core (library), dsav-gui (binary)

The three versions share the same teaching goal and the same core feature
set; each leans into its platform (the terminal's escape codes, ImGui's
camera, egui's themes), so the details differ deliberately.

MIT licensed.
