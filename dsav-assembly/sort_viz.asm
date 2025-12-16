// ============================================================================
// sort_viz.asm - Sorting Algorithm Visualization Module
// ============================================================================
// Implements animated visualizations for:
//   - Bubble Sort
//   - Selection Sort
//   - Insertion Sort
//   - Merge Sort
//   - Quick Sort
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// ----------------------------------------------------------------------------
// Sorting visualization state
// ----------------------------------------------------------------------------
sort_array:         .skip 40                // Temporary array for sorting (10 elements)
sort_aux_array:     .skip 40                // Auxiliary array for merge sort
sort_size:          .word 0                 // Current array size
sort_delay:         .word 500               // Animation delay in ms

// Highlight indices for visualization
highlight_idx1:     .word -1                // First highlighted index
highlight_idx2:     .word -1                // Second highlighted index
sorted_up_to:       .word -1                // Elements up to this index are sorted

// For merge sort and quick sort
merge_left:         .word -1                // Left bound of current merge/partition
merge_right:        .word -1                // Right bound of current merge/partition
merge_mid:          .word -1                // Middle point for merge

// ----------------------------------------------------------------------------
// UI Strings
// ----------------------------------------------------------------------------
sort_title:         .string "SORTING ALGORITHM VISUALIZATION"
sort_menu_title:    .string "SORTING ALGORITHMS MENU"

menu_opt_1:         .string "[1] Bubble Sort"
menu_opt_2:         .string "[2] Selection Sort"
menu_opt_3:         .string "[3] Insertion Sort"
menu_opt_4:         .string "[4] Merge Sort"
menu_opt_5:         .string "[5] Quick Sort"
menu_opt_6:         .string "[6] Initialize Random Array"
menu_opt_0:         .string "[0] Back to Main Menu"

menu_prompt:        .string "Enter your choice: "
prompt_size:        .string "Enter array size (3-10): "
prompt_continue:    .string "\n\x1b[33mPress Enter to start sorting...\x1b[0m"

msg_sorted:         .string "\x1b[32mArray is now sorted!\x1b[0m"
msg_initialized:    .string "\x1b[32mArray initialized with %d random elements.\x1b[0m"
msg_empty:          .string "\x1b[33mPlease initialize array first!\x1b[0m"
msg_comparing:      .string "Comparing: "
msg_swapping:       .string "Swapping: "
msg_sorted_marker:  .string "✓ Sorted"

label_comparisons:  .string "Comparisons: %d"
label_swaps:        .string "Swaps: %d"

    .text
    .balign 4

// ============================================================================
// FUNCTION: sort_menu
// Main menu for sorting algorithms
// Parameters: none
// Returns: none
// ============================================================================
    .global sort_menu
sort_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

sort_menu_loop:
    // Clear screen and display menu
    bl      ansi_clear_screen
    bl      display_sort_menu

    // Get user choice
    mov     w0, 0                            // min
    mov     w1, 6                            // max
    bl      read_int_range

    // Dispatch based on choice
    cmp     w0, 0
    b.eq    sort_menu_exit

    cmp     w0, 1
    b.eq    sort_menu_bubble

    cmp     w0, 2
    b.eq    sort_menu_selection

    cmp     w0, 3
    b.eq    sort_menu_insertion

    cmp     w0, 4
    b.eq    sort_menu_merge

    cmp     w0, 5
    b.eq    sort_menu_quick

    cmp     w0, 6
    b.eq    sort_menu_initialize

    b       sort_menu_loop

sort_menu_bubble:
    bl      sort_bubble_interactive
    bl      wait_for_enter
    b       sort_menu_loop

sort_menu_selection:
    bl      sort_selection_interactive
    bl      wait_for_enter
    b       sort_menu_loop

sort_menu_insertion:
    bl      sort_insertion_interactive
    bl      wait_for_enter
    b       sort_menu_loop

sort_menu_merge:
    bl      sort_merge_interactive
    bl      wait_for_enter
    b       sort_menu_loop

sort_menu_quick:
    bl      sort_quick_interactive
    bl      wait_for_enter
    b       sort_menu_loop

sort_menu_initialize:
    bl      sort_initialize_array
    bl      wait_for_enter
    b       sort_menu_loop

sort_menu_exit:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: display_sort_menu
// Displays the sorting algorithms menu
// Parameters: none
// Returns: none
// ============================================================================
    .global display_sort_menu
