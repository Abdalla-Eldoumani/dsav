// ============================================================================
// stack_viz.asm - Stack Visualization Module
// ============================================================================
// Implements stack data structure (LIFO - Last In First Out) with visualization:
// - Push/Pop operations with visual feedback
// - Peek at top element
// - Vertical visualization (grows upward)
// - Overflow/underflow error handling
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// ----------------------------------------------------------------------------
// Stack Data Storage
// ----------------------------------------------------------------------------
stack_max_size = 8                          // Maximum stack capacity
stack_data:     .skip stack_max_size * 4    // Stack storage (8 x 4 bytes)
stack_top:      .word -1                    // Top index (-1 = empty)

// ----------------------------------------------------------------------------
// UI Strings
// ----------------------------------------------------------------------------
stack_title:        .string "STACK VISUALIZATION (LIFO)"
stack_menu_title:   .string "STACK OPERATIONS MENU"

menu_opt_1:         .string "[1] Push Value"
menu_opt_2:         .string "[2] Pop Value"
menu_opt_3:         .string "[3] Peek Top"
menu_opt_4:         .string "[4] Display Stack"
menu_opt_5:         .string "[5] Clear Stack"
menu_opt_0:         .string "[0] Back to Main Menu"

menu_prompt:        .string "Enter your choice: "
prompt_value:       .string "Enter value to push: "

msg_empty:          .string "\x1b[33mStack is empty!\x1b[0m"
msg_full:           .string "\x1b[31mStack Overflow! Cannot push.\x1b[0m"
msg_pushed:         .string "\x1b[32mPushed %d onto stack.\x1b[0m"
msg_popped:         .string "\x1b[32mPopped %d from stack.\x1b[0m"
msg_peek:           .string "Top element: \x1b[36m%d\x1b[0m"
msg_cleared:        .string "\x1b[32mStack cleared.\x1b[0m"

label_top:          .string "TOP"
label_bottom:       .string "BOTTOM"
label_size:         .string "Size: %d/%d"

    .text
    .balign 4

// ============================================================================
// FUNCTION: stack_menu
// Main menu for stack operations
// Parameters: none
// Returns: none
// ============================================================================
    .global stack_menu
stack_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

stack_menu_loop:
    // Clear screen and display menu
    bl      ansi_clear_screen
    bl      display_stack_menu

    // Get user choice
    mov     w0, 0                            // min
    mov     w1, 5                            // max
    bl      read_int_range

    // Dispatch based on choice
    cmp     w0, 0
    b.eq    stack_menu_exit

    cmp     w0, 1
    b.eq    stack_menu_push

    cmp     w0, 2
    b.eq    stack_menu_pop

    cmp     w0, 3
    b.eq    stack_menu_peek

    cmp     w0, 4
    b.eq    stack_menu_display

    cmp     w0, 5
    b.eq    stack_menu_clear

    b       stack_menu_loop

stack_menu_push:
    bl      stack_push_interactive
    bl      wait_for_enter
    b       stack_menu_loop

stack_menu_pop:
    bl      stack_pop_interactive
    bl      wait_for_enter
    b       stack_menu_loop

stack_menu_peek:
    bl      stack_peek_interactive
    bl      wait_for_enter
    b       stack_menu_loop

stack_menu_display:
    bl      stack_display
    bl      wait_for_enter
    b       stack_menu_loop

stack_menu_clear:
    bl      stack_clear

    // Position cursor for status message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_cleared
    add     x0, x0, :lo12:msg_cleared
    bl      printf
    bl      print_newline
    bl      wait_for_enter
    b       stack_menu_loop

stack_menu_exit:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: display_stack_menu
// Displays the stack operations menu
// Parameters: none
// Returns: none
// ============================================================================
    .global display_stack_menu
display_stack_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Draw menu box
    mov     w0, 3                            // row
    mov     w1, 15                           // column
    mov     w2, 50                           // width
    mov     w3, 14                           // height
    mov     w4, 1                            // double-line style
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 17
    bl      ansi_move_cursor
    adrp    x0, stack_menu_title
    add     x0, x0, :lo12:stack_menu_title
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
    adrp    x0, menu_opt_0
    add     x0, x0, :lo12:menu_opt_0
    bl      printf

    // Print separator
    mov     w0, 14
    mov     w1, 15
    mov     w2, 50
    mov     w3, 1
    bl      draw_horizontal_border_top

    // Position cursor for input
    mov     w0, 15
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_prompt
    add     x0, x0, :lo12:menu_prompt
    bl      printf

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: stack_push_interactive
// Interactive push operation
// Parameters: none
// Returns: none
// ============================================================================
    .global stack_push_interactive
stack_push_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    bl      ansi_clear_screen

    // Check if stack is full
    adrp    x19, stack_top
    add     x19, x19, :lo12:stack_top
    ldr     w19, [x19]
    cmp     w19, stack_max_size - 1
    b.ge    stack_push_overflow

    // Prompt for value
    adrp    x0, prompt_value
    add     x0, x0, :lo12:prompt_value
    bl      printf

    bl      read_int
    mov     w19, w0                          // Save value

    // Push value
    mov     w0, w19
    bl      stack_push

    // Display stack
    bl      stack_display

    // Position cursor for success message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    // Print success message
    adrp    x0, msg_pushed
    add     x0, x0, :lo12:msg_pushed
    mov     w1, w19
    bl      printf
    bl      print_newline

    b       stack_push_done

