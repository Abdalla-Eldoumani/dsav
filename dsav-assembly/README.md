# Data Structures & Algorithms Visualizer - ARMv8 Assembly

Terminal visualizer for the classic data structures and algorithms, written
in AArch64 assembly. Every screen is drawn with ANSI escape codes on a fixed
80x24 canvas; menus read plain numeric input.

## Data structures

- Array: init, set, get, swap, clear, with index highlighting
- Stack: push, pop, peek, clear (8 slots, drawn vertically with a top marker)
- Queue: enqueue, dequeue, peek, clear (8 slots, circular, front/rear arrows)
- Linked list: insert front/back, delete, animated search, malloc/free nodes
- Binary search tree: insert, delete, animated search, all four traversals
- Red-black tree: insert, delete, search with animated fixups, colored
  nodes, and a property display
- Heap: insert with sift-up, extract-min with sift-down, peek, and Floyd's
  bottom-up build, shown as a tree over the array that backs it
- Hash table: insert, search, and delete with linear probing, a load-factor
  bar, a collision count, and each displaced key labelled with its home
  bucket
- Graph: breadth first and depth first walks over a shared eight-vertex
  graph, with the queue or stack shown beside it, plus connected components
  and edge editing

## Algorithms

- Sorting over one shared array, every compare and move drawn: bubble,
  selection, insertion, merge, quick, heap, shell, and counting. Counting
  sort gets its own layout - input strip, bucket grid, output strip - so the
  run that costs zero comparisons can be read next to the ones that do not
- Searching, each with the markers its own rule needs: linear, binary
  (low/mid/high), jump (block ends, then a walk), and interpolation (a
  computed guess). The arithmetic is shown as symbols and again as numbers,
  and the shipped array is spaced so interpolation finds the demo target in
  one probe where binary takes four
- Recursion: towers of hanoi, with the real call stack shown frame by frame
  beside the pegs
- Animation speed is adjustable per run

## Screens

Every screen is drawn through one kernel rather than by hand:

- `theme.asm` holds the palette. Modules ask for a role (the cell in hand,
  compared, settled, removed) and never for a colour, so the whole program
  restyles from one file.
- `ui.asm` owns the canvas. The frame is rows 1 and 24, the title bar row 2,
  rules on rows 3 and 21, the footer row 22, and the message row 23 that
  `utils.asm` writes input complaints into. Modules own rows 4 to 20 and ask
  for panels, badges, text, and the big-O card by name.

Where an algorithm runs, the screen carries a complexity card: what the run
you just watched costs in the best, average, and worst case, and in memory.

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

    main.asm              the home screen and dispatch
    theme.asm             the palette, addressed by role
    ui.asm                the screen kernel: frame, panels, badges, cards
    ansi.asm              colors and cursor control
    display.asm           boxes and centered text
    utils.asm             input, delays, random numbers
    array_viz.asm         one module per structure or algorithm family
    stack_viz.asm
    queue_viz.asm
    linkedlist_viz.asm
    bst_viz.asm
    rbt_viz.asm
    heap_viz.asm
    hash_viz.asm
    graph_viz.asm
    sort_viz.asm
    search_viz.asm
    recursion_viz.asm

Each .asm file is preprocessed with m4 (register aliases like fp and lr are
m4 defines), assembled with gcc, and linked against libc. Registers follow
AAPCS64 and the stack stays 16-byte aligned.

Two things the preprocessor makes sharp. m4 reads a backtick as an opening
quote, so a single backtick in a comment swallows source up to the next
apostrophe: keep both out of comments. And each file assembles on its own,
so a module that names a `UI_ROLE_*` constant declares it locally; ui.asm
holds the canonical values.

The stack and queue modules export small accessors (stack_get_data,
queue_get_front, ...) that the C++ asm-linked build reads.