display_sort_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Draw menu box
    mov     w0, 3                            // row
    mov     w1, 15                           // column
    mov     w2, 50                           // width
    mov     w3, 15                           // height (increased for more options)
    mov     w4, 1                            // double-line style
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 17
    bl      ansi_move_cursor
    adrp    x0, sort_menu_title
    add     x0, x0, :lo12:sort_menu_title
    mov     w1, 46
    bl      print_centered

    // Print separator
    mov     w0, 5
    mov     w1, 15
    mov     w2, 50
    mov     w3, 1
    bl      draw_horizontal_border_top

    // Print menu options
    mov     w0, 7
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_1
    add     x0, x0, :lo12:menu_opt_1
    bl      printf

    mov     w0, 8
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_2
    add     x0, x0, :lo12:menu_opt_2
    bl      printf

    mov     w0, 9
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_3
    add     x0, x0, :lo12:menu_opt_3
    bl      printf

    mov     w0, 10
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_4
    add     x0, x0, :lo12:menu_opt_4
    bl      printf

    mov     w0, 11
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_5
    add     x0, x0, :lo12:menu_opt_5
    bl      printf

    mov     w0, 12
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_6
    add     x0, x0, :lo12:menu_opt_6
    bl      printf

    mov     w0, 13
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_opt_0
    add     x0, x0, :lo12:menu_opt_0
    bl      printf

    // Print separator
    mov     w0, 15
    mov     w1, 15
    mov     w2, 50
    mov     w3, 1
    bl      draw_horizontal_border_top

    // Position cursor for input
    mov     w0, 16
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_prompt
    add     x0, x0, :lo12:menu_prompt
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: sort_initialize_array
// Initializes the sorting array with random values
// Parameters: none
// Returns: none
// ============================================================================
    .global sort_initialize_array
sort_initialize_array:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    bl      ansi_clear_screen

    // Prompt for size
    adrp    x0, prompt_size
    add     x0, x0, :lo12:prompt_size
    bl      printf

    mov     w0, 3
    mov     w1, 10
    bl      read_int_range
    mov     w19, w0                          // Save size

    // Store size
    adrp    x20, sort_size
    add     x20, x20, :lo12:sort_size
    str     w19, [x20]

    // Fill array with random values
    adrp    x20, sort_array
    add     x20, x20, :lo12:sort_array
    mov     w21, 0

sort_init_loop:
    cmp     w21, w19
    b.ge    sort_init_done

    mov     w0, 100
    bl      get_random
    str     w0, [x20, w21, SXTW 2]

    add     w21, w21, 1
    b       sort_init_loop

sort_init_done:
    // Display array
    bl      sort_display_array

    // Position cursor for message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_initialized
    add     x0, x0, :lo12:msg_initialized
    mov     w1, w19
    bl      printf
    bl      print_newline

    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: sort_display_array
// Displays array with highlighting for visualization
// Parameters: none
// Returns: none
// ============================================================================
    .global sort_display_array
sort_display_array:
    stp     x29, x30, [sp, -80]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]
    stp     x25, x26, [sp, 64]

    // Load size
    adrp    x19, sort_size
    add     x19, x19, :lo12:sort_size
    ldr     w19, [x19]

    // Load array
    adrp    x20, sort_array
    add     x20, x20, :lo12:sort_array

    // Load highlight indices
    adrp    x21, highlight_idx1
    add     x21, x21, :lo12:highlight_idx1
    ldr     w21, [x21]

    adrp    x22, highlight_idx2
    add     x22, x22, :lo12:highlight_idx2
    ldr     w22, [x22]

    adrp    x23, sorted_up_to
    add     x23, x23, :lo12:sorted_up_to
    ldr     w23, [x23]

    // Load merge range for merge/quick sort visualization
    adrp    x25, merge_left
    add     x25, x25, :lo12:merge_left
    ldr     w25, [x25]

    adrp    x26, merge_right
    add     x26, x26, :lo12:merge_right
    ldr     w26, [x26]

    // Clear screen
    bl      ansi_clear_screen

    // Draw box
    mov     w0, 3
    mov     w1, 2
    mov     w2, 80
    mov     w3, 10
    mov     w4, 0
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 4
    bl      ansi_move_cursor
    adrp    x0, sort_title
    add     x0, x0, :lo12:sort_title
    mov     w1, 76
    bl      print_centered

    // Print separator
    mov     w0, 5
    mov     w1, 2
    mov     w2, 80
    mov     w3, 0
    bl      draw_horizontal_border_top

    // Print array values
    mov     w0, 8
    mov     w1, 15
    bl      ansi_move_cursor

    mov     w24, 0                           // Index counter
