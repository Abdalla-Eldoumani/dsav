# Data Structures & Algorithms Visualizer - ARMv8 Assembly

An interactive terminal-based visualizer for fundamental data structures and algorithms, written in pure ARMv8 AArch64 assembly language.

## Features

### Data Structures
- **Arrays** - Basic array operations with visual highlighting
- **Stacks** - LIFO structure with vertical visualization
- **Queues** - Circular FIFO implementation with horizontal layout
- **Linked Lists** - Dynamic singly-linked lists with malloc/free

### Algorithms
- **Sorting**
  - Bubble Sort
  - Selection Sort
  - Insertion Sort
- **Searching**
  - Linear Search
  - Binary Search

### Visualization
- Color-coded ANSI terminal output
- Step-by-step animated execution
- Interactive menu-driven interface
- 200ms animation delays for clarity

## Requirements

- **Architecture**: ARMv8 AArch64 (64-bit ARM)
- **OS**: Linux (tested on WSL)
- **Tools**:
  - GNU Assembler (`as`) via gcc
  - m4 macro preprocessor
  - gcc (for linking with libc)

## Building

```bash
make clean    # Remove generated files
make          # Build the project
```

## Running

```bash
./dsav
```

Navigate using the numbered menu options to explore different data structures and algorithms.

## Project Structure

```
dsav-assembly/
├── main.asm              # Entry point and main menu
├── macros.m4             # Shared macro definitions
├── ansi.asm              # ANSI escape code utilities
├── display.asm           # Display helpers (boxes, centering)
├── utils.asm             # Utilities (delay, input, random)
├── array_viz.asm         # Array visualization
├── stack_viz.asm         # Stack implementation
├── queue_viz.asm         # Queue implementation
├── linkedlist_viz.asm    # Linked list implementation
├── sort_viz.asm          # Sorting algorithms
├── search_viz.asm        # Search algorithms
└── Makefile              # Build configuration
```

## Implementation Details

- **Calling Convention**: AArch64 AAPCS64
- **Register Usage**: x19-x28 for callee-saved variables
- **Stack Alignment**: 16-byte alignment maintained
- **C Library Integration**: Uses printf, scanf, malloc, free, usleep

## Notes

- Array/Stack/Queue sizes are limited (8-10 elements) for visibility
- Linked lists use dynamic memory allocation
- All modules are self-contained with independent state
- ANSI colors require terminal support
