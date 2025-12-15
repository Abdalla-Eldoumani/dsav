// ============================================================================
// queue_viz.asm - Queue Visualization Module
// ============================================================================
// Implements queue data structure (FIFO - First In First Out) with visualization:
// - Enqueue/Dequeue operations with visual feedback
// - Circular queue implementation
// - Horizontal visualization
// - Overflow/underflow error handling
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// ----------------------------------------------------------------------------
// Queue Data Storage (Circular Queue)
// ----------------------------------------------------------------------------
queue_max_size = 8                          // Maximum queue capacity
queue_data:     .skip queue_max_size * 4    // Queue storage (8 x 4 bytes)
queue_front:    .word 0                     // Front index
queue_rear:     .word -1                    // Rear index (-1 = empty)
queue_count:    .word 0                     // Current element count

// ----------------------------------------------------------------------------
// UI Strings
// ----------------------------------------------------------------------------
queue_title:        .string "QUEUE VISUALIZATION (FIFO)"
queue_menu_title:   .string "QUEUE OPERATIONS MENU"

menu_opt_1:         .string "[1] Enqueue Value"
menu_opt_2:         .string "[2] Dequeue Value"
menu_opt_3:         .string "[3] Peek Front"
menu_opt_4:         .string "[4] Display Queue"
menu_opt_5:         .string "[5] Clear Queue"
menu_opt_0:         .string "[0] Back to Main Menu"

menu_prompt:        .string "Enter your choice: "
prompt_value:       .string "Enter value to enqueue: "

msg_empty:          .string "\x1b[33mQueue is empty!\x1b[0m"
msg_full:           .string "\x1b[31mQueue Overflow! Cannot enqueue.\x1b[0m"
msg_enqueued:       .string "\x1b[32mEnqueued %d to queue.\x1b[0m"
msg_dequeued:       .string "\x1b[32mDequeued %d from queue.\x1b[0m"
msg_peek:           .string "Front element: \x1b[36m%d\x1b[0m"
msg_cleared:        .string "\x1b[32mQueue cleared.\x1b[0m"

label_front:        .string "FRONT"
label_rear:         .string "REAR"
label_count:        .string "Count: %d/%d"

    .text
    .balign 4

// ============================================================================
// FUNCTION: queue_menu
// Main menu for queue operations
// Parameters: none
// Returns: none
// ============================================================================
    .global queue_menu
queue_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

queue_menu_loop:
    // Clear screen and display menu
    bl      ansi_clear_screen
    bl      display_queue_menu

    // Get user choice
    mov     w0, 0                            // min
    mov     w1, 5                            // max
    bl      read_int_range

    // Dispatch based on choice
    cmp     w0, 0
    b.eq    queue_menu_exit

    cmp     w0, 1
    b.eq    queue_menu_enqueue

    cmp     w0, 2
    b.eq    queue_menu_dequeue

    cmp     w0, 3
    b.eq    queue_menu_peek

    cmp     w0, 4
    b.eq    queue_menu_display

    cmp     w0, 5
    b.eq    queue_menu_clear

    b       queue_menu_loop

queue_menu_enqueue:
    bl      queue_enqueue_interactive
    bl      wait_for_enter
    b       queue_menu_loop

queue_menu_dequeue:
    bl      queue_dequeue_interactive
    bl      wait_for_enter
    b       queue_menu_loop

queue_menu_peek:
    bl      queue_peek_interactive
    bl      wait_for_enter
    b       queue_menu_loop

queue_menu_display:
    bl      queue_display
    bl      wait_for_enter
    b       queue_menu_loop

queue_menu_clear:
    bl      queue_clear

    // Position cursor for status message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_cleared
    add     x0, x0, :lo12:msg_cleared
    bl      printf
    bl      print_newline
    bl      wait_for_enter
    b       queue_menu_loop

queue_menu_exit:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: display_queue_menu
// Displays the queue operations menu
// Parameters: none
// Returns: none
// ============================================================================
    .global display_queue_menu
display_queue_menu:
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
    adrp    x0, queue_menu_title
    add     x0, x0, :lo12:queue_menu_title
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
// FUNCTION: queue_enqueue_interactive
// Interactive enqueue operation
// Parameters: none
// Returns: none
// ============================================================================
    .global queue_enqueue_interactive
queue_enqueue_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    bl      ansi_clear_screen

    // Check if queue is full
    bl      queue_is_full
    cmp     w0, 1
    b.eq    queue_enqueue_overflow

    // Prompt for value
    adrp    x0, prompt_value
    add     x0, x0, :lo12:prompt_value
    bl      printf

    bl      read_int
    mov     w19, w0                          // Save value

    // Enqueue value
    mov     w0, w19
    bl      queue_enqueue

    // Display queue
    bl      queue_display

    // Position cursor for success message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    // Print success message
    adrp    x0, msg_enqueued
    add     x0, x0, :lo12:msg_enqueued
    mov     w1, w19
    bl      printf
    bl      print_newline

    b       queue_enqueue_done