sort_display_loop:
    cmp     w24, w19
    b.ge    sort_display_done

    // Determine color for this element
    cmp     w24, w21
    b.eq    sort_display_highlight1
    cmp     w24, w22
    b.eq    sort_display_highlight2
    cmp     w24, w23
    b.le    sort_display_sorted

    // Check if in merge range (for merge/quick sort)
    cmp     w25, 0
    b.lt    sort_display_normal              // merge_left < 0, not in merge mode
    cmp     w24, w25
    b.lt    sort_display_normal              // element before merge range
    cmp     w24, w26
    b.gt    sort_display_normal              // element after merge range

    // Element is in merge range - highlight with yellow
    mov     w0, 43                           // Yellow background
    bl      ansi_set_color_bg
    b       sort_display_print

sort_display_normal:
    // Normal color (white)
    bl      ansi_reset_attributes
    b       sort_display_print

sort_display_highlight1:
    mov     w0, 43                           // Yellow background
    bl      ansi_set_color_bg
    b       sort_display_print

sort_display_highlight2:
    mov     w0, 46                           // Cyan background
    bl      ansi_set_color_bg
    b       sort_display_print

sort_display_sorted:
    mov     w0, 42                           // Green background
    bl      ansi_set_color_bg

sort_display_print:
    // Print value
    ldr     w1, [x20, w24, SXTW 2]
    adrp    x0, .Lvalue_fmt
    add     x0, x0, :lo12:.Lvalue_fmt
    bl      printf

    // Reset color
    bl      ansi_reset_attributes

    // Print space
    adrp    x0, .Lspace
    add     x0, x0, :lo12:.Lspace
    bl      printf

    add     w24, w24, 1
    b       sort_display_loop

sort_display_done:
    bl      print_newline

    ldp     x25, x26, [sp, 64]
    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 80
    ret

    .section .rodata
.Lvalue_fmt:    .string "[%3d]"
.Lspace:        .string " "
    .text

// ============================================================================
// FUNCTION: sort_bubble_interactive
// Interactive bubble sort with visualization
// Parameters: none
// Returns: none
// ============================================================================
    .global sort_bubble_interactive
sort_bubble_interactive:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Check if array is initialized
    adrp    x0, sort_size
    add     x0, x0, :lo12:sort_size
    ldr     w0, [x0]
    cmp     w0, 0
    b.le    sort_bubble_empty

    // Reset highlights
    bl      sort_reset_highlights

    // Show initial array
    bl      sort_display_array

    // Wait for user
    adrp    x0, prompt_continue
    add     x0, x0, :lo12:prompt_continue
    bl      printf
    bl      wait_for_enter

    // Perform bubble sort
    bl      bubble_sort

    // Show sorted message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_sorted
    add     x0, x0, :lo12:msg_sorted
    bl      printf
    bl      print_newline

    b       sort_bubble_done

sort_bubble_empty:
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

sort_bubble_done:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: bubble_sort
// Bubble sort with visualization
// Parameters: none (uses global sort_array and sort_size)
// Returns: none
// ============================================================================
    .global bubble_sort
bubble_sort:
    stp     x29, x30, [sp, -64]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]

    // Load size
    adrp    x19, sort_size
    add     x19, x19, :lo12:sort_size
    ldr     w19, [x19]

    // Load array
    adrp    x20, sort_array
    add     x20, x20, :lo12:sort_array

    // Outer loop: i = 0 to n-2
    mov     w21, 0
bubble_outer:
    sub     w0, w19, 1
    cmp     w21, w0
    b.ge    bubble_done

    // Inner loop: j = 0 to n-i-2
    mov     w22, 0
