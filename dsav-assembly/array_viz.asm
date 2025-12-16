// ============================================================================
// array_viz.asm - Array Visualization Module
// ============================================================================
// Implements array data structure with visualization capabilities:
// - Initialize array with random or user-specified values
// - Display array with indices and values
// - Highlight specific elements (for algorithm visualization)
// - Swap elements with visual feedback
// - Get/set operations
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// ----------------------------------------------------------------------------
// Array Data Storage
// ----------------------------------------------------------------------------
array_max_size = 10                         // Maximum array capacity
array_data:     .skip array_max_size * 4    // Array storage (10 x 4 bytes)
array_count:    .word 0                     // Current number of elements

// Highlight tracking for visualization
highlight_index: .word -1                   // Index to highlight (-1 = none)
highlight_color: .word 33                   // Color for highlighting (yellow)

// ----------------------------------------------------------------------------
// UI Strings
// ----------------------------------------------------------------------------
array_title:        .string "ARRAY VISUALIZATION"
array_menu_title:   .string "ARRAY OPERATIONS MENU"

menu_opt_1:         .string "[1] Initialize with Random Values"
menu_opt_2:         .string "[2] Initialize with User Input"
menu_opt_3:         .string "[3] Display Array"
menu_opt_4:         .string "[4] Set Element at Index"
menu_opt_5:         .string "[5] Get Element at Index"
menu_opt_6:         .string "[6] Swap Two Elements"
menu_opt_7:         .string "[7] Clear Array"
menu_opt_0:         .string "[0] Back to Main Menu"

menu_prompt:        .string "Enter your choice: "
prompt_count:       .string "Enter number of elements (1-%d): "
prompt_value:       .string "Enter value for element %d: "
prompt_index:       .string "Enter index (0-%d): "
prompt_index1:      .string "Enter first index (0-%d): "
prompt_index2:      .string "Enter second index (0-%d): "
prompt_new_value:   .string "Enter new value: "

msg_empty:          .string "\x1b[33mArray is empty!\x1b[0m"
msg_initialized:    .string "\x1b[32mArray initialized with %d elements.\x1b[0m"
msg_value_at:       .string "Value at index %d: \x1b[36m%d\x1b[0m"
msg_swapped:        .string "\x1b[32mSwapped elements at indices %d and %d\x1b[0m"
msg_cleared:        .string "\x1b[32mArray cleared.\x1b[0m"
msg_updated:        .string "\x1b[32mElement at index %d updated to %d\x1b[0m"

label_index:        .string "Index:"
label_value:        .string "Value:"
label_size:         .string "Size: %d/%d"

    .text
    .balign 4

// ============================================================================
// FUNCTION: array_menu
// Main menu for array operations
// Parameters: none
// Returns: none
// ============================================================================
    .global array_menu
array_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

array_menu_loop:
    // Clear screen and display menu
    bl      ansi_clear_screen
    bl      display_array_menu

    // Get user choice
    mov     w0, 0                            // min
    mov     w1, 7                            // max
    bl      read_int_range

    // Dispatch based on choice
    cmp     w0, 0
    b.eq    array_menu_exit

    cmp     w0, 1
    b.eq    array_menu_init_random

    cmp     w0, 2
    b.eq    array_menu_init_user

    cmp     w0, 3
    b.eq    array_menu_display

    cmp     w0, 4
    b.eq    array_menu_set

    cmp     w0, 5
    b.eq    array_menu_get

    cmp     w0, 6
    b.eq    array_menu_swap

    cmp     w0, 7
    b.eq    array_menu_clear

    b       array_menu_loop

array_menu_init_random:
    bl      array_init_random
    bl      wait_for_enter
    b       array_menu_loop

array_menu_init_user:
    bl      array_init_user
    bl      wait_for_enter
    b       array_menu_loop

array_menu_display:
    bl      array_display
    bl      wait_for_enter
    b       array_menu_loop

array_menu_set:
    bl      array_set_interactive
    bl      wait_for_enter
    b       array_menu_loop

array_menu_get:
    bl      array_get_interactive
    bl      wait_for_enter
    b       array_menu_loop

array_menu_swap:
    bl      array_swap_interactive
    bl      wait_for_enter
    b       array_menu_loop

array_menu_clear:
    bl      array_clear

    // Position cursor for status message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_cleared
    add     x0, x0, :lo12:msg_cleared
    bl      printf
    bl      print_newline
    bl      wait_for_enter
    b       array_menu_loop