stack_push_overflow:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_full
    add     x0, x0, :lo12:msg_full
    bl      printf
    bl      print_newline

stack_push_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: stack_pop_interactive
// Interactive pop operation
// Parameters: none
// Returns: none
// ============================================================================
    .global stack_pop_interactive
stack_pop_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    bl      ansi_clear_screen

    // Check if stack is empty
    bl      stack_is_empty
    cmp     w0, 1
    b.eq    stack_pop_underflow

    // Pop value
    bl      stack_pop
    mov     w19, w0                          // Save popped value

    // Display stack
    bl      stack_display

    // Position cursor for success message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    // Print success message
    adrp    x0, msg_popped
    add     x0, x0, :lo12:msg_popped
    mov     w1, w19
    bl      printf
    bl      print_newline

    b       stack_pop_done

stack_pop_underflow:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

stack_pop_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: stack_peek_interactive
// Interactive peek operation
// Parameters: none
// Returns: none
// ============================================================================
    .global stack_peek_interactive
stack_peek_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    bl      ansi_clear_screen

    // Check if stack is empty
    bl      stack_is_empty
    cmp     w0, 1
    b.eq    stack_peek_empty

    // Peek at top
    bl      stack_peek
    mov     w19, w0                          // Save peeked value

    // Display stack
    bl      stack_display

    // Position cursor for result message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    // Print result
    adrp    x0, msg_peek
    add     x0, x0, :lo12:msg_peek
    mov     w1, w19
    bl      printf
    bl      print_newline

    b       stack_peek_done

stack_peek_empty:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

stack_peek_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: stack_push
// Pushes a value onto the stack (internal function)
// Parameters:
//   w0 = value to push
// Returns:
//   w0 = 1 on success, 0 on overflow
// ============================================================================
    .global stack_push
stack_push:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    mov     w19, w0                          // Save value

    // Load stack top
    adrp    x20, stack_top
    add     x20, x20, :lo12:stack_top
    ldr     w21, [x20]

    // Check for overflow
    cmp     w21, stack_max_size - 1
    b.ge    stack_push_fail

    // Increment top
    add     w21, w21, 1
    str     w21, [x20]

    // Store value
    adrp    x20, stack_data
    add     x20, x20, :lo12:stack_data
    str     w19, [x20, w21, SXTW 2]

    mov     w0, 1                            // Success
    b       stack_push_ret

stack_push_fail:
    mov     w0, 0                            // Failure

stack_push_ret:
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: stack_pop
// Pops a value from the stack (internal function)
// Parameters: none
// Returns:
//   w0 = popped value (or 0 if empty)
// ============================================================================
    .global stack_pop
stack_pop:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    // Load stack top
    adrp    x19, stack_top
    add     x19, x19, :lo12:stack_top
    ldr     w20, [x19]

    // Check for underflow
    cmp     w20, 0
    b.lt    stack_pop_fail

    // Get value
    adrp    x21, stack_data
    add     x21, x21, :lo12:stack_data
    ldr     w0, [x21, w20, SXTW 2]

    // Decrement top
    sub     w20, w20, 1
    str     w20, [x19]

    b       stack_pop_ret

stack_pop_fail:
    mov     w0, 0                            // Return 0 if empty

stack_pop_ret:
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: stack_peek
// Views top element without removing it (internal function)
// Parameters: none
// Returns:
//   w0 = top value (or 0 if empty)
// ============================================================================
    .global stack_peek
stack_peek:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    // Load stack top
    adrp    x19, stack_top
    add     x19, x19, :lo12:stack_top
    ldr     w20, [x19]

    // Check if empty
    cmp     w20, 0
    b.lt    stack_peek_fail

    // Get value (don't remove)
    adrp    x21, stack_data
    add     x21, x21, :lo12:stack_data
    ldr     w0, [x21, w20, SXTW 2]

    b       stack_peek_ret

stack_peek_fail:
    mov     w0, 0                            // Return 0 if empty

stack_peek_ret:
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: stack_display
// Displays the stack with vertical visualization
// Parameters: none
// Returns: none
// ============================================================================
    .global stack_display
stack_display:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]

    bl      ansi_clear_screen

    // Load stack top
    adrp    x19, stack_top
    add     x19, x19, :lo12:stack_top
    ldr     w19, [x19]                       // w19 = top index

    // Check if empty
    cmp     w19, 0
    b.lt    stack_display_empty

    // Draw box
    mov     w0, 3
    mov     w1, 20
    mov     w2, 40
    mov     w3, 29
    mov     w4, 0                            // single-line
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 22
    bl      ansi_move_cursor
    adrp    x0, stack_title
    add     x0, x0, :lo12:stack_title
    mov     w1, 36
    bl      print_centered

    // Draw separator
    mov     w0, 5
    mov     w1, 20
    mov     w2, 40
    mov     w3, 0
    bl      draw_horizontal_border_top

    // Display stack elements (from top to bottom)
    adrp    x20, stack_data
    add     x20, x20, :lo12:stack_data

    mov     w21, w19                         // Start from top index
    mov     w22, 7                           // Start row position