bubble_inner:
    sub     w0, w19, w21
    sub     w0, w0, 2
    cmp     w22, w0
    b.gt    bubble_inner_done

    // Highlight elements being compared
    adrp    x23, highlight_idx1
    add     x23, x23, :lo12:highlight_idx1
    str     w22, [x23]

    add     w0, w22, 1
    adrp    x23, highlight_idx2
    add     x23, x23, :lo12:highlight_idx2
    str     w0, [x23]

    // Display with highlights
    bl      sort_display_array

    // Load values to compare
    ldr     w23, [x20, w22, SXTW 2]          // arr[j]
    add     w0, w22, 1
    ldr     w24, [x20, w0, SXTW 2]           // arr[j+1]

    // Delay for visualization
    adrp    x1, sort_delay
    add     x1, x1, :lo12:sort_delay
    ldr     w0, [x1]
    bl      delay_ms

    // Compare and swap if needed
    cmp     w23, w24
    b.le    bubble_no_swap

    // Swap arr[j] and arr[j+1]
    str     w24, [x20, w22, SXTW 2]
    add     w0, w22, 1
    str     w23, [x20, w0, SXTW 2]

    // Show swap with different delay
    bl      sort_display_array
    mov     w0, 100
    bl      delay_ms

bubble_no_swap:
    add     w22, w22, 1
    b       bubble_inner

bubble_inner_done:
    // Mark last element as sorted
    sub     w0, w19, w21
    sub     w0, w0, 1
    adrp    x23, sorted_up_to
    add     x23, x23, :lo12:sorted_up_to
    str     w0, [x23]

    add     w21, w21, 1
    b       bubble_outer

bubble_done:
    // Mark all as sorted
    sub     w0, w19, 1
    adrp    x23, sorted_up_to
    add     x23, x23, :lo12:sorted_up_to
    str     w0, [x23]

    bl      sort_reset_highlights
    bl      sort_display_array

    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 64
    ret

// ============================================================================
// FUNCTION: sort_selection_interactive
// Interactive selection sort with visualization
// Parameters: none
// Returns: none
// ============================================================================
    .global sort_selection_interactive
sort_selection_interactive:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Check if array is initialized
    adrp    x0, sort_size
    add     x0, x0, :lo12:sort_size
    ldr     w0, [x0]
    cmp     w0, 0
    b.le    sort_selection_empty

    bl      sort_reset_highlights
    bl      sort_display_array

    adrp    x0, prompt_continue
    add     x0, x0, :lo12:prompt_continue
    bl      printf
    bl      wait_for_enter

    bl      selection_sort

    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_sorted
    add     x0, x0, :lo12:msg_sorted
    bl      printf
    bl      print_newline

    b       sort_selection_done

sort_selection_empty:
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

sort_selection_done:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: selection_sort
// Selection sort with visualization
// Parameters: none (uses global sort_array and sort_size)
// Returns: none
// ============================================================================
    .global selection_sort
selection_sort:
    stp     x29, x30, [sp, -64]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]

    // Load size and array
    adrp    x19, sort_size
    add     x19, x19, :lo12:sort_size
    ldr     w19, [x19]

    adrp    x20, sort_array
    add     x20, x20, :lo12:sort_array

    // Outer loop: i = 0 to n-2
    mov     w21, 0
selection_outer:
    sub     w0, w19, 1
    cmp     w21, w0
    b.ge    selection_done

    // min_idx = i
    mov     w22, w21

    // Inner loop: j = i+1 to n-1
    add     w23, w21, 1
selection_inner:
    cmp     w23, w19
    b.ge    selection_inner_done

    // Highlight current position and current min
    adrp    x24, highlight_idx1
    add     x24, x24, :lo12:highlight_idx1
    str     w22, [x24]

    adrp    x24, highlight_idx2
    add     x24, x24, :lo12:highlight_idx2
    str     w23, [x24]

    bl      sort_display_array

    // Delay
    adrp    x1, sort_delay
    add     x1, x1, :lo12:sort_delay
    ldr     w0, [x1]
    bl      delay_ms

    // Compare arr[j] < arr[min_idx]
    ldr     w0, [x20, w23, SXTW 2]
    ldr     w1, [x20, w22, SXTW 2]
    cmp     w0, w1
    b.ge    selection_no_update

    // Update min_idx
    mov     w22, w23

selection_no_update:
    add     w23, w23, 1
    b       selection_inner

selection_inner_done:
    // Swap arr[i] and arr[min_idx] if different
    cmp     w21, w22
    b.eq    selection_no_swap

    ldr     w0, [x20, w21, SXTW 2]
    ldr     w1, [x20, w22, SXTW 2]
    str     w1, [x20, w21, SXTW 2]
    str     w0, [x20, w22, SXTW 2]

    bl      sort_display_array
    mov     w0, 100
    bl      delay_ms