array_menu_exit:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: display_array_menu
// Displays the array operations menu
// Parameters: none
// Returns: none
// ============================================================================
    .global display_array_menu
display_array_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Draw menu box
    mov     w0, 3                            // row
    mov     w1, 10                           // column
    mov     w2, 60                           // width
    mov     w3, 16                           // height
    mov     w4, 1                            // double-line style
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 12
    bl      ansi_move_cursor
    adrp    x0, array_menu_title
    add     x0, x0, :lo12:array_menu_title
    mov     w1, 56
    bl      print_centered

    // Print separator
    mov     w0, 5
    mov     w1, 10
    mov     w2, 60
    mov     w3, 1
    bl      draw_horizontal_border_top

    // Print menu options
    mov     w0, 7
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, menu_opt_1
    add     x0, x0, :lo12:menu_opt_1
    bl      printf

    mov     w0, 8
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, menu_opt_2
    add     x0, x0, :lo12:menu_opt_2
    bl      printf

    mov     w0, 9
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, menu_opt_3
    add     x0, x0, :lo12:menu_opt_3
    bl      printf

    mov     w0, 10
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, menu_opt_4
    add     x0, x0, :lo12:menu_opt_4
    bl      printf

    mov     w0, 11
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, menu_opt_5
    add     x0, x0, :lo12:menu_opt_5
    bl      printf

    mov     w0, 12
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, menu_opt_6
    add     x0, x0, :lo12:menu_opt_6
    bl      printf

    mov     w0, 13
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, menu_opt_7
    add     x0, x0, :lo12:menu_opt_7
    bl      printf

    mov     w0, 14
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, menu_opt_0
    add     x0, x0, :lo12:menu_opt_0
    bl      printf

    // Print separator
    mov     w0, 16
    mov     w1, 10
    mov     w2, 60
    mov     w3, 1
    bl      draw_horizontal_border_top

    // Position cursor for input
    mov     w0, 17
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, menu_prompt
    add     x0, x0, :lo12:menu_prompt
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: array_init_random
// Initializes array with random values
// Parameters: none
// Returns: none
// ============================================================================
    .global array_init_random
array_init_random:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    bl      ansi_clear_screen

    // Prompt for array size
    adrp    x0, prompt_count
    add     x0, x0, :lo12:prompt_count
    mov     w1, array_max_size
    bl      printf

    mov     w0, 1
    mov     w1, array_max_size
    bl      read_int_range
    mov     w19, w0                          // Save count

    // Store count
    adrp    x20, array_count
    add     x20, x20, :lo12:array_count
    str     w19, [x20]

    // Fill with random values
    adrp    x20, array_data
    add     x20, x20, :lo12:array_data
    mov     w21, 0                           // index

array_init_random_loop:
    cmp     w21, w19
    b.ge    array_init_random_done

    // Generate random value (0-99)
    mov     w0, 100
    bl      get_random

    // Store in array
    str     w0, [x20, w21, SXTW 2]

    add     w21, w21, 1
    b       array_init_random_loop

array_init_random_done:
    // Display the array first
    bl      array_display

    // Position cursor for success message
    mov     w0, 15
    mov     w1, 1
    bl      ansi_move_cursor

    // Print success message
    adrp    x0, msg_initialized
    add     x0, x0, :lo12:msg_initialized
    mov     w1, w19
    bl      printf
    bl      print_newline

    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: array_init_user
// Initializes array with user-provided values
// Parameters: none
// Returns: none
// ============================================================================
    .global array_init_user
array_init_user:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    bl      ansi_clear_screen

    // Prompt for array size
    adrp    x0, prompt_count
    add     x0, x0, :lo12:prompt_count
    mov     w1, array_max_size
    bl      printf

    mov     w0, 1
    mov     w1, array_max_size
    bl      read_int_range
    mov     w19, w0                          // Save count

    // Store count
    adrp    x20, array_count
    add     x20, x20, :lo12:array_count
    str     w19, [x20]

    // Get values from user
    adrp    x20, array_data
    add     x20, x20, :lo12:array_data
    mov     w21, 0                           // index

array_init_user_loop:
    cmp     w21, w19
    b.ge    array_init_user_done

    // Prompt for value
    adrp    x0, prompt_value
    add     x0, x0, :lo12:prompt_value
    mov     w1, w21
    bl      printf

    bl      read_int
    mov     w22, w0                          // Save value

    // Store in array
    str     w22, [x20, w21, SXTW 2]

    add     w21, w21, 1
    b       array_init_user_loop