queue_enqueue_overflow:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_full
    add     x0, x0, :lo12:msg_full
    bl      printf
    bl      print_newline

queue_enqueue_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: queue_dequeue_interactive
// Interactive dequeue operation
// Parameters: none
// Returns: none
// ============================================================================
    .global queue_dequeue_interactive
queue_dequeue_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    bl      ansi_clear_screen

    // Check if queue is empty
    bl      queue_is_empty
    cmp     w0, 1
    b.eq    queue_dequeue_underflow

    // Dequeue value
    bl      queue_dequeue
    mov     w19, w0                          // Save dequeued value

    // Display queue
    bl      queue_display

    // Position cursor for success message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    // Print success message
    adrp    x0, msg_dequeued
    add     x0, x0, :lo12:msg_dequeued
    mov     w1, w19
    bl      printf
    bl      print_newline

    b       queue_dequeue_done

queue_dequeue_underflow:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

queue_dequeue_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: queue_peek_interactive
// Interactive peek operation
// Parameters: none
// Returns: none
// ============================================================================
    .global queue_peek_interactive
queue_peek_interactive:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    str     x19, [sp, 16]

    bl      ansi_clear_screen

    // Check if queue is empty
    bl      queue_is_empty
    cmp     w0, 1
    b.eq    queue_peek_empty

    // Peek at front
    bl      queue_peek
    mov     w19, w0                          // Save peeked value

    // Display queue
    bl      queue_display

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

    b       queue_peek_done

queue_peek_empty:
    // Position cursor for error message
    mov     w0, 22
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

queue_peek_done:
    ldr     x19, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: queue_enqueue
// Enqueues a value to the queue (internal function)
// Parameters:
//   w0 = value to enqueue
// Returns:
//   w0 = 1 on success, 0 on overflow
// ============================================================================
    .global queue_enqueue
queue_enqueue:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]

    mov     w19, w0                          // Save value

    // Load count
    adrp    x20, queue_count
    add     x20, x20, :lo12:queue_count
    ldr     w21, [x20]

    // Check if full
    cmp     w21, queue_max_size
    b.ge    queue_enqueue_fail

    // Load rear
    adrp    x22, queue_rear
    add     x22, x22, :lo12:queue_rear
    ldr     w23, [x22]

    // Calculate new rear: (rear + 1) % max_size
    add     w23, w23, 1
    mov     w24, queue_max_size
    udiv    w25, w23, w24                   // w25 = (rear+1) / max_size
    msub    w23, w25, w24, w23              // w23 = (rear+1) % max_size

    // Store value at new rear
    adrp    x24, queue_data
    add     x24, x24, :lo12:queue_data
    str     w19, [x24, w23, SXTW 2]

    // Update rear
    str     w23, [x22]

    // Increment count
    add     w21, w21, 1
    str     w21, [x20]

    mov     w0, 1                            // Success
    b       queue_enqueue_ret

queue_enqueue_fail:
    mov     w0, 0                            // Failure

queue_enqueue_ret:
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

// ============================================================================
// FUNCTION: queue_dequeue
// Dequeues a value from the queue (internal function)
// Parameters: none
// Returns:
//   w0 = dequeued value (or 0 if empty)
// ============================================================================
    .global queue_dequeue
queue_dequeue:
    stp     x29, x30, [sp, -48]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]

    // Load count
    adrp    x19, queue_count
    add     x19, x19, :lo12:queue_count
    ldr     w20, [x19]

    // Check if empty
    cmp     w20, 0
    b.le    queue_dequeue_fail

    // Load front
    adrp    x21, queue_front
    add     x21, x21, :lo12:queue_front
    ldr     w22, [x21]

    // Get value at front
    adrp    x23, queue_data
    add     x23, x23, :lo12:queue_data
    ldr     w0, [x23, w22, SXTW 2]          // Return value

    // Calculate new front: (front + 1) % max_size
    add     w22, w22, 1
    mov     w24, queue_max_size
    udiv    w25, w22, w24
    msub    w22, w25, w24, w22

    // Update front
    str     w22, [x21]

    // Decrement count
    sub     w20, w20, 1
    str     w20, [x19]

    b       queue_dequeue_ret

queue_dequeue_fail:
    mov     w0, 0                            // Return 0 if empty

queue_dequeue_ret:
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 48
    ret

// ============================================================================
// FUNCTION: queue_peek
// Views front element without removing it (internal function)
// Parameters: none
// Returns:
//   w0 = front value (or 0 if empty)
// ============================================================================
    .global queue_peek