selection_no_swap:
    // Mark element as sorted
    adrp    x23, sorted_up_to
    add     x23, x23, :lo12:sorted_up_to
    str     w21, [x23]

    add     w21, w21, 1
    b       selection_outer

selection_done:
    sub     w0, w19, 1
    adrp    x23, sorted_up_to
    add     x23, x23, :lo12:sorted_up_to
    str     w0, [x23]

    bl      sort_reset_highlights
    bl      sort_display_array

    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 64
    ret

// ============================================================================
// FUNCTION: sort_insertion_interactive
// Interactive insertion sort with visualization
// Parameters: none
// Returns: none
// ============================================================================
    .global sort_insertion_interactive
sort_insertion_interactive:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Check if array is initialized
    adrp    x0, sort_size
    add     x0, x0, :lo12:sort_size
    ldr     w0, [x0]
    cmp     w0, 0
    b.le    sort_insertion_empty

    bl      sort_reset_highlights
    bl      sort_display_array

    adrp    x0, prompt_continue
    add     x0, x0, :lo12:prompt_continue
    bl      printf
    bl      wait_for_enter

    bl      insertion_sort

    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_sorted
    add     x0, x0, :lo12:msg_sorted
    bl      printf
    bl      print_newline

    b       sort_insertion_done

sort_insertion_empty:
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

sort_insertion_done:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: insertion_sort
// Insertion sort with visualization
// Parameters: none (uses global sort_array and sort_size)
// Returns: none
// ============================================================================
    .global insertion_sort
insertion_sort:
    stp     x29, x30, [sp, -64]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]

    // Load size and array
    adrp    x19, sort_size
    add     x19, x19, :lo12:sort_size
    ldr     w19, [x19]

    adrp    x20, sort_array
    add     x20, x20, :lo12:sort_array

    // First element is already sorted
    mov     w0, 0
    adrp    x21, sorted_up_to
    add     x21, x21, :lo12:sorted_up_to
    str     w0, [x21]

    // Loop: i = 1 to n-1
    mov     w21, 1
insertion_outer:
    cmp     w21, w19
    b.ge    insertion_done

    // key = arr[i]
    ldr     w22, [x20, w21, SXTW 2]

    // j = i - 1
    sub     w23, w21, 1

insertion_inner:
    // Check if j < 0
    cmp     w23, 0
    b.lt    insertion_inner_done

    // Highlight key position and comparison position
    adrp    x24, highlight_idx1
    add     x24, x24, :lo12:highlight_idx1
    str     w23, [x24]

    adrp    x24, highlight_idx2
    add     x24, x24, :lo12:highlight_idx2
    add     w0, w23, 1
    str     w0, [x24]

    bl      sort_display_array

    // Delay
    adrp    x1, sort_delay
    add     x1, x1, :lo12:sort_delay
    ldr     w0, [x1]
    bl      delay_ms

    // Check if arr[j] > key
    ldr     w0, [x20, w23, SXTW 2]
    cmp     w0, w22
    b.le    insertion_inner_done

    // Shift arr[j] to arr[j+1]
    add     w1, w23, 1
    str     w0, [x20, w1, SXTW 2]

    sub     w23, w23, 1
    b       insertion_inner

insertion_inner_done:
    // Place key at arr[j+1]
    add     w0, w23, 1
    str     w22, [x20, w0, SXTW 2]

    // Mark up to current as sorted
    adrp    x23, sorted_up_to
    add     x23, x23, :lo12:sorted_up_to
    str     w21, [x23]

    add     w21, w21, 1
    b       insertion_outer

insertion_done:
    sub     w0, w19, 1
    adrp    x23, sorted_up_to
    add     x23, x23, :lo12:sorted_up_to
    str     w0, [x23]

    bl      sort_reset_highlights
    bl      sort_display_array

    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 64
    ret

// ============================================================================
// FUNCTION: sort_reset_highlights
// Resets all highlight indices
// Parameters: none
// Returns: none
// ============================================================================
    .global sort_reset_highlights
