// ============================================================================
// main.asm - Main Entry Point and Menu System
// ============================================================================
// ARMv8 Assembly DSAV - Data Structures & Algorithms Visualizer
// Main program with menu-driven interface
// ============================================================================

include(`macros.m4')

    .data
    .balign 8

// ----------------------------------------------------------------------------
// Menu Strings
// ----------------------------------------------------------------------------
app_title:      .string "DATA STRUCTURES & ALGORITHMS VISUALIZER"
app_subtitle:   .string "ARMv8 Assembly Edition"

menu_option_1:  .string "[1] Array Operations"
menu_option_2:  .string "[2] Stack Operations"
menu_option_3:  .string "[3] Queue Operations"
menu_option_4:  .string "[4] Linked List Operations"
menu_option_5:  .string "[5] Binary Search Tree"
menu_option_6:  .string "[6] Sorting Algorithms"
menu_option_7:  .string "[7] Search Algorithms"
menu_option_0:  .string "[0] Exit"

menu_prompt:    .string "Enter your choice (0-7): "
invalid_choice: .string "\x1b[31mInvalid choice! Please select 0-7.\x1b[0m"
goodbye_msg:    .string "\n\x1b[32mThank you for using DSAV! Goodbye.\x1b[0m\n"
test_msg:       .string "\x1b[33m[This feature is not yet implemented]\x1b[0m\n"

    .text
    .balign 4
    .global main

// ============================================================================
// FUNCTION: main
// Main entry point of the program
// Parameters: none (standard C main)
// Returns: w0 = exit code (0 for success)
// ============================================================================
main:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Initialize random number generator
    bl      seed_random

    // Hide cursor for cleaner display
    bl      ansi_hide_cursor

main_loop:
    // Clear screen and display menu
    bl      display_main_menu

    // Get user choice
    adrp    x0, menu_prompt
    add     x0, x0, :lo12:menu_prompt
    bl      printf

    mov     w0, 0                            // min value
    mov     w1, 7                            // max value
    bl      read_int_range                  // Returns validated choice in w0

    // Dispatch to appropriate handler
    cmp     w0, 0
    b.eq    main_exit                       // Exit program

    cmp     w0, 1
    b.eq    handle_array_menu

    cmp     w0, 2
    b.eq    handle_stack_menu

    cmp     w0, 3
    b.eq    handle_queue_menu

    cmp     w0, 4
    b.eq    handle_linkedlist_menu

    cmp     w0, 5
    b.eq    handle_bst_menu

    cmp     w0, 6
    b.eq    handle_sort_menu

    cmp     w0, 7
    b.eq    handle_search_menu

    // Should never reach here due to read_int_range validation
    b       main_loop

handle_array_menu:
    bl      array_menu
    bl      wait_for_enter
    b       main_loop

handle_stack_menu:
    bl      stack_menu
    bl      wait_for_enter
    b       main_loop

handle_queue_menu:
    bl      queue_menu
    bl      wait_for_enter
    b       main_loop

handle_linkedlist_menu:
    bl      linkedlist_menu
    bl      wait_for_enter
    b       main_loop

handle_bst_menu:
    bl      bst_menu
    bl      wait_for_enter
    b       main_loop

handle_sort_menu:
    bl      sort_menu
    bl      wait_for_enter
    b       main_loop

handle_search_menu:
    bl      search_menu
    bl      wait_for_enter
    b       main_loop

main_exit:
    // Show cursor before exit
    bl      ansi_show_cursor

    // Clear screen
    bl      ansi_clear_screen

    // Print goodbye message
    adrp    x0, goodbye_msg
    add     x0, x0, :lo12:goodbye_msg
    bl      printf

    // Return 0 (success)
    mov     w0, 0
    ldp     x29, x30, [sp], 16
    ret

// ============================================================================
// FUNCTION: display_main_menu
// Displays the main menu with title and options
// Parameters: none
// Returns: none
// ============================================================================
    .global display_main_menu
display_main_menu:
    stp     x29, x30, [sp, -16]!
    mov     x29, sp

    // Clear screen
    bl      ansi_clear_screen

    // Draw main menu box (double-line style)
    mov     w0, 3                            // row
    mov     w1, 15                           // column
    mov     w2, 54                           // width
    mov     w3, 19                           // height (increased for new option)
    mov     w4, 1                            // style (1 = double line)
    bl      draw_box

    // Print title (centered)
    mov     w0, 4                            // row
    mov     w1, 17                           // column
    bl      ansi_move_cursor
    adrp    x0, app_title
    add     x0, x0, :lo12:app_title
    mov     w1, 50
    bl      print_centered

    // Print subtitle
    mov     w0, 5                            // row
    mov     w1, 17                           // column
    bl      ansi_move_cursor
    adrp    x0, app_subtitle
    add     x0, x0, :lo12:app_subtitle
    mov     w1, 50
    bl      print_centered

    // Draw separator line
    mov     w0, 6                            // row
    mov     w1, 15                           // column
    mov     w2, 54                           // width
    mov     w3, 1                            // style
    bl      draw_horizontal_border_top

    // Print menu options
    mov     w0, 9                            // row
    mov     w1, 20                           // column
    bl      ansi_move_cursor
    adrp    x0, menu_option_1
    add     x0, x0, :lo12:menu_option_1
    bl      printf

    mov     w0, 10
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_option_2
    add     x0, x0, :lo12:menu_option_2
    bl      printf

    mov     w0, 11
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_option_3
    add     x0, x0, :lo12:menu_option_3
    bl      printf

    mov     w0, 12
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_option_4
    add     x0, x0, :lo12:menu_option_4
    bl      printf

    mov     w0, 13
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_option_5
    add     x0, x0, :lo12:menu_option_5
    bl      printf

    mov     w0, 14
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_option_6
    add     x0, x0, :lo12:menu_option_6
    bl      printf

    mov     w0, 15
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_option_7
    add     x0, x0, :lo12:menu_option_7
    bl      printf

    mov     w0, 16
    mov     w1, 20
    bl      ansi_move_cursor
    adrp    x0, menu_option_0
    add     x0, x0, :lo12:menu_option_0
    bl      printf

    // Draw another separator
    mov     w0, 18                           // row (adjusted for new option)
    mov     w1, 15                           // column
    mov     w2, 54                           // width
    mov     w3, 1                            // style
    bl      draw_horizontal_border_top

    // Position cursor for input prompt
    mov     w0, 20                           // row (adjusted)
    mov     w1, 18                           // column
    bl      ansi_move_cursor

    ldp     x29, x30, [sp], 16
    ret