queue_peek:
    stp     x29, x30, [sp, -32]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]

    // Load count
    adrp    x19, queue_count
    add     x19, x19, :lo12:queue_count
    ldr     w20, [x19]

    // Check if empty
    cmp     w20, 0
    b.le    queue_peek_fail

    // Load front
    adrp    x21, queue_front
    add     x21, x21, :lo12:queue_front
    ldr     w22, [x21]

    // Get value at front (don't remove)
    adrp    x23, queue_data
    add     x23, x23, :lo12:queue_data
    ldr     w0, [x23, w22, SXTW 2]

    b       queue_peek_ret

queue_peek_fail:
    mov     w0, 0                            // Return 0 if empty

queue_peek_ret:
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 32
    ret

// ============================================================================
// FUNCTION: queue_display
// Displays the queue with horizontal visualization
// Parameters: none
// Returns: none
// ============================================================================
    .global queue_display
queue_display:
    stp     x29, x30, [sp, -64]!
    mov     x29, sp
    stp     x19, x20, [sp, 16]
    stp     x21, x22, [sp, 32]
    stp     x23, x24, [sp, 48]

    bl      ansi_clear_screen

    // Load count
    adrp    x19, queue_count
    add     x19, x19, :lo12:queue_count
    ldr     w19, [x19]

    // Check if empty
    cmp     w19, 0
    b.le    queue_display_empty

    // Draw box
    mov     w0, 3
    mov     w1, 10
    mov     w2, 60
    mov     w3, 14
    mov     w4, 0                            // single-line
    bl      draw_box

    // Print title
    mov     w0, 4
    mov     w1, 12
    bl      ansi_move_cursor
    adrp    x0, queue_title
    add     x0, x0, :lo12:queue_title
    mov     w1, 56
    bl      print_centered

    // Draw separator
    mov     w0, 5
    mov     w1, 10
    mov     w2, 60
    mov     w3, 0
    bl      draw_horizontal_border_top

    // Print FRONT label
    mov     w0, 8
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, label_front
    add     x0, x0, :lo12:label_front
    bl      printf

    // Print REAR label
    mov     w0, 8
    mov     w1, 58
    bl      ansi_move_cursor
    adrp    x0, label_rear
    add     x0, x0, :lo12:label_rear
    bl      printf

    // Print arrows
    mov     w0, 9
    mov     w1, 16
    bl      ansi_move_cursor
    adrp    x0, .Larrow_down
    add     x0, x0, :lo12:.Larrow_down
    bl      printf

    // Calculate rear arrow position based on count
    mov     w0, 9
    add     w1, w19, w19                    // count * 2
    add     w1, w1, w19                     // count * 3
    add     w1, w1, w19                     // count * 4
    add     w1, w1, 12                      // offset
    bl      ansi_move_cursor
    adrp    x0, .Larrow_down
    add     x0, x0, :lo12:.Larrow_down
    bl      printf

    // Draw queue boxes
    mov     w0, 10
    mov     w1, 14
    bl      ansi_move_cursor
    adrp    x0, .Lbox_line
    add     x0, x0, :lo12:.Lbox_line
    bl      printf

    // Load front index
    adrp    x20, queue_front
    add     x20, x20, :lo12:queue_front
    ldr     w20, [x20]

    // Load queue data
    adrp    x21, queue_data
    add     x21, x21, :lo12:queue_data

    // Print values row
    mov     w0, 11
    mov     w1, 14
    bl      ansi_move_cursor

    mov     w22, 0                           // Counter
    mov     w23, w20                         // Current index = front

queue_display_loop:
    cmp     w22, w19                         // Compare with count
    b.ge    queue_display_bottom

    // Print cell start
    adrp    x0, .Lcell_start
    add     x0, x0, :lo12:.Lcell_start
    bl      printf

    // Print value
    ldr     w1, [x21, w23, SXTW 2]
    adrp    x0, .Lvalue_fmt
    add     x0, x0, :lo12:.Lvalue_fmt
    bl      printf

    // Move to next element (circular)
    add     w23, w23, 1
    mov     w24, queue_max_size
    udiv    w25, w23, w24
    msub    w23, w25, w24, w23

    add     w22, w22, 1
    b       queue_display_loop

queue_display_bottom:
    // Fill empty cells
    mov     w24, w19
queue_display_empty_cells:
    cmp     w24, queue_max_size
    b.ge    queue_display_bottom_done

    adrp    x0, .Lcell_empty
    add     x0, x0, :lo12:.Lcell_empty
    bl      printf

    add     w24, w24, 1
    b       queue_display_empty_cells