sort_reset_highlights:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    mov     w0, -1

    adrp    x1, highlight_idx1
    add     x1, x1, :lo12:highlight_idx1
    str     w0, [x1]

    adrp    x1, highlight_idx2
    add     x1, x1, :lo12:highlight_idx2
    str     w0, [x1]

    adrp    x1, sorted_up_to
    add     x1, x1, :lo12:sorted_up_to
    str     w0, [x1]

    adrp    x1, merge_left
    add     x1, x1, :lo12:merge_left
    str     w0, [x1]

    adrp    x1, merge_right
    add     x1, x1, :lo12:merge_right
    str     w0, [x1]

    adrp    x1, merge_mid
    add     x1, x1, :lo12:merge_mid
    str     w0, [x1]

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: sort_merge_interactive
// Interactive merge sort with visualization
// ============================================================================
    .global sort_merge_interactive
sort_merge_interactive:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Check if array is initialized
    adrp    x0, sort_size
    add     x0, x0, :lo12:sort_size
    ldr     w0, [x0]
    cmp     w0, 0
    b.le    merge_empty

    // Clear screen and display array
    bl      ansi_clear_screen
    bl      sort_reset_highlights
    bl      sort_display_array

    // Position cursor for message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, prompt_continue
    add     x0, x0, :lo12:prompt_continue
    bl      printf

    bl      wait_for_enter

    // Perform merge sort
    bl      sort_merge_sort

    // Display final sorted array
    bl      sort_reset_highlights
    adrp    x0, sort_size
    add     x0, x0, :lo12:sort_size
    ldr     w0, [x0]
    sub     w0, w0, 1
    adrp    x1, sorted_up_to
    add     x1, x1, :lo12:sorted_up_to
    str     w0, [x1]
    bl      sort_display_array

    // Position cursor for message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_sorted
    add     x0, x0, :lo12:msg_sorted
    bl      printf
    bl      print_newline

    b       merge_done

merge_empty:
    bl      ansi_clear_screen
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

merge_done:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: sort_merge_sort
// Bottom-up iterative merge sort
// ============================================================================
sort_merge_sort:
    stp     x29, x30, [sp, -64]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]

    // Load array size
    adrp    x0, sort_size
    add     x0, x0, :lo12:sort_size
    ldr     w19, [x0]                    // w19 = size

    // Load array pointer
    adrp    x22, sort_array              // x22 = array_ptr
    add     x22, x22, :lo12:sort_array

    // Start with merge size of 1, double each iteration
    mov     w20, 1                       // w20 = curr_size

merge_outer_loop:
    cmp     w20, w19
    b.ge    merge_sort_done

    // Start from leftmost subarray
    mov     w21, 0                       // w21 = left_start

merge_inner_loop:
    cmp     w21, w19
    b.ge    merge_next_size

    // Calculate mid and right_end
    add     w23, w21, w20                // w23 = mid
    sub     w23, w23, 1

    // Calculate right_end = min(left_start + 2*curr_size - 1, size - 1)
    lsl     w0, w20, 1
    add     w24, w21, w0                 // w24 = right_end
    sub     w24, w24, 1
    sub     w0, w19, 1
    cmp     w24, w0
    csel    w24, w24, w0, lt

    // Only merge if mid < right_end
    cmp     w23, w24
    b.ge    merge_skip

    // Set merge bounds for visualization
    adrp    x0, merge_left
    add     x0, x0, :lo12:merge_left
    str     w21, [x0]

    adrp    x0, merge_right
    add     x0, x0, :lo12:merge_right
    str     w24, [x0]

    adrp    x0, merge_mid
    add     x0, x0, :lo12:merge_mid
    str     w23, [x0]

    // Perform merge
    mov     w0, w21
    mov     w1, w23
    mov     w2, w24
    bl      sort_merge_arrays

    // Display after merge
    bl      sort_display_array

    // Delay for animation
    adrp    x0, sort_delay
    add     x0, x0, :lo12:sort_delay
    ldr     w0, [x0]
    bl      delay_ms

merge_skip:
    // Move to next pair of subarrays
    lsl     w0, w20, 1
    add     w21, w21, w0
    b       merge_inner_loop

merge_next_size:
    // Double the merge size
    lsl     w20, w20, 1
    b       merge_outer_loop

merge_sort_done:
    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 64
    ret

