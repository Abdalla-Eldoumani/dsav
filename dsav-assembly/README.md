# Data Structures & Algorithms Visualizer - ARMv8 Assembly

Terminal-based visualizer for data structures and algorithms, written in pure ARMv8 AArch64 assembly.

## Features

### Data Structures
- **Array** - Index-based access with visual highlighting and table display
- **Stack** - LIFO structure, vertical display with top indicator
- **Queue** - Circular FIFO, horizontal display with front/rear pointers
- **Linked List** - Dynamic nodes with malloc/free and **animated search traversal**
  - Search highlights each node as it traverses (similar to BST search)
- **Binary Search Tree** - Insert, delete, **animated search**, traversals (in/pre/post-order)
  - ASCII tree display option
  - Search highlights path through tree

### Algorithms
**Sorting:**
- **Bubble Sort** - Shows each comparison and swap
- **Selection Sort** - Highlights minimum search process
- **Insertion Sort** - Displays shifting elements step-by-step
- **Merge Sort** - Visualizes division, comparison, and merging
- **Quick Sort** - Shows pivot comparisons and partitioning

All sorting algorithms support user-configurable animation speed.

**Searching:**
- **Linear Search** - Sequential scanning with element highlighting
- **Binary Search** - Shows low/mid/high pointers with smart sorted array detection
  - Automatically checks if array is sorted
  - Offers to sort array if needed before searching

Both search algorithms support user-configurable animation speed and table-style array display.

### Display
- Colored ANSI terminal output
- Step-by-step animations with detailed comparisons
- Interactive menu with input validation
- **User-configurable animation speed** (100-2500ms)
  - Adjustable before each sort or search operation
  - Allows fast overview or slow step-by-step learning

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