queue_display_bottom_done:
    // Print closing bracket
    adrp    x0, .Lcell_end
    add     x0, x0, :lo12:.Lcell_end
    bl      printf

    // Print bottom border
    bl      print_newline
    mov     w0, 12
    mov     w1, 14
    bl      ansi_move_cursor
    adrp    x0, .Lbox_line
    add     x0, x0, :lo12:.Lbox_line
    bl      printf

    // Print count info
    mov     w0, 14
    mov     w1, 15
    bl      ansi_move_cursor
    adrp    x0, label_count
    add     x0, x0, :lo12:label_count
    mov     w1, w19
    mov     w2, queue_max_size
    bl      printf

    bl      print_newline
    b       queue_display_done

queue_display_empty:
    // Position cursor for error message
    mov     w0, 10
    mov     w1, 1
    bl      ansi_move_cursor

    adrp    x0, msg_empty
    add     x0, x0, :lo12:msg_empty
    bl      printf
    bl      print_newline

queue_display_done:
    ldp     x23, x24, [sp, 48]
    ldp     x21, x22, [sp, 32]
    ldp     x19, x20, [sp, 16]
    ldp     x29, x30, [sp], 64
    ret

    .section .rodata
.Larrow_down:   .string "↓"
.Lbox_line:     .string "└────┴────┴────┴────┴────┴────┴────┴────┘"
.Lcell_start:   .string "│"
.Lcell_end:     .string "│"
.Lvalue_fmt:    .string "%3d "
.Lcell_empty:   .string "│    "
    .text

// ============================================================================
// FUNCTION: queue_is_empty
// Checks if queue is empty
// Parameters: none
// Returns:
//   w0 = 1 if empty, 0 otherwise
// ============================================================================
    .global queue_is_empty
queue_is_empty:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, queue_count
    add     x0, x0, :lo12:queue_count
    ldr     w0, [x0]

    cmp     w0, 0
    b.le    queue_empty_yes
    mov     w0, 0                            // Not empty
    b       queue_empty_ret

queue_empty_yes:
    mov     w0, 1                            // Empty

queue_empty_ret:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: queue_is_full
// Checks if queue is full
// Parameters: none
// Returns:
//   w0 = 1 if full, 0 otherwise
// ============================================================================
    .global queue_is_full
queue_is_full:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    adrp    x0, queue_count
    add     x0, x0, :lo12:queue_count
    ldr     w0, [x0]

    cmp     w0, queue_max_size
    b.ge    queue_full_yes
    mov     w0, 0                            // Not full
    b       queue_full_ret

queue_full_yes:
    mov     w0, 1                            // Full

queue_full_ret:
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: queue_clear
// Clears the queue (resets to empty state)
// Parameters: none
// Returns: none
// ============================================================================
    .global queue_clear
queue_clear:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Reset front to 0
    adrp    x0, queue_front
    add     x0, x0, :lo12:queue_front
    mov     w1, 0
    str     w1, [x0]

    // Reset rear to -1
    adrp    x0, queue_rear
    add     x0, x0, :lo12:queue_rear
    mov     w1, -1
    str     w1, [x0]

    // Reset count to 0
    adrp    x0, queue_count
    add     x0, x0, :lo12:queue_count
    mov     w1, 0
    str     w1, [x0]

    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// C++ INTERFACE FUNCTIONS (for asm-linked version)
// These functions expose internal state for visualization
// ============================================================================

// ============================================================================
// FUNCTION: queue_get_data
// Get pointer to internal queue array (for C++ visualization)
// Parameters: none
// Returns: x0 = pointer to queue_data array
// ============================================================================
    .global queue_get_data
queue_get_data:
    adrp    x0, queue_data
    add     x0, x0, :lo12:queue_data
    ret

// ============================================================================
// FUNCTION: queue_get_front
// Get current front index (for C++ visualization)
// Parameters: none
// Returns: w0 = front index
// ============================================================================
    .global queue_get_front
queue_get_front:
    adrp    x0, queue_front
    add     x0, x0, :lo12:queue_front
    ldr     w0, [x0]
    ret

// ============================================================================
// FUNCTION: queue_get_rear
// Get current rear index (for C++ visualization)
// Parameters: none
// Returns: w0 = rear index (-1 if empty)
// ============================================================================
    .global queue_get_rear
queue_get_rear:
    adrp    x0, queue_rear
    add     x0, x0, :lo12:queue_rear
    ldr     w0, [x0]
    ret

// ============================================================================
// FUNCTION: queue_get_count
// Get current number of elements (for C++ visualization)
// Parameters: none
// Returns: w0 = element count
// ============================================================================
    .global queue_get_count
queue_get_count:
    adrp    x0, queue_count
    add     x0, x0, :lo12:queue_count
    ldr     w0, [x0]
    ret

// ============================================================================
// FUNCTION: queue_get_capacity
// Get queue capacity (for C++ visualization)
// Parameters: none
// Returns: w0 = maximum capacity
// ============================================================================
    .global queue_get_capacity
queue_get_capacity:
    mov     w0, queue_max_size
    ret