// ============================================================================
// FUNCTION: sort_merge_arrays
// Merge two sorted subarrays
// Parameters: w0 = left, w1 = mid, w2 = right
// ============================================================================
sort_merge_arrays:
    stp     x29, x30, [sp, -80]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]
    stp     x25, x26, [sp, 64]

    mov     w19, w0                      // w19 = left
    mov     w20, w1                      // w20 = mid
    mov     w21, w2                      // w21 = right

    // Load array pointers
    adrp    x25, sort_array              // x25 = array_ptr
    add     x25, x25, :lo12:sort_array

    adrp    x26, sort_aux_array          // x26 = aux_ptr
    add     x26, x26, :lo12:sort_aux_array

    // Copy to auxiliary array
    mov     w22, w19                     // w22 = i = left
merge_copy_loop:
    cmp     w22, w21                     // Compare i with right
    b.gt    merge_copy_done
    ldr     w0, [x25, w22, SXTW 2]       // Load array[i]
    str     w0, [x26, w22, SXTW 2]       // Store to aux[i]
    add     w22, w22, 1                  // i++
    b       merge_copy_loop

merge_copy_done:
    // Merge back to original array
    mov     w22, w19                     // w22 = i = left (left subarray index)
    add     w23, w20, 1                  // w23 = j = mid + 1 (right subarray index)
    mov     w24, w19                     // w24 = k = left (merged array index)

merge_compare_loop:
    cmp     w22, w20                     // Compare i with mid
    b.gt    merge_copy_right
    cmp     w23, w21                     // Compare j with right
    b.gt    merge_copy_left

    // Compare aux[i] and aux[j]
    ldr     w0, [x26, w22, SXTW 2]       // Load aux[i]
    ldr     w1, [x26, w23, SXTW 2]       // Load aux[j]
    cmp     w0, w1
    b.le    merge_take_left

merge_take_right:
    str     w1, [x25, w24, SXTW 2]       // Store aux[j] to array[k]
    add     w23, w23, 1                  // j++
    add     w24, w24, 1                  // k++
    b       merge_compare_loop

merge_take_left:
    str     w0, [x25, w24, SXTW 2]       // Store aux[i] to array[k]
    add     w22, w22, 1                  // i++
    add     w24, w24, 1                  // k++
    b       merge_compare_loop

merge_copy_left:
    cmp     w22, w20                     // Compare i with mid
    b.gt    merge_complete
    ldr     w0, [x26, w22, SXTW 2]       // Load aux[i]
    str     w0, [x25, w24, SXTW 2]       // Store to array[k]
    add     w22, w22, 1                  // i++
    add     w24, w24, 1                  // k++
    b       merge_copy_left

merge_copy_right:
    cmp     w23, w21                     // Compare j with right
    b.gt    merge_complete
    ldr     w0, [x26, w23, SXTW 2]       // Load aux[j]
    str     w0, [x25, w24, SXTW 2]       // Store to array[k]
    add     w23, w23, 1                  // j++
    add     w24, w24, 1                  // k++
    b       merge_copy_right

merge_complete:
    ldp     x25, x26, [sp, 64]
    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 80
    ret

// ============================================================================
// FUNCTION: sort_quick_interactive
// Interactive quick sort with visualization
// ============================================================================
    .global sort_quick_interactive
sort_quick_interactive:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Check if array is initialized
    adrp    x0, sort_size
    add     x0, x0, :lo12:sort_size
    ldr     w0, [x0]
    cmp     w0, 0
    b.le    quick_empty

    // Clear screen and display array
    bl      ansi_clear_screen
    bl      sort_reset_highlights
    bl      sort_display_array

    // Position cursor for message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, prompt_continue
    add     x0, x0, :lo12:prompt_continue
    bl      printf

    bl      wait_for_enter

    // Perform quick sort
    mov     w0, 0
    adrp    x1, sort_size
    add     x1, x1, :lo12:sort_size
    ldr     w1, [x1]
    sub     w1, w1, 1
    bl      sort_quick_sort

    // Display final sorted array
    bl      sort_reset_highlights
    adrp    x0, sort_size
    add     x0, x0, :lo12:sort_size
    ldr     w0, [x0]
    sub     w0, w0, 1
    adrp    x1, sorted_up_to
    add     x1, x1, :lo12:sorted_up_to
    str     w0, [x1]
    bl      sort_display_array

    // Position cursor for message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_sorted
    add     x0, x0, :lo12:msg_sorted
    bl      printf
    bl      print_newline

    b       quick_done

