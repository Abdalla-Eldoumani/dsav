# Data Structures & Algorithms Visualizer - ARMv8 Assembly

Terminal-based visualizer for data structures and algorithms, written in pure ARMv8 AArch64 assembly.

## Features

### Data Structures
- **Array** - Index-based access with visual highlighting
- **Stack** - LIFO structure, vertical display
- **Queue** - Circular FIFO, horizontal display
- **Linked List** - Dynamic nodes with malloc/free
- **Binary Search Tree** - Insert, delete, search, traversals

### Algorithms
**Sorting:**
- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quick Sort

**Searching:**
- Linear Search
- Binary Search

### Display
- Colored ANSI terminal output
- Step-by-step animations
- Interactive menu
- 500ms delays between steps

## Requirements

- ARM64 processor (or ARM cross-compiler like `aarch64-linux-gnu-gcc`)
- Linux
- gcc (for linking with C library)
- m4 (macro preprocessor)

## Building

```bash
make          # Build project
make clean    # Remove generated files
make test     # Build and run
```

## Running

```bash
./dsav
```

Use number keys to navigate menus.

## File Structure

```
dsav-assembly/
├── main.asm              # Entry point and menu
├── macros.m4             # Shared macros
├── ansi.asm              # Terminal colors and cursor
├── display.asm           # Box drawing and text
├── utils.asm             # Delay, input, random numbers
├── array_viz.asm         # Array operations
├── stack_viz.asm         # Stack with accessor functions
├── queue_viz.asm         # Queue with accessor functions
├── linkedlist_viz.asm    # Linked list with malloc
├── bst_viz.asm           # Binary search tree
├── sort_viz.asm          # Sorting algorithms
├── search_viz.asm        # Search algorithms
└── Makefile              # Build configuration
```

## Technical Details

- **Calling Convention:** AArch64 AAPCS64
- **Stack:** 16-byte aligned
- **Registers:** x19-x28 for local variables (callee-saved)
- **C Functions:** printf, scanf, malloc, free, usleep

## C++ Interface Functions

Stack and queue modules include accessor functions for C++ integration:
- `stack_get_data()` - Returns pointer to stack array
- `stack_get_top()` - Returns current top index
- `queue_get_data()` - Returns pointer to queue array
- `queue_get_front()` - Returns front index
- `queue_get_rear()` - Returns rear index
- `queue_get_count()` - Returns element count

These let C++ code read assembly data structure state for visualization.

## Limits

- Array/Stack/Queue: 8-10 elements (for screen space)
- Linked List/BST: No limit (uses malloc)
- All modules maintain independent state