stack_display_loop:
    cmp     w21, 0
    b.lt    stack_display_footer

    // Position cursor for element box
    mov     w0, w22
    mov     w1, 28
    bl      ansi_move_cursor

    // Draw element box top
    adrp    x0, .Lbox_top
    add     x0, x0, :lo12:.Lbox_top
    bl      printf

    // Next row - print value
    add     w22, w22, 1
    mov     w0, w22
    mov     w1, 28
    bl      ansi_move_cursor

    adrp    x0, .Lbox_mid
    add     x0, x0, :lo12:.Lbox_mid
    ldr     w1, [x20, w21, SXTW 2]
    bl      printf

    // Print TOP indicator if this is the top element
    cmp     w21, w19
    b.ne    stack_display_no_top_label
    adrp    x0, .Ltop_arrow
    add     x0, x0, :lo12:.Ltop_arrow
    bl      printf

stack_display_no_top_label:
    // Next row - print bottom of element
    add     w22, w22, 1
    mov     w0, w22
    mov     w1, 28
    bl      ansi_move_cursor

    adrp    x0, .Lbox_bot
    add     x0, x0, :lo12:.Lbox_bot
    bl      printf

    add     w22, w22, 1
    sub     w21, w21, 1
    b       stack_display_loop

stack_display_footer:
    // Print BOTTOM label
    add     w22, w22, 1
    mov     w0, w22
    mov     w1, 30
    bl      ansi_move_cursor
    adrp    x0, .Lbottom_label
    add     x0, x0, :lo12:.Lbottom_label
    bl      printf

    // Print size info
    mov     w0, 30
    mov     w1, 25
    bl      ansi_move_cursor
    adrp    x0, label_size
    add     x0, x0, :lo12:label_size
    add     w1, w19, 1                       // size = top + 1
    mov     w2, stack_max_size
    bl      printf

    bl      print_newline
    b       stack_display_done

stack_display_empty:
    // Position cursor for error message
    mov     w0, 10
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

stack_display_done:
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

    .section .rodata
.Lbox_top:      .string "┌───────────┐"
.Lbox_mid:      .string "│   %4d    │"
.Lbox_bot:      .string "└───────────┘"
.Ltop_arrow:    .string " <- TOP"
.Lbottom_label: .string "[BOTTOM]"
    .text

// ============================================================================
// FUNCTION: stack_is_empty
// Checks if stack is empty
// Parameters: none
// Returns:
//   w0 = 1 if empty, 0 otherwise
// ============================================================================
    .global stack_is_empty
stack_is_empty:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, stack_top
    add     x0, x0, :lo12:stack_top
    ldr     w0, [x0]

    cmp     w0, 0
    b.lt    stack_empty_yes
    mov     w0, 0                            // Not empty
    b       stack_empty_ret

stack_empty_yes:
    mov     w0, 1                            // Empty

stack_empty_ret:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: stack_is_full
// Checks if stack is full
// Parameters: none
// Returns:
//   w0 = 1 if full, 0 otherwise
// ============================================================================
    .global stack_is_full
stack_is_full:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, stack_top
    add     x0, x0, :lo12:stack_top
    ldr     w0, [x0]

    cmp     w0, stack_max_size - 1
    b.ge    stack_full_yes
    mov     w0, 0                            // Not full
    b       stack_full_ret

stack_full_yes:
    mov     w0, 1                            // Full

stack_full_ret:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: stack_clear
// Clears the stack (resets to empty state)
// Parameters: none
// Returns: none
// ============================================================================
    .global stack_clear
stack_clear:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, stack_top
    add     x0, x0, :lo12:stack_top
    mov     w1, -1
    str     w1, [x0]

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// C++ INTERFACE FUNCTIONS (for asm-linked version)
// These functions expose internal state for visualization
// ============================================================================

// ============================================================================
// FUNCTION: stack_get_data
// Get pointer to internal stack array (for C++ visualization)
// Parameters: none
// Returns: x0 = pointer to stack_data array
// ============================================================================
    .global stack_get_data
stack_get_data:
    adrp    x0, stack_data
    add     x0, x0, :lo12:stack_data
    ret

// ============================================================================
// FUNCTION: stack_get_top
// Get current top index (for C++ visualization)
// Parameters: none
// Returns: w0 = top index (-1 if empty)
// ============================================================================
    .global stack_get_top
stack_get_top:
    adrp    x0, stack_top
    add     x0, x0, :lo12:stack_top
    ldr     w0, [x0]
    ret

// ============================================================================
// FUNCTION: stack_get_capacity
// Get stack capacity (for C++ visualization)
// Parameters: none
// Returns: w0 = maximum capacity
// ============================================================================
    .global stack_get_capacity
stack_get_capacity:
    mov     w0, stack_max_size
    ret