quick_empty:
    bl      ansi_clear_screen
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor
    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

quick_done:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: sort_quick_sort
// Recursive quick sort
// Parameters: w0 = low, w1 = high
// ============================================================================
sort_quick_sort:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    str     x21, [sp, 32]

    mov     w19, w0                      // w19 = low
    mov     w20, w1                      // w20 = high

    // Base case: if low >= high, return
    cmp     w19, w20                     // Compare low with high
    b.ge    quick_sort_done

    // Partition array and get pivot index
    mov     w0, w19                      // Pass low
    mov     w1, w20                      // Pass high
    bl      sort_quick_partition
    mov     w21, w0                      // w21 = pivot index

    // Recursively sort left partition
    mov     w0, w19                      // Pass low
    sub     w1, w21, 1                   // Pass pivot - 1
    bl      sort_quick_sort

    // Recursively sort right partition
    add     w0, w21, 1                   // Pass pivot + 1
    mov     w1, w20                      // Pass high
    bl      sort_quick_sort

quick_sort_done:
    ldr     x21, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

// ============================================================================
// FUNCTION: sort_quick_partition
// Partition array for quick sort (last element as pivot)
// Parameters: w0 = low, w1 = high
// Returns: w0 = pivot index
// ============================================================================
sort_quick_partition:
    stp     x29, x30, [sp, -64]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    str     x23, [sp, 48]
    str     x24, [sp, 56]

    mov     w19, w0                      // w19 = low
    mov     w20, w1                      // w20 = high

    // Load array pointer
    adrp    x24, sort_array              // x24 = array_ptr
    add     x24, x24, :lo12:sort_array

    // Choose last element as pivot
    ldr     w21, [x24, w20, SXTW 2]      // w21 = pivot_val = array[high]

    // Highlight pivot
    adrp    x0, highlight_idx1
    add     x0, x0, :lo12:highlight_idx1
    str     w20, [x0]                    // Store high as highlight

    // Display with pivot highlighted
    bl      sort_display_array

    // i = low - 1
    sub     w22, w19, 1                  // w22 = i = low - 1

    // j starts from low
    mov     w23, w19                     // w23 = j = low

partition_loop:
    cmp     w23, w20                     // Compare j with high
    b.ge    partition_done

    // Highlight current element
    adrp    x0, highlight_idx2
    add     x0, x0, :lo12:highlight_idx2
    str     w23, [x0]                    // Store j as highlight

    // Compare arr[j] with pivot
    ldr     w0, [x24, w23, SXTW 2]       // Load array[j]
    cmp     w0, w21                      // Compare with pivot_val
    b.gt    partition_no_swap

    // Increment i
    add     w22, w22, 1                  // i++

    // Swap arr[i] and arr[j]
    ldr     w0, [x24, w22, SXTW 2]       // Load array[i]
    ldr     w1, [x24, w23, SXTW 2]       // Load array[j]
    str     w1, [x24, w22, SXTW 2]       // Store array[j] to array[i]
    str     w0, [x24, w23, SXTW 2]       // Store array[i] to array[j]

    // Display after swap
    bl      sort_display_array

    // Delay
    adrp    x0, sort_delay
    add     x0, x0, :lo12:sort_delay
    ldr     w0, [x0]
    bl      delay_ms

partition_no_swap:
    add     w23, w23, 1                  // j++
    b       partition_loop

partition_done:
    // Swap arr[i+1] with arr[high] (pivot)
    add     w22, w22, 1                  // i++
    ldr     w0, [x24, w22, SXTW 2]       // Load array[i]
    ldr     w1, [x24, w20, SXTW 2]       // Load array[high]
    str     w1, [x24, w22, SXTW 2]       // Store array[high] to array[i]
    str     w0, [x24, w20, SXTW 2]       // Store array[i] to array[high]

    // Display final pivot position
    adrp    x0, highlight_idx2
    add     x0, x0, :lo12:highlight_idx2
    mov     w1, -1
    str     w1, [x0]
    bl      sort_display_array

    // Delay
    adrp    x0, sort_delay
    add     x0, x0, :lo12:sort_delay
    ldr     w0, [x0]
    bl      delay_ms

    // Return pivot index
    mov     w0, w22                      // Return i

    ldr     x24, [sp, 56]
    ldr     x23, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 64
    ret
