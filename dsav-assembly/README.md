# Data Structures & Algorithms Visualizer - ARMv8 Assembly

Terminal visualizer for the classic data structures and algorithms, written
in AArch64 assembly. Animations are drawn with ANSI escape codes; menus read
plain numeric input.

## Data structures

- Array: init, set, get, swap, clear, with index highlighting
- Stack: push, pop, peek, clear (8 slots, drawn vertically with a top marker)
- Queue: enqueue, dequeue, peek, clear (8 slots, circular, front/rear arrows)
- Linked list: insert front/back, delete, animated search, malloc/free nodes
- Binary search tree: insert, delete, animated search, all four traversals
- Red-black tree: insert, delete, search with animated fixups, colored
  nodes, and a property display

## Algorithms

- Sorting: bubble, selection, insertion, merge, quick - every compare and
  swap is drawn
- Searching: linear and binary with low/mid/high markers; binary offers to
  sort an unsorted array first
- Animation speed is adjustable per run (100-2500 ms)

## Building

Needs Linux, gcc targeting AArch64, and m4.

    make          # build
    make run      # build and run
    make clean

The Makefile's `CC` points at a local wrapper; on a stock toolchain override
it, e.g. `make CC=gcc` on an ARM64 machine. On x86, cross-compile and run
under qemu:

    for f in *.asm; do m4 "$f" > "${f%.asm}.s"; done
    aarch64-linux-gnu-gcc -static *.s -o dsav
    qemu-aarch64 ./dsav

## Layout

    main.asm              menu loop and dispatch
    ansi.asm              colors and cursor control
    display.asm           boxes and centered text
    utils.asm             input, delays, random numbers
    array_viz.asm         one module per structure or algorithm family
    stack_viz.asm
    queue_viz.asm
    linkedlist_viz.asm
    bst_viz.asm
    rbt_viz.asm
    sort_viz.asm
    search_viz.asm

Each .asm file is preprocessed with m4 (register aliases like fp and lr are
m4 defines), assembled with gcc, and linked against libc. Registers follow
AAPCS64 and the stack stays 16-byte aligned.

The stack and queue modules export small accessors (stack_get_data,
queue_get_front, ...) that the C++ asm-linked build reads.