array_init_user_done:
    // Display the array first
    bl      array_display

    // Position cursor for success message
    mov     w0, 15
    mov     w1, 1
    bl      ansi_move_cursor

    // Print success message
    adrp    x0, msg_initialized
    add     x0, x0, :lo12:msg_initialized
    mov     w1, w19
    bl      printf
    bl      print_newline

    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: array_display
// Displays the array with visual formatting
// Parameters: none
// Returns: none
// ============================================================================
    .global array_display
array_display:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]

    bl      ansi_clear_screen

    // Load array count
    adrp    x19, array_count
    add     x19, x19, :lo12:array_count
    ldr     w19, [x19]

    // Check if empty
    cmp     w19, 0
    b.le    array_display_empty

    // Draw box
    mov     w0, 3
    mov     w1, 2
    mov     w2, 80
    mov     w3, 10
    mov     w4, 0                            // single-line
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 4
    bl      ansi_move_cursor
    adrp    x0, array_title
    add     x0, x0, :lo12:array_title
    mov     w1, 76
    bl      print_centered

    // Print "Index:" label
    mov     w0, 6
    mov     w1, 10
    bl      ansi_move_cursor
    adrp    x0, label_index
    add     x0, x0, :lo12:label_index
    bl      printf

    // Print indices
    adrp    x20, array_data
    add     x20, x20, :lo12:array_data
    mov     w21, 0                           // index counter
    mov     w22, 20                          // column position

array_display_indices:
    cmp     w21, w19
    b.ge    array_display_values_start

    mov     w0, 6
    mov     w1, w22
    bl      ansi_move_cursor

    adrp    x0, .Lindex_fmt
    add     x0, x0, :lo12:.Lindex_fmt
    mov     w1, w21
    bl      printf

    add     w22, w22, 6                      // Next column
    add     w21, w21, 1
    b       array_display_indices

array_display_values_start:
    // Print "Value:" label
    mov     w0, 7
    mov     w1, 10
    bl      ansi_move_cursor
    adrp    x0, label_value
    add     x0, x0, :lo12:label_value
    bl      printf

    // Print values
    mov     w21, 0                           // index counter
    mov     w22, 20                          // column position

    // Load highlight index
    adrp    x23, highlight_index
    add     x23, x23, :lo12:highlight_index
    ldr     w23, [x23]

array_display_values:
    cmp     w21, w19
    b.ge    array_display_footer

    mov     w0, 7
    mov     w1, w22
    bl      ansi_move_cursor

    // Check if this index should be highlighted
    cmp     w21, w23
    b.ne    array_display_normal

    // Highlighted value (yellow background)
    mov     w0, 43                           // yellow background
    bl      ansi_set_color_bg

array_display_normal:
    // Print value
    ldr     w1, [x20, w21, SXTW 2]
    adrp    x0, .Lvalue_fmt
    add     x0, x0, :lo12:.Lvalue_fmt
    bl      printf

    // Reset color if was highlighted
    cmp     w21, w23
    b.ne    array_display_next
    bl      ansi_reset_attributes

array_display_next:
    add     w22, w22, 6                      // Next column
    add     w21, w21, 1
    b       array_display_values

array_display_footer:
    // Print size info
    mov     w0, 9
    mov     w1, 10
    bl      ansi_move_cursor
    adrp    x0, label_size
    add     x0, x0, :lo12:label_size
    mov     w1, w19
    mov     w2, array_max_size
    bl      printf

    bl      print_newline
    b       array_display_done

array_display_empty:
    // Position cursor for error message
    mov     w0, 10
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

array_display_done:
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

    .section .rodata
.Lindex_fmt: .string "%2d    "
.Lvalue_fmt: .string "%4d  "
    .text

// ============================================================================
// FUNCTION: array_get_interactive
// Interactive get element operation
// Parameters: none
// Returns: none
// ============================================================================
    .global array_get_interactive
array_get_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    // Load array count
    adrp    x19, array_count
    add     x19, x19, :lo12:array_count
    ldr     w19, [x19]

    cmp     w19, 0
    b.le    array_get_empty

    bl      ansi_clear_screen

    // Prompt for index
    adrp    x0, prompt_index
    add     x0, x0, :lo12:prompt_index
    sub     w1, w19, 1
    bl      printf

    mov     w0, 0
    sub     w1, w19, 1
    bl      read_int_range
    mov     w20, w0                          // Save index

    // Get value
    adrp    x0, array_data
    add     x0, x0, :lo12:array_data
    ldr     w1, [x0, w20, SXTW 2]

    // Position cursor for result message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    // Print result
    adrp    x0, msg_value_at
    add     x0, x0, :lo12:msg_value_at
    mov     w2, w1
    mov     w1, w20
    bl      printf
    bl      print_newline

    b       array_get_done

array_get_empty:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

array_get_done:
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: array_set_interactive
// Interactive set element operation
// Parameters: none
// Returns: none
// ============================================================================
    .global array_set_interactive
array_set_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    // Load array count
    adrp    x19, array_count
    add     x19, x19, :lo12:array_count
    ldr     w19, [x19]

    cmp     w19, 0
    b.le    array_set_empty

    bl      ansi_clear_screen

    // Prompt for index
    adrp    x0, prompt_index
    add     x0, x0, :lo12:prompt_index
    sub     w1, w19, 1
    bl      printf

    mov     w0, 0
    sub     w1, w19, 1
    bl      read_int_range
    mov     w20, w0                          // Save index

    // Prompt for new value
    adrp    x0, prompt_new_value
    add     x0, x0, :lo12:prompt_new_value
    bl      printf

    bl      read_int
    mov     w21, w0                          // Save new value

    // Set value
    adrp    x22, array_data
    add     x22, x22, :lo12:array_data
    str     w21, [x22, w20, SXTW 2]

    // Display updated array
    bl      array_display

    // Position cursor for confirmation message
    mov     w0, 15
    mov     w1, 1
    bl      ansi_move_cursor

    // Print confirmation
    adrp    x0, msg_updated
    add     x0, x0, :lo12:msg_updated
    mov     w1, w20
    mov     w2, w21
    bl      printf
    bl      print_newline

    b       array_set_done

array_set_empty:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

array_set_done:
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: array_swap_interactive
// Interactive swap operation
// Parameters: none
// Returns: none
// ============================================================================
    .global array_swap_interactive
array_swap_interactive:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]

    // Load array count
    adrp    x19, array_count
    add     x19, x19, :lo12:array_count
    ldr     w19, [x19]

    cmp     w19, 2
    b.lt    array_swap_too_small

    bl      ansi_clear_screen

    // Get first index
    adrp    x0, prompt_index1
    add     x0, x0, :lo12:prompt_index1
    sub     w1, w19, 1
    bl      printf

    mov     w0, 0
    sub     w1, w19, 1
    bl      read_int_range
    mov     w20, w0                          // Save index1

    // Get second index
    adrp    x0, prompt_index2
    add     x0, x0, :lo12:prompt_index2
    sub     w1, w19, 1
    bl      printf

    mov     w0, 0
    sub     w1, w19, 1
    bl      read_int_range
    mov     w21, w0                          // Save index2

    // Perform swap
    adrp    x22, array_data
    add     x22, x22, :lo12:array_data

    ldr     w23, [x22, w20, SXTW 2]          // temp = arr[i]
    ldr     w24, [x22, w21, SXTW 2]          // arr[j]
    str     w24, [x22, w20, SXTW 2]          // arr[i] = arr[j]
    str     w23, [x22, w21, SXTW 2]          // arr[j] = temp

    // Display updated array
    bl      array_display

    // Position cursor for confirmation message
    mov     w0, 15
    mov     w1, 1
    bl      ansi_move_cursor

    // Print confirmation
    adrp    x0, msg_swapped
    add     x0, x0, :lo12:msg_swapped
    mov     w1, w20
    mov     w2, w21
    bl      printf
    bl      print_newline

    b       array_swap_done

array_swap_too_small:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, .Lswap_err
    add     x0, x0, :lo12:.Lswap_err
    bl      printf
    bl      print_newline

array_swap_done:
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

    .section .rodata
.Lswap_err: .string "\x1b[33mNeed at least 2 elements to swap!\x1b[0m"
    .text

// ============================================================================
// FUNCTION: array_clear
// Clears the array (sets count to 0)
// Parameters: none
// Returns: none
// ============================================================================
    .global array_clear
array_clear:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, array_count
    add     x0, x0, :lo12:array_count
    mov     w1, 0
    str     w1, [x0]

    // Clear highlight
    adrp    x0, highlight_index
    add     x0, x0, :lo12:highlight_index
    mov     w1, -1
    str     w1, [x0]

    ldp     x29, x30, [sp], 16
    ret
